/*
Copyright (c) 2019, Thomas DiModica
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the names of the copyright holders nor the names of other
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// If you include this twice, bad things will happen and you deserve it.

#include <map>
#include <deque>
#include <memory>
#include <vector>
#include <cstdio>

class Machine; // Forward declaration for Op.

// Abstract base class for all operations.
class Op
 {
public:
   virtual void perform(Machine&) = 0;
 };

const size_t BELT_SIZE = 64U;

// Enum to define when we are addressing the belt vs when we are using an immediate.
enum BELT_LOCATION
 {
   B1,
   B2,
   B3,
   B4,
   B5,
   B6,
   B7,
   B8,
   B9,
   B10,
   B11,
   B12,
   B13,
   B14,
   B15,
   B16,
   B17,
   B18,
   B19,
   B20,
   B21,
   B22,
   B23,
   B24,
   B25,
   B26,
   B27,
   B28,
   B29,
   B30,
   B31,
   B32,
   B33,
   B34,
   B35,
   B36,
   B37,
   B38,
   B39,
   B40,
   B41,
   B42,
   B43,
   B44,
   B45,
   B46,
   B47,
   B48,
   B49,
   B50,
   B51,
   B52,
   B53,
   B54,
   B55,
   B56,
   B57,
   B58,
   B59,
   B60,
   B61,
   B62,
   B63,
   B64
 };

enum NONE_TYPE
 {
   NONE_VAL = 1
 };
enum NAR_TYPE
 {
   NAR_VAL = 2
 };

enum BELT_TYPE
 {
   WORD
 };

struct BELT_T
 {
   union
    {
      int w;
    };
   int length;
   int narflags;
   BELT_TYPE type;

   BELT_T() : w(0), length(0), narflags(0), type (WORD) { }
   BELT_T(int w) : w(w), length(0), narflags(0), type(WORD) { }
   BELT_T(NONE_TYPE) : w(0), length(0), narflags(NONE_VAL), type(WORD) { }
   BELT_T(NAR_TYPE) : w(0), length(0), narflags(NAR_VAL), type(WORD) { }
 };

enum FRAME_TYPE
 {
   CALL_FRAME, // Associated BFrame
   LOOP_FRAME  // No associated BFrame
 };

class IFrame
 {
public:
   size_t pc; // Index into the instruction array to the current instruction.
   size_t ip; // Index into the operation array to the current operation.
   int flags; // The current set flags in the operation gang.
   int c; // The number of instructions in the future that those flags will appear.
   FRAME_TYPE type;

   IFrame() : pc(0U), ip(0U), flags(0), c(0), type(CALL_FRAME) { }
 };

class BFrame
 {
public:
   std::deque<BELT_T> belt;
   std::deque<std::deque<BELT_T> > retire;
   std::deque<std::vector<std::pair<size_t, BELT_T> > > loads; // Loads have a latency of 3 and are alias safe.

   BELT_T getLoc(BELT_LOCATION loc)
    {
      if (loc > belt.size())
       {
         return NAR_VAL;
       }
      else
       {
         return belt[belt.size() - 1 - loc];
       }
    }
 };

class Machine
 {
private:
   std::map<std::string, size_t> labels; // The instruction that label refers to.
   std::map<std::string, std::vector<size_t*> > prereferences; // References to labels that haven't been defined yet.

public:
   std::vector<std::vector<std::unique_ptr<Op> > > instructions;
   std::vector<IFrame> stack;
   std::vector<BFrame> belt;
   bool faultFlag;
   std::map<std::string, BELT_T> loads; // BELT_T is location to read. Read is on pickup.
   int * memory;

   // Builder functions : these all return the machine that is being operated on.
   // At run time, these functions compile the genAsm into something that will execute.
   // Each function creates an operation and performs instructions.back().push_back().
   Machine& add (BELT_LOCATION, BELT_LOCATION);
   Machine& add (BELT_LOCATION, BELT_T);
   Machine& br (BELT_LOCATION);
   Machine& con (BELT_T);
   Machine& fault ();
   Machine& innertr (BELT_LOCATION pred, int drops, BELT_LOCATION dest);
   Machine& load (BELT_LOCATION);
   Machine& load (BELT_LOCATION, int latency);
   Machine& load (BELT_LOCATION, const std::string& tag);
   Machine& pick (BELT_LOCATION pred, BELT_LOCATION);
   Machine& pick (BELT_LOCATION pred, BELT_LOCATION, BELT_LOCATION);
   Machine& pickup (const std::string& tag);
   Machine& recur (BELT_LOCATION pred, BELT_LOCATION, BELT_LOCATION); // Pick that looks at NaR
   Machine& refuse (const std::string& tag);
   Machine& retnfl (BELT_LOCATION pred);
   Machine& store (BELT_LOCATION, BELT_LOCATION);
   Machine& sub (BELT_LOCATION, BELT_LOCATION);
   Machine& sub (BELT_LOCATION, BELT_T);

   // Create the next instruction to be built.
   Machine& next()
    {
      instructions.push_back(std::vector<std::unique_ptr<Op> >());
      return *this;
    }
   // Define a label. Defining a label implicitly calls next().
   Machine& label(const std::string&);

   // Run the code.
   void run()
    {
      stack.push_back(IFrame());
      belt.push_back(BFrame());
      faultFlag = false;
      while ((stack.back().pc < instructions.size()) && (false == faultFlag))
       {
         // Note 1 : the instruction is completed in the event of a fault.
         // Note 2 : branches work because branches are ALWAYS to labels,
         //    and labels are ALWAYS preceded by an instructions whose only operation is fault.
         //    (This prevents un-Mill-like falling off of an EBB.)
         while ((stack.back().ip < instructions[stack.back().pc].size()))
          {
            const std::unique_ptr<Op>& current = instructions[stack.back().pc][stack.back().ip];
            ++stack.back().ip;
            current->perform(*this);
          }
         stack.back().ip = 0U;
         ++stack.back().pc;

         if ((false == belt.back().loads.empty()) && (false == belt.back().loads[0].empty()))
          {
            if (true == belt.back().retire.empty())
             {
               belt.back().retire.push_back(std::deque<BELT_T>());
             }
            for (auto load : belt.back().loads[0])
             {
               belt.back().retire.front()[load.first] = memory[load.second.w];
             }
          }
         if (false == belt.back().retire.empty())
          {
            belt.back().belt.insert(belt.back().belt.end(), belt.back().retire.front().begin(), belt.back().retire.front().end());
            belt.back().retire.pop_front();
            while (belt.back().belt.size() > BELT_SIZE)
             {
               belt.back().belt.pop_front();
             }
/*
int loca = 1;
for (auto loc : belt.back().belt)
 {
   std::printf("Belt at %d is %d\n", loca++, loc.w);
 }
*/
          }
       }
    }
 };

class add_op : public Op
 {
public:
   BELT_LOCATION lhs, rhs;
   void perform(Machine& m)
    {
      if (true == m.belt.back().retire.empty())
       {
         m.belt.back().retire.push_back(std::deque<BELT_T>());
       }
      // The instruction-level polymorphism goes here.
      m.belt.back().retire.front().push_back(m.belt.back().getLoc(lhs).w + m.belt.back().getLoc(rhs).w);
    }
 };

class addi_op : public Op
 {
public:
   BELT_LOCATION lhs;
   BELT_T rhs;
   void perform(Machine& m)
    {
      if (true == m.belt.back().retire.empty())
       {
         m.belt.back().retire.push_back(std::deque<BELT_T>());
       }
      // The instruction-level polymorphism goes here.
      m.belt.back().retire.front().push_back(m.belt.back().getLoc(lhs).w + rhs.w);
    }
 };

class con_op : public Op
 {
public:
   BELT_T value;
   void perform(Machine& m)
    {
//std::printf("Putting %d on belt...\n", value.w);
      if (true == m.belt.back().retire.empty())
       {
         m.belt.back().retire.push_back(std::deque<BELT_T>());
       }
      // The instruction-level polymorphism goes here.
      m.belt.back().retire.front().push_back(value);
    }
 };

class store_op : public Op
 {
public:
   BELT_LOCATION location, value;
   void perform(Machine& m)
    {
      // The instruction-level polymorphism goes here.
      int loc = m.belt.back().getLoc(location).w;
      int val = m.belt.back().getLoc(value).w;
//std::printf("Storing (%d) %d at (%d) %d...\n", value, val, location, loc);
      m.memory[loc] = val;
      if (255 == loc)
       {
         std::putchar(val);
       }
    }
 };

Machine& Machine::con (BELT_T val)
 {
   std::unique_ptr<con_op> op = std::make_unique<con_op>();
   op->value = val;
   instructions.back().push_back(std::move(op));
   return *this;
 }

Machine& Machine::store (BELT_LOCATION loc, BELT_LOCATION val)
 {
   std::unique_ptr<store_op> op = std::make_unique<store_op>();
   op->location = loc;
   op->value = val;
   instructions.back().push_back(std::move(op));
   return *this;
 }

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

#include "Mill.h"

int main (void)
 {
   Machine m;

   m.next();
   m.con('H').con('e').con('l').con('o').con(' ').con('W').con('r').con('d').con('!').con('\n').con(255);
   m.next();
   m.store(B1, B11).store(B1, B10).store(B1, B9).store(B1, B9).store(B1, B8).store(B1, B7).store(B1, B6).store(B1, B8).store(B1, B5).store(B1, B9).store(B1, B4).store(B1, B3).store(B1, B2);

   m.next();
   m.next();

   m.con('H').con(255).next();
   m.store(B1, B2).next();
   m.con('e').con(255).next();
   m.store(B1, B2).next();
   m.con('l').con(255).next();
   m.store(B1, B2).next();
   m.con('l').con(255).next();
   m.store(B1, B2).next();
   m.con('o').con(255).next();
   m.store(B1, B2).next();
   m.con(',').con(255).next();
   m.store(B1, B2).next();
   m.con(' ').con(255).next();
   m.store(B1, B2).next();
   m.con('W').con(255).next();
   m.store(B1, B2).next();
   m.con('o').con(255).next();
   m.store(B1, B2).next();
   m.con('r').con(255).next();
   m.store(B1, B2).next();
   m.con('l').con(255).next();
   m.store(B1, B2).next();
   m.con('d').con(255).next();
   m.store(B1, B2).next();
   m.con('!').con(255).next();
   m.store(B1, B2).next();
   m.con('\n').con(255).next();
   m.store(B1, B2).next();

   m.store(B29, B39).store(B29, B38).store(B29, B37).store(B29, B37).store(B29, B36).store(B29, B35).store(B29, B34).store(B29, B36).store(B29, B33).store(B29, B37).store(B29, B32).store(B29, B31).store(B29, B30);

   m.con('H').con(255).next();
   m.store(B1, B2).next();
   m.con('e').con(255).next();
   m.store(B1, B2).next();
   m.con('l').con(255).next();
   m.store(B1, B2).next();
   m.con('l').con(255).next();
   m.store(B1, B2).next();
   m.con('o').con(255).next();
   m.store(B1, B2).next();
   m.con(',').con(255).next();
   m.store(B1, B2).next();
   m.con(' ').con(255).next();
   m.store(B1, B2).next();
   m.con('W').con(255).next();
   m.store(B1, B2).next();
   m.con('o').con(255).next();
   m.store(B1, B2).next();
   m.con('r').con(255).next();
   m.store(B1, B2).next();
   m.con('l').con(255).next();
   m.store(B1, B2).next();
   m.con('d').con(255).next();
   m.store(B1, B2).next();
   m.con('!').con(255).next();
   m.store(B1, B2).next();
   m.con('\n').con(255).next();
   m.store(B1, B2).next();

   m.store(B57, B64).store(B57, B63).store(B57, B62).store(B57, B61).store(B57, B60).store(B57, B59).store(B57, B58);

   m.memory = new int [1024];

   m.run();

   return 0;
 }

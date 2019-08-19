/*
Homebrew game for the Nintendo 3DS
Copyright (C) 2019  HueStudios

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "autoplay.h"

void Autoplay_FakeInput(LevelRuntimeData* toPlay) {
  if (toPlay->BreakerCount > 0) {
    short higherDistance = toPlay->Breakers[0]->Position->Y;
    short targetPosition = toPlay->Breakers[0]->Position->X;
    for (unsigned char i = 1; i<toPlay->BreakerCount;i++) {
      if (toPlay->Breakers[i]->Position->Y > higherDistance) {
        higherDistance = toPlay->Breakers[i]->Position->Y;
        targetPosition = toPlay->Breakers[i]->Position->X;
      }
    }
    toPlay->TargetPadPosition = targetPosition - (toPlay->PadLength/2);
  }
}

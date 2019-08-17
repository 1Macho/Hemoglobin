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

#include "level.h"

unsigned char Level_TickBall(BreakerBall* target, LevelRuntimeData* level) {
  target->Position->X += target->Velocity->X;
  target->Position->Y += target->Velocity->Y;

  //Collision with the right and left walls
  if (Breaker_RightEnd(target) > SCREEN_WIDTH) {
    Point_Invert(target->Velocity, 0);
    target->Position->X = SCREEN_WIDTH - BREAKER_SIDE;
  }

  if (Breaker_LeftEnd(target) < 0) {
    Point_Invert(target->Velocity, 0);
    target->Position->X = 0;
  }

  //Collision with the top wall
  if ((Breaker_TopEnd(target) < 0)) {
    Point_Invert(target->Velocity, 1);
    target->Position->Y = 0;
  }

  if (Breaker_BottomEnd(target) > SCREEN_HEIGHT - PAD_HEIGHT) {
    if (Breaker_ResolveCollisionBelow(target, level->PadPosition, level->PadPosition + PAD_LENGTH)) {
      Point_Invert(target->Velocity, 1);
      target->Position->Y = SCREEN_HEIGHT - PAD_HEIGHT - BREAKER_SIDE;
    }
    else {
      return 1;
    }
  }
  return 0;
}

unsigned char Level_TickLevel (LevelRuntimeData* level) {
  short PadDifference = level->TargetPadPosition - level->PadPosition;
  level->PadPosition += PadDifference / 2;
  for (unsigned char i = 0; i < level->BreakerCount; i++) {
    if (Level_TickBall(level->Breakers[i], level)) {
      return 1;
    }
  }
  return 0;
}

void Level_DrawLevel(LevelRuntimeData* level, u32 clrRec, u32 clrPad) {
  for (unsigned char i = 0; i < level->BreakerCount; i++) {
    Breaker_DrawBreaker(level->Breakers[i], clrRec);
  }
  C2D_DrawRectangle(level->PadPosition, SCREEN_HEIGHT - PAD_HEIGHT, 0, PAD_LENGTH, PAD_HEIGHT, clrPad, clrPad, clrPad, clrPad);
}

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

#ifndef LEVEL
#define LEVEL

#include "point.h"
#include "breaker.h"
#include "constants.h"
#include <citro2d.h>


#define PAD_LENGTH 32
#define PAD_HEIGHT 6

#define TOUCH_WIDTH 310
#define TOUCH_PADDING 5


typedef struct LevelRuntimeData {
  BreakerBall* Breakers[32];
  unsigned char BreakerCount;
  short TargetPadPosition;
  short PadPosition;
  unsigned char BlockStates [11][6];
} LevelRuntimeData;

unsigned char Level_TickBall(BreakerBall* target, LevelRuntimeData* level);
unsigned char Level_TickLevel (LevelRuntimeData* level);
void Level_DrawLevel(LevelRuntimeData* level, u32 clrRec, u32 clrPad);
void Level_HandleInput(LevelRuntimeData* level);

#endif

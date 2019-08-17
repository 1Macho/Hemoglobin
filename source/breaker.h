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

#ifndef BREAKER
#define BREAKER

#include "point.h"
#include "constants.h"
#include <citro2d.h>

#define BREAKER_SIDE 8

typedef struct BreakerBall {
  Point* Position;
  Point* Velocity;
  unsigned char Saveable;
} BreakerBall;

short Breaker_TopEnd(BreakerBall* target);
short Breaker_BottomEnd(BreakerBall* target);
short Breaker_LeftEnd(BreakerBall* target);
short Breaker_RightEnd(BreakerBall* target);
void Breaker_ClampPosition(BreakerBall* target, short left, short right, short top, short bottom);
unsigned char Breaker_ResolveCollisionVertical(BreakerBall* target, short start, short end);
unsigned char Breaker_ResolveCollisionHorizontal(BreakerBall* target, short start, short end);

void Breaker_DrawBreaker(BreakerBall* target, u32 clrRec);

#endif

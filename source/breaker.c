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


#include "breaker.h"

BreakerBall* Breaker_CreateNew(short x, short y, short dx, short dy) {
  BreakerBall* target = malloc(sizeof(BreakerBall));
  Point* pos = malloc(sizeof(Point));
  Point* vel = malloc(sizeof(Point));
  pos->X = x;
  pos->Y = y;
  vel->X = dx;
  vel->Y = dy;
  target->Position = pos;
  target->Velocity = vel;
  return target;
}


short Breaker_TopEnd(BreakerBall* target) {
  return target->Position->Y;
}
short Breaker_BottomEnd(BreakerBall* target) {
  return target->Position->Y + BREAKER_SIDE;
}

short Breaker_LeftEnd(BreakerBall* target) {
  return target->Position->X;
}

short Breaker_RightEnd(BreakerBall* target) {
  return target->Position->X + BREAKER_SIDE;
}

unsigned char Breaker_ResolveCollisionVertical(BreakerBall* target, short start, short end) {
  unsigned short result = 0;
  if ((Breaker_LeftEnd(target) >= start) & (Breaker_LeftEnd(target) <= end)) {
    result = 1;
  }
  if ((Breaker_RightEnd(target) >= start) & (Breaker_RightEnd(target) <= end)) {
    result = result + 2;
  }
  return result;
}

unsigned char Breaker_ResolveCollisionHorizontal(BreakerBall* target, short start, short end) {
  unsigned short result = 0;
  if ((Breaker_TopEnd(target) >= start) & (Breaker_TopEnd(target) <= end)) {
    result = 1;
  }
  if ((Breaker_BottomEnd(target) >= start) & (Breaker_BottomEnd(target) <= end)) {
    result = result + 2;
  }
  return result;
}

void Breaker_DrawBreaker(BreakerBall* target, u32 clrRec) {
  C2D_DrawRectangle(target->Position->X, target->Position->Y, 0, BREAKER_SIDE, BREAKER_SIDE, clrRec, clrRec, clrRec, clrRec);
}

void Breaker_CollideTop(BreakerBall* target, short surface) {
  target->Position->Y = surface + 1;
  Point_Invert(target->Velocity, 1);
}

void Breaker_CollideBottom(BreakerBall* target, short surface) {
  target->Position->Y = surface - BREAKER_SIDE - 1;
  Point_Invert(target->Velocity, 1);
}

void Breaker_CollideRight(BreakerBall* target, short surface) {
  target->Position->X = surface - BREAKER_SIDE - 1;
  Point_Invert(target->Velocity, 0);
}

void Breaker_CollideLeft(BreakerBall* target, short surface) {
  target->Position->X = surface + 1;
  Point_Invert(target->Velocity, 0);
}

unsigned char Breaker_CollideWithBlock(BreakerBall* target, short blockStartX, short blockEndX, short blockStartY, short blockEndY) {
  unsigned char collisionMaskV = Breaker_ResolveCollisionVertical(target, blockStartX, blockEndX);
  unsigned char collisionMaskH = Breaker_ResolveCollisionHorizontal(target, blockStartY, blockEndY);
  unsigned char left = collisionMaskV % 2;
  unsigned char right = collisionMaskV / 2;
  unsigned char top = collisionMaskH % 2;
  unsigned char bottom = collisionMaskH / 2;

  //Trivial collision cases
  if (left & right & top & bottom) {
    return 1;
  }
  if (left & right) {
    if (top) {
      Breaker_CollideTop(target, blockEndY);
      return 1;
    }
    if (bottom) {
      Breaker_CollideBottom(target, blockStartY);
      return 1;
    }
  }
  if (top & bottom) {
    if (left) {
      Breaker_CollideLeft(target, blockEndX);
      return 1;
    }
    if (right) {
      Breaker_CollideRight(target, blockStartX);
      return 1;
    }
  }

  //Non-trivial collision cases

  if (top & left) {
    short dx = blockEndX - Breaker_LeftEnd(target);
    short dy = blockEndY - Breaker_TopEnd(target);
    if (dx <= dy) {
      Breaker_CollideLeft(target, blockEndX);
    }
    if (dy <= dx) {
      Breaker_CollideTop(target, blockEndY);
    }
    return 1;
  }

  if (top & right) {
    short dx = Breaker_RightEnd(target) - blockStartX;
    short dy = blockEndY - Breaker_TopEnd(target);
    if (dx <= dy) {
      Breaker_CollideRight(target, blockStartX);
    }
    if (dy <= dx) {
      Breaker_CollideTop(target, blockEndY);
    }
    return 1;
  }

  if (bottom & left) {
    short dx = blockEndX - Breaker_LeftEnd(target);
    short dy = Breaker_BottomEnd(target) - blockStartY;
    if (dx <= dy) {
      Breaker_CollideLeft(target, blockEndX);
    }
    if (dy <= dx) {
      Breaker_CollideBottom(target, blockStartY);
    }
    return 1;
  }

  if (bottom & right) {
    short dx = Breaker_RightEnd(target) - blockStartX;
    short dy = Breaker_BottomEnd(target) - blockStartY;
    if (dx <= dy) {
      Breaker_CollideRight(target, blockStartX);
    }
    if (dy <= dx) {
      Breaker_CollideBottom(target, blockStartY);
    }
    return 1;
  }
  return 0;
}

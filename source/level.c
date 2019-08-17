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

unsigned char Level_VerifyBallBlockCollision(BreakerBall* target, unsigned char blockX, unsigned char blockY) {
  short blockStartX = BLOCK_START_H + ((BLOCK_WIDTH + BLOCK_PADDING)*blockX);
  short blockEndX = blockStartX + BLOCK_WIDTH;
  short blockStartY = BLOCK_START_V + ((BLOCK_HEIGHT + BLOCK_PADDING)*blockY);
  short blockEndY = blockStartY + BLOCK_HEIGHT;
  unsigned char collisionMaskV = Breaker_ResolveCollisionVertical(target, blockStartX, blockEndX);
  unsigned char collisionMaskH = Breaker_ResolveCollisionHorizontal(target, blockStartY, blockEndY);
  unsigned char left = collisionMaskV % 2;
  unsigned char right = collisionMaskV / 2;
  unsigned char top = collisionMaskH % 2;
  unsigned char bottom = collisionMaskH / 2;
  //printf("\x1b[9;1HCb:  %03d; %03d; %03d; %03d", left, right, top, bottom);
  if (left & right & top & bottom) {
    return 1;
  }
  if (left & right) {
    if (top) {
      target->Position->Y = blockEndY + 1;
      Point_Invert(target->Velocity, 1);
      return 1;
    }
    if (bottom) {
      target->Position->Y = blockStartY - BREAKER_SIDE - 1;
      Point_Invert(target->Velocity, 1);
      return 1;
    }
  }
  if (top & bottom) {
    if (right) {
      target->Position->X = blockStartX - BREAKER_SIDE - 1;
      Point_Invert(target->Velocity, 0);
      return 1;
    }
    if (left) {
      target->Position->X = blockEndX + 1;
      Point_Invert(target->Velocity, 0);
      return 1;
    }
  }
  return 0;
}

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
    if (Breaker_ResolveCollisionVertical(target, level->PadPosition, level->PadPosition + PAD_LENGTH)) {
      Point_Invert(target->Velocity, 1);
      target->Position->Y = SCREEN_HEIGHT - PAD_HEIGHT - BREAKER_SIDE;
    }
    else {
      return 1;
    }
  }

  for (unsigned char x = 0; x < BLOCK_HORIZONTAL; x++) {
    for (unsigned char y = 0; y < BLOCK_VERTICAL; y++) {
      if (level->BlockStates[x][y] == 0x1) {
        if (Level_VerifyBallBlockCollision(target, x, y)) {
          level->BlockStates[x][y] = 0x0;
        }
      }
    }
  }
 /*

  short closestBlockX = (target->Position->X - BLOCK_START_H) / (BLOCK_WIDTH + BLOCK_PADDING);
  short closestBlockY = (target->Position->Y - BLOCK_START_V) / (BLOCK_HEIGHT + BLOCK_PADDING);

  short minBlockX = 0;
  short minBlockY = 0;
  short maxBlockX = BLOCK_HORIZONTAL - 1;
  short maxBlockY = BLOCK_VERTICAL - 1;
  if (closestBlockX - 1 > minBlockX) {
    minBlockX = closestBlockX - 1;
  }
  if (closestBlockX + 1 < maxBlockX) {
    maxBlockX = closestBlockX + 1;
  }
  if (closestBlockY - 1 > minBlockY) {
    minBlockY = closestBlockY - 1;
  }
  if (closestBlockY + 1 < maxBlockY) {
    maxBlockY = closestBlockY + 1;
  }

  for (unsigned char i = closestBlockX - 1; i <= closestBlockX + 1; i++) {
    for (unsigned char j = closestBlockY - 1; j <= closestBlockY + 1; j++) {
      if ((i > minBlockX) & (i < maxBlockX) & (j > minBlockY) & (j < maxBlockY)) {
        if (level->BlockStates[i][j] == 0x1) {
          if (Level_VerifyBallBlockCollision(target, i, j)) {
            level->BlockStates[i][j] = 0x0;
          }
        }
      }
    }
  }

  printf("\x1b[9;1HCb:  %03d; %03d; %03d; %03d", minBlockX, maxBlockX, minBlockY, maxBlockY);*/

  return 0;
}

unsigned char Level_TickLevel (LevelRuntimeData* level) {
  short PadDifference = level->TargetPadPosition - level->PadPosition;
  level->PadPosition += PadDifference / 3;
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

  for (unsigned char x = 0; x < BLOCK_HORIZONTAL; x++) {
    for (unsigned char y = 0; y < BLOCK_VERTICAL; y++) {
      if (level->BlockStates[x][y] == 0x1) {
        short xPos = BLOCK_START_H + (x * (BLOCK_WIDTH + BLOCK_PADDING));
        short yPos = BLOCK_START_V + (y * (BLOCK_HEIGHT + BLOCK_PADDING));
        C2D_DrawRectangle(xPos, yPos, 0, BLOCK_WIDTH, BLOCK_HEIGHT, clrPad, clrPad, clrPad, clrPad);
      }
    }
  }
}

void Level_HandleInput(LevelRuntimeData* level) {
  touchPosition touch;
  hidTouchRead(&touch);

  if ((touch.px >= TOUCH_PADDING) & (touch.px <= TOUCH_WIDTH - TOUCH_PADDING)) {
    short interpolation_id = touch.px - TOUCH_PADDING;
    long interpolation = interpolation_id * SCREEN_WIDTH / (TOUCH_WIDTH - TOUCH_PADDING * 2);
    level->TargetPadPosition = interpolation - (PAD_LENGTH / 2);
  }
}

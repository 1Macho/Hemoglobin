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

LevelRuntimeData* Level_CreateNew (unsigned char difficulty) {
  LevelRuntimeData* result = malloc(sizeof(LevelRuntimeData));
  result->Difficulty = difficulty;
  long accumulatedHardness = 0;
  for (int y = 0; y < BLOCK_VERTICAL; y++) {
    double calculatedHardness = tan((M_PI*((double)y))/(20.0)) * (double)(difficulty);
    unsigned short toSet = (unsigned short) calculatedHardness;
    if (toSet < 1) {toSet = 1;}
    if (toSet > 255) {toSet = 255;}
    for (int x = 0; x < BLOCK_HORIZONTAL; x++) {
      result->BlockStates[x][y] = toSet;
      accumulatedHardness = accumulatedHardness + toSet;
    }
  }
  short ballCountIntermediate = (accumulatedHardness/(BLOCK_HORIZONTAL*BLOCK_VERTICAL));
  if (ballCountIntermediate > 8) { ballCountIntermediate = 8; }
  if (ballCountIntermediate < 1) { ballCountIntermediate = 1; }
  result->TargetBreakerCount = ballCountIntermediate;
  result->EnabledBlocks = BLOCK_VERTICAL * BLOCK_HORIZONTAL;
  result->TargetPadPosition = (SCREEN_WIDTH / 2) - (PAD_LENGTH / 2);
  result->PadPosition = (SCREEN_WIDTH / 2) - (PAD_LENGTH / 2);
  result->BreakerCount = 0;
  result->PadLength = (300 + (difficulty * -1)) / 2;
  result->BreakerSpawnTime = 128;
  result->SafeSpawn = 1;
  Level_SetNewSpawnPosition(result);
  //printf("%x", &initialBreaker);
  return result;
}

void Level_SetNewSpawnPosition(LevelRuntimeData* level) {
  unsigned char randomPos = rand() % 4;
  unsigned short spawnPos = SPAWN_AREA_BEGIN + (SPAWN_AREA_SEGMENTS * randomPos);
  level->BreakerSpawnPos = spawnPos;
}

unsigned char Level_VerifyBallBlockCollision(BreakerBall* target, unsigned char blockX, unsigned char blockY) {
  short blockStartX = BLOCK_START_H + ((BLOCK_WIDTH + BLOCK_PADDING)*blockX);
  short blockEndX = blockStartX + BLOCK_WIDTH;
  short blockStartY = BLOCK_START_V + ((BLOCK_HEIGHT + BLOCK_PADDING)*blockY);
  short blockEndY = blockStartY + BLOCK_HEIGHT;
  return Breaker_CollideWithBlock(target, blockStartX, blockEndX, blockStartY, blockEndY);
}

void Level_RemoveBreaker (LevelRuntimeData* level, unsigned char toRemove) {
  for(unsigned char i = toRemove + 1; i < level->BreakerCount;i++) {
    level->Breakers[i-1] = level->Breakers[i];
  }
  level->BreakerCount = level->BreakerCount - 1;
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
    if (Breaker_ResolveCollisionVertical(target, level->PadPosition, level->PadPosition + level->PadLength)) {
      Point_Invert(target->Velocity, 1);
      target->Position->Y = SCREEN_HEIGHT - PAD_HEIGHT - BREAKER_SIDE;
      level->Multiplier += 1;
      level->Score += (target->Score * level->Multiplier);
      target->Score = 0;
      target->Multiplier = 0;
    }
    else {
      level->Multiplier = 0;
      return 1;
    }
  }

  for (unsigned char x = 0; x < BLOCK_HORIZONTAL; x++) {
    for (unsigned char y = 0; y < BLOCK_VERTICAL; y++) {
      if (level->BlockStates[x][y] > 0x0) {
        if (Level_VerifyBallBlockCollision(target, x, y)) {
          level->BlockStates[x][y] = level->BlockStates[x][y] - 1;
          if (level->BlockStates[x][y] == 0x0) {
            level->EnabledBlocks = level->EnabledBlocks - 1;
            target->Multiplier += 1;
            unsigned char generatedScore = rand()%5 + 4;
            target->Score += generatedScore * target->Multiplier;
          }
        }
      }
    }
  }
  return 0;
}

unsigned char Level_TickLevel (LevelRuntimeData* level) {
  unsigned char ticks = (level->Difficulty / 80) + 3;
  if (ticks == 0) {ticks = 1;}
  for (unsigned char i = 0; i < ticks; i++) {
    if (level->BreakerCount < level->TargetBreakerCount) {
      if (level->BreakerSpawnTime > 0) {
        level->BreakerSpawnTime = level->BreakerSpawnTime - 1;
      }
      else {
        unsigned char randomOrientation = rand() % 2;
        short hVelocity = -1;
        if (randomOrientation == 1) {
          hVelocity = randomOrientation;
        }
        BreakerBall* newBall = Breaker_CreateNew(level->BreakerSpawnPos, SCREEN_HEIGHT / 2, hVelocity, -1);
        level->Breakers[level->BreakerCount] = newBall;
        level->BreakerCount = level->BreakerCount + 1;
        level->BreakerSpawnTime = 1023;
        Level_SetNewSpawnPosition(level);
        level->SafeSpawn = 0;
      }
    }
    short PadDifference = level->TargetPadPosition - level->PadPosition;
    level->PadPosition += PadDifference / 3;

    unsigned char toRemove = 16;

    if (level->BreakerCount > 0) {
      for (unsigned char i = 0; i < level->BreakerCount; i++) {
        if (Level_TickBall(level->Breakers[i], level)) {
          toRemove = i;
        }
      }
      if (toRemove != 16) {
        Level_RemoveBreaker(level, toRemove);
      }
    } else {
      if (level->SafeSpawn == 0) {
        return 1;
      }
    }
    if (level->EnabledBlocks == 0x0) {
      return 2;
    }
  }
  return 0;
}

void Level_DrawLevel(LevelRuntimeData* level, u32* clrPalette) {

  u32 newBreakerColor = clrPalette[level->BreakerSpawnTime / 4];

  if (level->BreakerCount != level->TargetBreakerCount) {
    C2D_DrawRectangle(level->BreakerSpawnPos, (SCREEN_HEIGHT/2), 0, BREAKER_SIDE, BREAKER_SIDE, newBreakerColor, newBreakerColor, newBreakerColor, newBreakerColor);
  }

  u32 padColor = clrPalette[level->Difficulty];

  if (level->BreakerCount > 0) {
    for (unsigned char i = 0; i < level->BreakerCount; i++) {
      Breaker_DrawBreaker(level->Breakers[i], padColor);
    }
  }

  C2D_DrawRectangle(level->PadPosition, SCREEN_HEIGHT - PAD_HEIGHT, 0, level->PadLength, PAD_HEIGHT, padColor, padColor, padColor, padColor);

  for (unsigned char x = 0; x < BLOCK_HORIZONTAL; x++) {
    for (unsigned char y = 0; y < BLOCK_VERTICAL; y++) {
      if (level->BlockStates[x][y] > 0x0) {
        short xPos = BLOCK_START_H + (x * (BLOCK_WIDTH + BLOCK_PADDING));
        short yPos = BLOCK_START_V + (y * (BLOCK_HEIGHT + BLOCK_PADDING));
        u32 thisColor = clrPalette[level->BlockStates[x][y]];
        C2D_DrawRectangle(xPos, yPos, 0, BLOCK_WIDTH, BLOCK_HEIGHT, thisColor, thisColor, thisColor, thisColor);
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
    level->TargetPadPosition = interpolation - (level->PadLength / 2);
    rand();
  }
}

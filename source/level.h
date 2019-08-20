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
#include <stdlib.h>
#include <math.h>

#define PAD_LENGTH 32
#define PAD_HEIGHT 6

#define BLOCK_WIDTH (36)
#define BLOCK_HEIGHT (BLOCK_WIDTH / 3)
#define BLOCK_PADDING (3)
#define BLOCK_VERTICAL (6)
#define BLOCK_HORIZONTAL (8)
#define BLOCK_SIZE_HORIZONTAL (((BLOCK_WIDTH + BLOCK_PADDING) * BLOCK_HORIZONTAL) - BLOCK_PADDING)
#define BLOCK_START_H ((SCREEN_WIDTH / 2) - (BLOCK_SIZE_HORIZONTAL / 2))
#define BLOCK_START_V (12)
#define SPAWN_AREA_WIDTH ((SCREEN_WIDTH * 3) / 4)
#define SPAWN_AREA_SEGMENTS (SPAWN_AREA_WIDTH / 3)
#define SPAWN_AREA_BEGIN ((SCREEN_WIDTH/2) - (SPAWN_AREA_WIDTH/2))

typedef struct LevelRuntimeData {
  BreakerBall* Breakers[15];
  unsigned char BreakerCount;
  unsigned char TargetBreakerCount;
  short TargetPadPosition;
  short PadPosition;
  unsigned short PadLength;
  unsigned short BreakerSpawnTime;
  unsigned short BreakerSpawnPos;
  unsigned char EnabledBlocks;
  unsigned char Difficulty;
  unsigned char SafeSpawn;
  unsigned char PerfectScore;
  unsigned long Multiplier;
  unsigned long Score;
  unsigned char BlockStates [BLOCK_HORIZONTAL][BLOCK_VERTICAL];
} LevelRuntimeData;

LevelRuntimeData* Level_CreateNew (unsigned char difficulty, unsigned long multiplier);

void Level_RemoveBreaker (LevelRuntimeData* level, unsigned char toRemove);
void Level_SetNewSpawnPosition (LevelRuntimeData* level);
unsigned char Level_VerifyBallBlockCollision(BreakerBall* target, unsigned char X, unsigned char Y);
unsigned char Level_TickBall(BreakerBall* target, LevelRuntimeData* level);
unsigned char Level_TickLevel (LevelRuntimeData* level);
void Level_DrawLevel(LevelRuntimeData* level, u32* clrPalette);
void Level_HandleInput(LevelRuntimeData* level);

#endif

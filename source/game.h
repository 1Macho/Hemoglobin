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

#ifndef GAME
#define GAME

#include <stdlib.h>
#include <citro2d.h>
#include "level.h"
#include "color.h"

#define GAME_SATURATION 0xA8
#define GAME_VALUE 0xB0

typedef struct GameRuntimeData {
  LevelRuntimeData* CurrentLevel;
  unsigned char CurrentDifficulty;
  u32 ClrClear;
  u32* ClrPalette;
  C3D_RenderTarget* TopLeft;
  C3D_RenderTarget* TopRight;
  C3D_RenderTarget* Bottom;
  unsigned long Score;
  C2D_Font Font;
  C2D_TextBuf ScoreTextBuf;
  C2D_TextBuf MultiplierTextBuf;
  C2D_TextBuf LevelTextBuf;
} GameRuntimeData;

GameRuntimeData* Game_Initialize(C3D_RenderTarget* topLeft, C3D_RenderTarget* topRight, C3D_RenderTarget* bottom);

unsigned char Game_Update (GameRuntimeData* game);

#endif

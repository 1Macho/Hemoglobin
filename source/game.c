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

#include "game.h"

GameRuntimeData* Game_Initialize(C3D_RenderTarget* topLeft, C3D_RenderTarget* topRight, C3D_RenderTarget* bottom) {
  GameRuntimeData* result = malloc(sizeof(GameRuntimeData));
  result->topLeft = topLeft;
  result->topRight = topRight;
  result->bottom = bottom;
  result->clrPalette = Color_GeneratePalete(GAME_SATURATION, GAME_VALUE);
  result->clrClear = C2D_Color32(0x23, 0x23, 0x23, 0xFF);
  result->currentDifficulty = 1;
  result->currentLevel = Level_CreateNew(result->currentDifficulty);
  return result;
}

unsigned char Game_Update (GameRuntimeData* game) {
  Level_HandleInput(game->currentLevel);
  unsigned char levelTickResult = Level_TickLevel(game->currentLevel);
  if (levelTickResult == 0x1) {
    return 1;
  }
  if (levelTickResult == 0x2) {
    game->currentDifficulty = game->currentDifficulty + 1;
    game->currentLevel = Level_CreateNew(game->currentDifficulty);
  }
  C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
  C2D_TargetClear(game->topLeft, game->clrClear);
  C2D_SceneBegin(game->topLeft);
  Level_DrawLevel(game->currentLevel, game->clrPalette);
  C2D_TargetClear(game->bottom, game->clrClear);
  C2D_SceneBegin(game->bottom);
  C3D_FrameEnd(0);
  return 0;
}

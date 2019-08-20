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
  result->TopLeft = topLeft;
  result->TopRight = topRight;
  result->Bottom = bottom;
  result->ClrPalette = Color_GeneratePalete(GAME_SATURATION, GAME_VALUE);
  result->ClrClear = C2D_Color32(0x23, 0x23, 0x23, 0xFF);
  result->CurrentDifficulty = 1;
  result->CurrentLevel = Level_CreateNew(result->CurrentDifficulty, 0);
  result->Score = 0;
  result->Font = C2D_FontLoad("romfs:/fonts/ui.bcfnt");
  result->ScoreTextBuf = C2D_TextBufNew(4096);
  result->MultiplierTextBuf = C2D_TextBufNew(4096);
  result->LevelTextBuf = C2D_TextBufNew(4096);
  return result;
}

unsigned char Game_Update (GameRuntimeData* game) {
  Level_HandleInput(game->CurrentLevel);
  unsigned char levelTickResult = Level_TickLevel(game->CurrentLevel);
  if (levelTickResult == 0x1) {
    return 1;
  }
  if (levelTickResult == 0x2) {
    unsigned long newMultiplier = 0;
    if (game->CurrentLevel->PerfectScore) {
      newMultiplier = game->CurrentLevel->Multiplier;
    }
    game->Score = game->Score + game->CurrentLevel->Score;
    game->CurrentDifficulty = game->CurrentDifficulty + 1;
    game->CurrentLevel = Level_CreateNew(game->CurrentDifficulty, newMultiplier);
  }
  C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
  C2D_TargetClear(game->TopLeft, game->ClrClear);
  C2D_SceneBegin(game->TopLeft);
  Level_DrawLevel(game->CurrentLevel, game->ClrPalette);
  C2D_TargetClear(game->Bottom, game->ClrClear);
  C2D_SceneBegin(game->Bottom);
  unsigned long fullScore = game->Score + game->CurrentLevel->Score;
  unsigned long level = game->CurrentDifficulty;
  unsigned long multiplier = game->CurrentLevel->Multiplier;
  C2D_TextBufClear(game->LevelTextBuf);
  char buflvl[60];
  C2D_Text LevelText;
  snprintf(buflvl, sizeof(buflvl), "Schicht %lX", level);
  C2D_TextFontParse(&LevelText, game->Font, game->LevelTextBuf, buflvl);
  C2D_TextOptimize(&LevelText);
	C2D_DrawText(&LevelText, C2D_AtBaseline | C2D_WithColor, 8.0f, 20.0f, 0.0f, 0.5f, 0.5f, C2D_Color32f(0.81f,0.81f,0.81f,1.0f));
  C2D_TextBufClear(game->ScoreTextBuf);
  char bufscr[60];
  C2D_Text ScoreText;
  snprintf(bufscr, sizeof(bufscr), "%lu", fullScore);
  C2D_TextFontParse(&ScoreText, game->Font, game->ScoreTextBuf, bufscr);
  C2D_TextOptimize(&ScoreText);
	C2D_DrawText(&ScoreText, C2D_AtBaseline | C2D_WithColor, 8.0f, 40.0f, 0.0f, 0.5f, 0.5f, C2D_Color32f(0.81f,0.81f,0.81f,1.0f));
  C2D_TextBufClear(game->MultiplierTextBuf);
  char bufmul[60];
  C2D_Text MultiplierText;
  snprintf(bufmul, sizeof(bufmul), "x%lu", multiplier);
  C2D_TextFontParse(&MultiplierText, game->Font, game->ScoreTextBuf, bufmul);
  C2D_TextOptimize(&MultiplierText);
	C2D_DrawText(&MultiplierText, C2D_AtBaseline | C2D_WithColor, 8.0f, 60.0f, 0.0f, 0.5f, 0.5f, C2D_Color32f(0.81f,0.81f,0.81f,1.0f));
  C3D_FrameEnd(0);
  return 0;
}

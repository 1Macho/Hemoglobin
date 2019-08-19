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

// Clear block width = 36
// Clear blocks height = 12
// Padding between blocks = 3
// Total blocks vertically = 6

// Total blocks horizontally = 8

#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "breaker.h"
#include "level.h"
#include "autoplay.h"
#include "color.h"

int main(int argc, char* argv[]) {

  // Init libs
  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  C2D_Prepare();
  consoleInit(GFX_BOTTOM, NULL);
  // Create screens
  C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

  Result rc = romfsInit();
  u32* clrPalette = Color_GeneratePalete(0xA8, 0xB0);
  u32 clrClear = C2D_Color32(0x23, 0x23, 0x23, 0x68);

  //C2D_Font testFont = C2D_FontLoad("romfs:/fonts/test.bcfnt");
  //C2D_TextBuf testTextBuf = C2D_TextBufNew(4096);
  //C2D_Text testText;
  //C2D_TextFontParse(&testText, testFont, testTextBuf, "Farbtönßtudios");
  //C2D_TextOptimize(&testText);

  unsigned char currentDifficulty = 5;

  LevelRuntimeData* testData = Level_CreateNew(currentDifficulty);


  // Main loop
  while (aptMainLoop())
  {
    hidScanInput();

		Level_HandleInput(testData);

    // Respond to user input
    u32 kDown = hidKeysDown();
    if (kDown & KEY_START)
      break; // break in order to return to hbmenu
    printf("\x1b[1;1HAttempt at simple 3ds pong game.");
    printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
    printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
    printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
    printf("\x1b[7;1HLevel:  %3d", testData->Difficulty);

    //Autoplay_FakeInput(testData);

    //for (unsigned char i = 0; i < 5; i++) {
      unsigned char levelTickResult = Level_TickLevel(testData);
      if (levelTickResult == 0x1) {
        break;
      }
      if (levelTickResult == 0x2) {
        currentDifficulty = currentDifficulty + 1;
        testData = Level_CreateNew(currentDifficulty);
      }
    //}

    // Render the scene
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, clrClear);
    C2D_SceneBegin(top);
    Level_DrawLevel(testData, clrPalette);
    //C2D_DrawText(&testText, C2D_AtBaseline | C2D_WithColor, 50.0f, 50.0f, 0.0f, 0.25f, 0.25f, C2D_Color32f(0.81f,0.81f,0.81f,1.0f));

    C3D_FrameEnd(0);
  }
  //C2D_FontFree(testFont);
  //C2D_TextBufDelete(testTextBuf);
  // Deinit libs
  C2D_Fini();
  C3D_Fini();
  gfxExit();
  return 0;
}

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
// Total blocks horizontally = 11

#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "breaker.h"
#include "level.h"

#define TOUCH_WIDTH 310
#define TOUCH_PADDING 5

int main(int argc, char* argv[]) {

  // Init libs
  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  C2D_Prepare();
  consoleInit(GFX_BOTTOM, NULL);

  // Create screens
  C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);


  u32 clrRec = C2D_Color32(0xAB, 0x3F, 0x3F, 0xFF);
  u32 clrClear = C2D_Color32(0x23, 0x23, 0x23, 0x68);

  Point testBallPosition;
  testBallPosition.X = SCREEN_WIDTH / 2;
  testBallPosition.Y = SCREEN_HEIGHT / 2;
  Point testBallVelocity;
  testBallVelocity.X = 1;
  testBallVelocity.Y = 1;
  BreakerBall testBall;
  testBall.Position = &testBallPosition;
  testBall.Velocity = &testBallVelocity;
  testBall.Saveable = 1;
  LevelRuntimeData testData;
  testData.Breakers[0] = &testBall;
  testData.BreakerCount = 1;
  testData.PadPosition = (SCREEN_WIDTH / 2) - (PAD_LENGTH / 2);


  // Main loop
  while (aptMainLoop())
  {
    hidScanInput();
    touchPosition touch;
    hidTouchRead(&touch);

    if (touch.px >= TOUCH_PADDING & touch.px <= TOUCH_WIDTH - TOUCH_PADDING) {
      short interpolation_id = touch.px - TOUCH_PADDING;
      long interpolation = interpolation_id * SCREEN_WIDTH / (TOUCH_WIDTH - TOUCH_PADDING * 2);
      testData.TargetPadPosition = interpolation - (PAD_LENGTH / 2);
    }

    // Respond to user input
    u32 kDown = hidKeysDown();
    if (kDown & KEY_START)
      break; // break in order to return to hbmenu
    printf("\x1b[1;1HAttempt at simple 3ds pong game.");
    printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
    printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
    printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);

    if (Level_TickLevel(&testData)) {
      break;
    }

    // Render the scene
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, clrClear);
    C2D_SceneBegin(top);

    Level_DrawLevel(&testData, clrRec, clrRec);

    C3D_FrameEnd(0);
  }

  // Deinit libs
  C2D_Fini();
  C3D_Fini();
  gfxExit();
  return 0;
}

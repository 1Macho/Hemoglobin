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
#include "game.h"

int main(int argc, char* argv[]) {

  // Init libs
  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  C2D_Prepare();
  //consoleInit(GFX_BOTTOM, NULL);
  // Create screens
  C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
  C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

  Result rc = romfsInit();

  GameRuntimeData* game = Game_Initialize(top, 0, bottom);


  // Main loop
  while (aptMainLoop())
  {
    hidScanInput();

    // Respond to user input
    u32 kDown = hidKeysDown();
    if (kDown & KEY_START)
      break; // break in order to return to hbmenu

    if(Game_Update(game)) {
      break;
    }
  }
  // Deinit libs
  C2D_Fini();
  C3D_Fini();
  gfxExit();
  return 0;
}

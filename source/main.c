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


// Simple citro2d untextured shape example
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

#define TOUCH_WIDTH 310
#define TOUCH_PADDING 5
#define BREAKER_SIDE 8
#define PAD_LENGTH 32
#define PAD_HEIGHT 6

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Init libs
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);


	u32 clrRec = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
	u32 clrClear = C2D_Color32(0x00, 0x00, 0x00, 0x68);

	short posX = SCREEN_WIDTH / 2;
	short posY = SCREEN_HEIGHT / 2;
	short vX = 1;
	short vY = 1;
	short padPosition = (SCREEN_WIDTH / 2) - (PAD_LENGTH / 2);

	short saveable = 1;


	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();
		touchPosition touch;
		hidTouchRead(&touch);

		if (touch.px >= TOUCH_PADDING & touch.px <= TOUCH_WIDTH - TOUCH_PADDING) {
			short interpolation_id = touch.px - TOUCH_PADDING;
			long interpolation = interpolation_id * SCREEN_WIDTH / (TOUCH_WIDTH - TOUCH_PADDING * 2);
			padPosition = interpolation - (PAD_LENGTH / 2);
		}

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
		printf("\x1b[1;1HAttempt at simple 3ds pong game.");
		printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
		printf("\x1b[5;1HPosX:  %d%%\x1b[K", posX);
		printf("\x1b[6;1HPosY:  %d%%\x1b[K", posY);
		printf("\x1b[7;1HVX:  %d%%\x1b[K", vX);
		printf("\x1b[8;1HVY:  %d%%\x1b[K", vY);
		printf("\x1b[9;1HTouch:  %03d; %03d", touch.px, touch.py);

				// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);

		// Bouncy rectangle
		posX += vX;
		posY += vY;
		if (posX + BREAKER_SIDE > SCREEN_WIDTH) {
			vX = 0 - vX;
			posX = SCREEN_WIDTH - BREAKER_SIDE;
		}
		if (posX < 0) {
			vX = 0 - vX;
			posX = 0;
		}
		if (posY + BREAKER_SIDE > SCREEN_HEIGHT - PAD_HEIGHT) {
			if (saveable) {
				if (((posX > padPosition) & (posX < padPosition + PAD_LENGTH)) | ((posX + BREAKER_SIDE > padPosition) & (posX + BREAKER_SIDE < padPosition + PAD_LENGTH))) {
					vY = 0 - vY;
					posY = SCREEN_HEIGHT - PAD_HEIGHT - BREAKER_SIDE - 1;
				}
				else {
					saveable = 0;
				}
			}
		}
		if (posY + BREAKER_SIDE > SCREEN_HEIGHT) {
			//vY = 0 - vY;
			//posY = SCREEN_HEIGHT - BREAKER_SIDE;
			break;
		}
		if (posY < 0) {
			vY = 0 - vY;
			posY = 0;
		}

		C2D_DrawRectangle(posX, posY, 0, BREAKER_SIDE, BREAKER_SIDE, clrRec, clrRec, clrRec, clrRec);

		//Draw bottom pad
		C2D_DrawRectangle(padPosition, SCREEN_HEIGHT - PAD_HEIGHT, 0, PAD_LENGTH, PAD_HEIGHT, clrRec, clrRec, clrRec, clrRec);

		C3D_FrameEnd(0);
	}

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}

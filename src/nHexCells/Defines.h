#ifndef DEFINES_H
#define DEFINES_H

extern "C"
{
#ifdef _TINSPIRE
#include <os.h>
#endif
#include "SDL/SDL.h"
}

#ifndef _TINSPIRE
#include <cassert>
#endif

#ifndef _TINSPIRE
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#endif

#ifndef DEBUG_MSG
#define DEBUG_MSG//     printf
#endif

#define GAME_BACKGROUND_R       (153)
#define GAME_BACKGROUND_G       (153)
#define GAME_BACKGROUND_B       (102)

#define MESSAGE_TEXT_R           (107)
#define MESSAGE_TEXT_G           (107)
#define MESSAGE_TEXT_B           (71)

#define MESSAGE_BACKGROUND_R     (255)
#define MESSAGE_BACKGROUND_G     (255)
#define MESSAGE_BACKGROUND_B     (207)

int min(int a, int b);
int max(int a, int b);

void draw_rectangle(SDL_Surface* Surface, Uint32 color, Uint16 x, Uint16 y, Uint16 width, Uint16 height, Uint8 lnpx);

Uint16 get_pixel16(SDL_Surface *surface, int x, int y);
void put_pixel16(SDL_Surface *surface, int x, int y, Uint16 pixel);

#ifndef ASSERT
#ifdef _TINSPIRE
#define ASSERT
#else
#define ASSERT assert
#endif
#endif

#endif

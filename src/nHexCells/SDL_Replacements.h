#ifndef SDL_REPLACEMENTS_H
#define SDL_REPLACEMENTS_H

#ifdef _TINSPIRE
#define FONT      nSDL_Font
#define LOADFONT  nSDL_LoadFont
#define FREEFONT  nSDL_FreeFont
#define DRAWTEXT  nSDL_DrawString
#else
#include "SDL/SDL.h"
#include "SDL_ttf.h"
#define FONT      TTF_Font
#define LOADFONT  TTF_OpenFont
#define FREEFONT  TTF_CloseFont
#define DRAWTEXT  SDLDrawText
#endif

void SDLDrawText(SDL_Surface* pSurface, FONT* pFont, int x, int y, char* pstr );

#endif

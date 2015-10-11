#include "SDL_Replacements.h"

void SDLDrawText(SDL_Surface* pSurface, FONT* pFont, int x, int y, char* pstr )
{
   SDL_Surface* pMessage = TTF_RenderText_Solid(pFont, pstr, { 0, 0, 0 });
   SDL_Rect offset;
   offset.x = x;
   offset.y = y;
   SDL_BlitSurface(pMessage, NULL, pSurface, &offset);
}


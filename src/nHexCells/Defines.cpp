#include "Defines.h"

int Puz_Min(int a, int b) { return a<b ? a : b; }
int Puz_Max(int a, int b) { return a>b ? a : b; }

Uint16 get_pixel16(SDL_Surface *surface, int x, int y)
{
   int bpp = surface->format->BytesPerPixel;
   Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp; //Get the requested pixel 
   return *(Uint16 *)p;
}

void put_pixel16(SDL_Surface *surface, int x, int y, Uint16 pixel)
{
   int bpp = surface->format->BytesPerPixel;
   Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
   *(Uint16 *)p = pixel;
}

void draw_rectangle(SDL_Surface* Surface, Uint32 color, Uint16 x, Uint16 y, Uint16 width, Uint16 height, Uint8 lnpx)
{
   SDL_Rect DestRect;

   // Draw the top line
   DestRect.x = x;
   DestRect.y = y;
   DestRect.w = width;
   DestRect.h = 1;
   SDL_FillRect(Surface, &DestRect, color);

   // Draw the bottum line
   DestRect.y = y+height-1;
   SDL_FillRect(Surface, &DestRect, color);

   // Draw the left line
   DestRect.y = y;
   DestRect.w = 1;
   DestRect.h = height;
   SDL_FillRect(Surface, &DestRect, color);

   // Draw the left line
   DestRect.x = x+width-1;
   SDL_FillRect(Surface, &DestRect, color);
}

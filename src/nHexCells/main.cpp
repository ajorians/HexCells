extern "C"
{
#ifdef _TINSPIRE
#include <os.h>
#endif

//#include "ArchiveLib/ArchiveLib.h"
}
#include "SDL/SDL.h"

#include "Defines.h"

#include "Menu.h"
//#include "Achievements.h"
//#include "Game.h"
//#include "CardImages.h"
//#include "Config.h"
//#include "Options.h"
//#include "AchieveConfig.h"
//#include "Help.h"

#if _MSC_VER >= 1400
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void)
{
   return _iob;
}
#endif

#define SCREEN_BIT_DEPTH        (16)
#ifdef _TINSPIRE
#define SCREEN_VIDEO_MODE (SDL_SWSURFACE|SDL_FULLSCREEN|SDL_HWPALETTE)
#else
#define SCREEN_VIDEO_MODE (SDL_SWSURFACE|SDL_HWPALETTE)
#endif

int main(int argc, char *argv[])
{
   printf("Initializing SDL.\n");

   /* Initialize the SDL library (starts the event loop) */
   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      fprintf(stderr,
              "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
   }

#ifndef _TINSPIRE
   if( TTF_Init() == -1 ) { 
      return false;
   }
#endif

   printf("SDL initialized.\n");

   SDL_Surface* pScreen = NULL;
   pScreen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BIT_DEPTH, SCREEN_VIDEO_MODE);

   /*ArchiveSetCurrentDirectory(argv[0]);
   Config config;
   AchieveConfig ac(&config);*/

   if (pScreen == NULL)
   {
      fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BIT_DEPTH, SDL_GetError());
      exit(1);
   }
   else
   {
      /* Hides the cursor */
      SDL_ShowCursor(SDL_DISABLE);

      /*CardImages cardImages;*/

      while (true)
      {
         bool bShowHelp = false, bShowOptions = false, bShowAchievements = false;
         SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

         if (argc != 2)
         {
            MainMenu menu(pScreen, NULL, NULL);// &config, &ac);
            while (menu.Loop()) {}
            if (menu.ShouldQuit())
               break;
            bShowHelp = menu.ShouldShowHelp();
            bShowOptions = menu.ShowShowOptions();
            bShowAchievements = menu.ShouldShowAchievements();
         }

         /*if (bShowOptions) {
            Options ops(pScreen, &config);
            while (ops.Loop()) {}
            continue;
         }
         else if (bShowAchievements) {
            Achievements ach(pScreen, &ac);
            while (ach.Loop()) {}
            continue;
         }
         else if (bShowHelp)
         {
            HeartsHelp help(pScreen);
            while (help.Loop()) {}
            continue;
         }
         else
         {
            bool bPlay = true;
            while (bPlay) {
               Game game(pScreen, &config, &ac, &cardImages);
               while (game.Loop()) {}

               bPlay = false;
               break;
            }
         }*/
      }
   }

   printf("Quitting SDL.\n");

   /* Shutdown all subsystems */
   SDL_Quit();

   printf("Quitting...\n");

   return 0;
}

#ifndef MENU_H
#define MENU_H

extern "C"
{
#ifdef _TINSPIRE
#include <os.h>
#endif
#include "SDL/SDL.h"
}

#include "SDL_Replacements.h"

//#include "Background.h"
//#include "Config.h"

class Config;
class AchieveConfig;

class MainMenu
{
public:
   MainMenu(SDL_Surface* pScreen, Config* pConfig, AchieveConfig* pAchieveConfig);
   ~MainMenu();

   bool Loop();

   bool ShouldQuit() const;
   bool ShowShowOptions() const;
   bool ShouldShowHelp() const;
   bool ShouldShowAchievements() const;

protected:
   bool PollEvents();
   void UpdateDisplay();

protected:
   SDL_Surface	*m_pScreen;//Does not own
#ifdef _TINSPIRE
   Background	m_Background;
   SDL_Surface	*m_pTitleGraphic;
   SDL_Surface	*m_pPlayGraphic;
   SDL_Surface	*m_pOptionsGraphic;
#endif
   FONT 	*m_pFont;
   enum MenuChoice { Play, Options, Help, Achieve, Quit };
   MenuChoice 	m_eChoice;
   Config		*m_pConfig;
   AchieveConfig	*m_pAchieve;
};

#endif

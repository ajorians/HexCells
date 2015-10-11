#ifndef GAME_H
#define GAME_H

extern "C"
{
#ifdef _TINSPIRE
#include <os.h>
#endif
#include "SDL/SDL.h"
#include "HexCellsLib/HexCellsLib.h"
}

#include "Defines.h"
//#include "CardImages.h"
#include "Metrics.h"
//#include "Piece.h"
//#include "Message.h"
//#include "Direction.h"
#include "SDL_Replacements.h"

class Config;
class AchieveConfig;

class Game
{
public:
   Game(SDL_Surface* pScreen, Config* pConfig, AchieveConfig* pArchieve);
   ~Game();

   bool Loop();

protected:
   bool PollEvents();
   void UpdateDisplay();
   /*void Move(Direction eDirection);
   void SelectCard();
   void DoGamePlay();
   void RemovedSelectedPieces();
   void RebuildPieces();
   void ConstructAIs();*/

protected:
   void DrawCell(int nX, int nY);
   void DrawBorder(int nX, int nY);

protected:
   SDL_Surface	*m_pScreen;//Does not own
   Config		*m_pConfig;//Does not own
   AchieveConfig	*m_pAchieve;
   FONT *m_pFont;
   HexCellsLib m_HexCells;
   Metrics		m_Metrics;
   //Message		m_ShotMoonMessage;*/
};

#endif

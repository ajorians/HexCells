#include "Game.h"
//#include "ScoreReview.h"
#include "Config.h"
//#include "AchieveConfig.h"

#ifdef _TINSPIRE
extern "C"
{
#include "SDL/SDL_gfxPrimitives.h"
}
#else
#include "SDL_gfxPrimitives.h"
#endif

//X Y Type Revealed Value(-1 if unknown) Details Orientation(indicators-only)
#define NEW_GAME_DATA   "Hexcells 1 0 8 8 5 \
0 3 0 0 1 0 64 \
1 4 2 0 1 0 \
2 3 1 0 -1 1 \
2 5 2 0 2 0 \
3 2 2 0 2 0 \
3 4 2 0 2 0 \
3 6 1 0 -1 1 \
4 1 1 0 -1 1 \
4 3 2 0 2 0 \
4 5 2 1 3 3 \
4 7 2 1 2 3 \
5 0 0 0 2 0 32 \
5 2 2 0 2 0 \
5 4 1 0 -1 1 \
5 6 1 0 -1 1 \
6 3 2 0 1 0 \
6 5 2 0 2 0 \
7 4 2 1 0 0"

Game::Game(SDL_Surface* pScreen, Config* pConfig, AchieveConfig* pAchieve)
   : m_pScreen(pScreen), m_pConfig(pConfig), m_pAchieve(pAchieve), m_nCurrentX(0), m_nCurrentY(0)
{
   HexCellsLibCreate(&m_HexCells, NEW_GAME_DATA);

#ifdef _TINSPIRE
   m_pFont = LOADFONT(NSDL_FONT_THIN, 0/*R*/, 0/*G*/, 0/*B*/);
#else
   m_pFont = LOADFONT("small_font.ttf", 10);
#endif

   m_Metrics.SetBoardDimensions(HexCellsGetWidth(m_HexCells), HexCellsGetHeight(m_HexCells));
}

Game::~Game()
{
   FREEFONT(m_pFont);
   HexCellsLibFree(&m_HexCells);
}

bool Game::Loop()
{
   //Handle keypresses
   if (PollEvents() == false)
      return false;

   //Update screen
   UpdateDisplay();

   SDL_Delay(30);
   /*if (GetHeartsGameOver(m_Hearts) == 1) {
      m_pAchieve->JustPlayedAGame();
      return false;
   }*/

   return true;
}

bool Game::PollEvents()
{
   SDL_Event event;

   /* Poll for events. SDL_PollEvent() returns 0 when there are no  */
   /* more events on the event queue, our while loop will exit when */
   /* that occurs.                                                  */
   while (SDL_PollEvent(&event))
   {
      /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
      switch (event.type)
      {
      case SDL_QUIT:
      return false;
      break;

      case SDL_KEYDOWN:
      switch (event.key.keysym.sym)
      {
      case SDLK_ESCAPE:
      fprintf(stderr, "Hit Escape!n");
      return false;
      break;

      case SDLK_RETURN:
      case SDLK_LCTRL:
      case SDLK_RCTRL:
      DetermineBomb();
      break;

      case SDLK_RIGHT:
      case SDLK_6:
      Move(Right);
      break;

      case SDLK_LEFT:
      case SDLK_4:
      Move(Left);
      break;

      case SDLK_UP:
      case SDLK_8:
      Move(Up);
      break;

      case SDLK_DOWN:
      case SDLK_2:
      Move(Down);
      break;

      case SDLK_CLEAR:
      case SDLK_BACKSPACE:
      DetermineClear();
      break;

      case SDLK_s:
      SimpleSolveStep();
      break;

      default:
      break;
      }
      break;

      //Called when the mouse moves
      case SDL_MOUSEMOTION:
      break;

      case SDL_KEYUP:
      break;

      default:
      break;
      }
   }
   return true;
}

void Game::UpdateDisplay()
{
   //Draw background
   SDL_FillRect(m_pScreen, NULL, SDL_MapRGB(m_pScreen->format, GAME_BACKGROUND_R, GAME_BACKGROUND_G, GAME_BACKGROUND_B));

   int nWidth = HexCellsGetWidth(m_HexCells);
   int nHeight = HexCellsGetHeight(m_HexCells);

   for(int x=0; x<nWidth; x++) {
      for(int y=0; y<nHeight; y++) {
         DrawCell(x, y);

         if( m_nCurrentX == x && m_nCurrentY == y )
            DrawSelector(x, y);
      }
   }

   SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Game::DrawCell(int nX, int nY)
{
   SpotType eType;
   HexCellGetSpotType(m_HexCells, nX, nY, &eType);
   if (eType == Nothing)
      return;

   if (eType == Indicator) {
      char buffer[8];
      IndicatorOrientation eOrientation;
      int nValue;
      ValueDetails eDetails;
      HexCellsGetIndicatorDirection(m_HexCells, nX, nY, &eOrientation);
      HexCellsGetIndicatorValue(m_HexCells, nX, nY, eOrientation, &nValue, &eDetails);

#ifdef _TINSPIRE
#else
      _itoa(nValue, buffer, 10);
#endif
      //TODO: Check if continous/flags
      DRAWTEXT(m_pScreen, m_pFont, m_Metrics.GetLeftForPiece(nX, nY) + m_Metrics.GetPieceSize()/2, m_Metrics.GetTopForPiece(nX, nY) + m_Metrics.GetPieceSize()/2, buffer);
   }
   else if (eType == Bomb || eType == NotBomb) {
      DrawBorder(nX, nY);
      ASSERT(HEXCELLS_IS_REVEALED == HexCellsIsRevealedSpot(m_HexCells, nX, nY));
      if (eType == Bomb) {
         DRAWTEXT(m_pScreen, m_pFont, m_Metrics.GetLeftForPiece(nX, nY) + m_Metrics.GetPieceSize()/2, m_Metrics.GetTopForPiece(nX, nY) + m_Metrics.GetPieceSize()/2, "B");
      }
      else {
         char buffer[8];
         int nValue;
         ValueDetails eDetails;
         HexCellsGetRevealedSpotValue(m_HexCells, nX, nY, &nValue, &eDetails);

#ifdef _TINSPIRE
#else
         _itoa(nValue, buffer, 10);
#endif

         DRAWTEXT(m_pScreen, m_pFont, m_Metrics.GetLeftForPiece(nX, nY) + m_Metrics.GetPieceSize()/2, m_Metrics.GetTopForPiece(nX, nY) + m_Metrics.GetPieceSize()/2, buffer);
      }
   }
   else if (eType == Unknown) {
      DrawBorder(nX, nY);
      //DRAWTEXT(m_pScreen, m_pFont, m_Metrics.GetLeftForPiece(nX, nY) + m_Metrics.GetPieceSize()/2, m_Metrics.GetTopForPiece(nX, nY) + m_Metrics.GetPieceSize()/2, "U");
   }
   else {
      ASSERT(0);
   }
}

void Game::DrawBorder(int nX, int nY)
{
   short nLeft = m_Metrics.GetLeftForPiece(nX, nY);
   short nTop = m_Metrics.GetTopForPiece(nX, nY);
   short nPieceSize = m_Metrics.GetPieceSize();

   {
   short s[] = { nLeft,                nLeft + nPieceSize/4,   nLeft + 3*nPieceSize/4, nLeft+nPieceSize,    nLeft+3*nPieceSize/4,   nLeft + nPieceSize/4 };
   short t[] = { nTop + nPieceSize/2,  nTop,                   nTop,                   nTop + nPieceSize/2, nTop + nPieceSize,      nTop + nPieceSize    };

   filledPolygonRGBA(m_pScreen,
                     s, t,
                     6,
                     255, 0, 0, 255);
   }

   {
      const short nSmallAmount = Metrics::GetGapAmount();
      short s[] = { nLeft + nSmallAmount, nLeft + nPieceSize/4,   nLeft + 3*nPieceSize/4, nLeft+nPieceSize - nSmallAmount,    nLeft+3*nPieceSize/4,               nLeft + nPieceSize/4 };
      short t[] = { nTop + nPieceSize/2,  nTop + 4*nSmallAmount/2,    nTop + 4*nSmallAmount/2,    nTop + nPieceSize/2,                nTop + nPieceSize - 4*nSmallAmount/2,   nTop + nPieceSize - 4*nSmallAmount/2 };

      filledPolygonRGBA(m_pScreen,
                        s, t,
                        6,
                        255, 255, 0, 255);
   }
}

void Game::DrawSelector(int nX, int nY)
{
   SpotType eType;
   HexCellGetSpotType(m_HexCells, nX, nY, &eType);
   if( eType == Nothing )
      return;

   short nLeft = m_Metrics.GetLeftForPiece(nX, nY);
   short nTop = m_Metrics.GetTopForPiece(nX, nY);
   short nPieceSize = m_Metrics.GetPieceSize();

   {
      short s[] = { nLeft,                nLeft + nPieceSize/4,   nLeft + 3*nPieceSize/4, nLeft+nPieceSize,    nLeft+3*nPieceSize/4,   nLeft + nPieceSize/4 };
      short t[] = { nTop + nPieceSize/2,  nTop,                   nTop,                   nTop + nPieceSize/2, nTop + nPieceSize,      nTop + nPieceSize };

      filledPolygonRGBA(m_pScreen,
                        s, t,
                        6,
                        0, 255, 255, 255);
   }
}

void Game::Move(Direction eDirection)
{
   if( eDirection == Down && m_nCurrentY < (HexCellsGetHeight(m_HexCells) -1) )
      m_nCurrentY++;
   else if( eDirection == Left && m_nCurrentX > 0 )
      m_nCurrentX--;
   else if( eDirection == Right && m_nCurrentX < (HexCellsGetWidth(m_HexCells)-1) )
      m_nCurrentX++;
   else if( eDirection == Up && m_nCurrentY > 0 )
      m_nCurrentY--;
}

void Game::DetermineBomb()
{
   SpotType eType;
   HexCellGetSpotType(m_HexCells, m_nCurrentX, m_nCurrentY, &eType);
   if( eType != Unknown )
      return;
   if( HEXCELLS_NOT_REVEALED != HexCellsIsRevealedSpot(m_HexCells, m_nCurrentX, m_nCurrentY) )
      return;

   HexCellsRevealAs(m_HexCells, m_nCurrentX, m_nCurrentY, HEXCELLS_REVEAL_BOMB);
}

void Game::DetermineClear()
{
   SpotType eType;
   HexCellGetSpotType(m_HexCells, m_nCurrentX, m_nCurrentY, &eType);
   if (eType != Unknown)
      return;
   if (HEXCELLS_NOT_REVEALED != HexCellsIsRevealedSpot(m_HexCells, m_nCurrentX, m_nCurrentY))
      return;

   HexCellsRevealAs(m_HexCells, m_nCurrentX, m_nCurrentY, HEXCELLS_REVEAL_NOT_BOMB);
}

void Game::SimpleSolveStep()
{
   int nX, nY, nAsBomb;
   HexCellsSimpleStep(m_HexCells, &nX, &nY, &nAsBomb);

   HexCellsRevealAs(m_HexCells, nX, nY, nAsBomb);
}


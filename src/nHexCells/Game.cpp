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
   : m_pScreen(pScreen), m_pConfig(pConfig), m_pAchieve(pAchieve)
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
   /*for (int i = 0; i<3; i++)
      HeartsAIFree(&m_aAIs[i]);
   nSDL_FreeFont(m_pFont);*/
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
      //SelectCard();
      break;

      case SDLK_RIGHT:
      case SDLK_6:
      //Move(Right);
      break;

      case SDLK_LEFT:
      case SDLK_4:
      //Move(Left);
      break;

      case SDLK_CLEAR:
      case SDLK_BACKSPACE:
      //RemovedSelectedPieces();
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
      DRAWTEXT(m_pScreen, m_pFont, m_Metrics.GetLeftForPiece(nX, nY), m_Metrics.GetTopForPiece(nX, nY), buffer);
   }
   else if (eType == Bomb || eType == NotBomb) {
      DrawBorder(nX, nY);
      ASSERT(HEXCELLS_IS_REVEALED == HexCellsIsRevealedSpot(m_HexCells, nX, nY));
      if (eType == Bomb) {
         DRAWTEXT(m_pScreen, m_pFont, m_Metrics.GetLeftForPiece(nX, nY), m_Metrics.GetTopForPiece(nX, nY), "B");
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

         DRAWTEXT(m_pScreen, m_pFont, m_Metrics.GetLeftForPiece(nX, nY), m_Metrics.GetTopForPiece(nX, nY), buffer);
      }
   }
   else if (eType == Unknown) {
      DrawBorder(nX, nY);
      DRAWTEXT(m_pScreen, m_pFont, m_Metrics.GetLeftForPiece(nX, nY), m_Metrics.GetTopForPiece(nX, nY), "U");
   }
   else {
      ASSERT(0);
   }
}

void Game::DrawBorder(int nX, int nY)
{
   int nLeft = m_Metrics.GetLeftForPiece(nX, nY);
   int nTop = m_Metrics.GetTopForPiece(nX, nY);
   int nPieceSize = m_Metrics.GetPieceSize();

   short s[] = { nLeft+nPieceSize/2,   nLeft + nPieceSize,  nLeft + nPieceSize,     nLeft+nPieceSize/2,  nLeft,                  nLeft };
   short t[] = { nTop,                 nTop + nPieceSize/3, nTop + 2*nPieceSize/3,  nTop + nPieceSize,   nTop +2*nPieceSize/3,   nTop + nPieceSize/3 };

   filledPolygonRGBA(m_pScreen,
                     s, t,
                     6,
                     255, 0, 0, 255);
}

//void Game::Move(Direction eDirection)
//{
//   int nNumCards = GetNumberOfCardsInHand(m_Hearts, 0);
//   if (eDirection == Left) {
//      m_nCurrentCard--;
//   }
//   else if (eDirection == Right) {
//      m_nCurrentCard++;
//   }
//
//   if (m_nCurrentCard < 0) {
//      m_nCurrentCard = nNumCards-1;
//   }
//   else if (m_nCurrentCard >= nNumCards) {
//      m_nCurrentCard = 0;
//   }
//}
//
//void Game::SelectCard()
//{
//   if (HasPassedCards(m_Hearts) == HEARTSLIB_NOT_PASSED_CARDS) {
//      ToggleSelectedCard(m_Hearts, 0, m_nCurrentCard);
//      if (GetNumberSelectedCards(m_Hearts, 0) == 3) {
//         PassSelectedCards(m_Hearts, 0);
//
//         for (int i = 1; i<4; i++) {
//            int nIndex1, nIndex2, nIndex3;
//            HeartsAIDesiredPassIndexes(m_aAIs[i-1], &nIndex1, &nIndex2, &nIndex3);
//            ToggleSelectedCard(m_Hearts, i, nIndex1);
//            ToggleSelectedCard(m_Hearts, i, nIndex2);
//            ToggleSelectedCard(m_Hearts, i, nIndex3);
//
//            PassSelectedCards(m_Hearts, i);
//         }
//      }
//   }
//   else {
//      PlayCard(m_Hearts, 0, m_nCurrentCard);
//   }
//}
//
//void Game::DoGamePlay()
//{
//   if (SDL_GetTicks() - m_uLastAction < 1000)
//      return;
//
//   if (HasPassedCards(m_Hearts) == HEARTSLIB_PASSED_CARDS) {
//      if (HasEverybodyPlayedTheirCard(m_Hearts)) {
//         int nPlayerIndex;
//         FigureOutWhoTakesTrick(m_Hearts, &nPlayerIndex);
//         int nNumCardsInMiddle = GetNumberOfCardsInMiddle(m_Hearts);
//         for (int nCard = 0; nCard < nNumCardsInMiddle; nCard++) {
//            Card c;
//            GetMiddleCard(m_Hearts, &c, nCard, NULL);
//            m_Pieces.MoveCard(c, m_Metrics.GetPlayerSideX(nPlayerIndex), m_Metrics.GetPlayerSideY(nPlayerIndex));
//         }
//         GiveTrickToPlayer(m_Hearts);
//      }
//      else {
//         int nPlayersTurn = GetPlayersTurn(m_Hearts);
//         if (nPlayersTurn != 0) {
//            int nIndex;
//            HeartsAIDesiredPlayIndex(m_aAIs[nPlayersTurn-1], &nIndex);
//            PlayCard(m_Hearts, nPlayersTurn, nIndex);
//            m_uLastAction = SDL_GetTicks();
//         }
//         return;//return so will check if everyone played their cards again and will give trick to player and update score
//      }
//   }
//
//   if (HasPassedCards(m_Hearts) == HEARTSLIB_PASSED_CARDS) {
//      if (GetNumberOfCardsInHand(m_Hearts, 0) == 0 && GetNumberOfCardsInHand(m_Hearts, 1) == 0 && GetNumberOfCardsInHand(m_Hearts, 2) == 0 && GetNumberOfCardsInHand(m_Hearts, 3) == 0) {
//         ScoreReview r(m_pScreen, &m_Hearts);
//         while (r.Loop()) {}
//         //m_pAchieve->LookForAchievements(m_Hearts);
//         DoHeartsNextHand(m_Hearts);
//         RebuildPieces();
//      }
//   }
//}
//
//void Game::RemovedSelectedPieces()
//{
//   if (HasPassedCards(m_Hearts) != HEARTSLIB_PASSED_CARDS) {
//      int nCardsInHand = GetNumberOfCardsInHand(m_Hearts, 0);
//      for (int i = 0; i<nCardsInHand; i++) {
//         if (IsCardSelected(m_Hearts, 0, i) == HEARTSLIB_CARD_SELECTED)
//            ToggleSelectedCard(m_Hearts, 0, i);
//      }
//   }
//}
//
//void Game::RebuildPieces()
//{
//   m_Pieces.ClearPieces();
//   for (int nPlayerIndex = 0; nPlayerIndex<4; nPlayerIndex++) {
//      int nNumCards = GetNumberOfCardsInHand(m_Hearts, nPlayerIndex);
//      m_Metrics.SetNumCards(nNumCards);
//      for (int i = 0; i<nNumCards; i++) {
//         Card c;
//         GetCardInHand(m_Hearts, &c, nPlayerIndex, i);
//         m_Pieces.CreateCard(c, nPlayerIndex, i, nPlayerIndex == 1 || nPlayerIndex == 3);
//      }
//   }
//}


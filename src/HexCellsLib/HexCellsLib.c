//Public domain :)

#ifdef _TINSPIRE
#include <os.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#endif
#include "HexCellsLib/HexCellsLib.h"
#include "Defines.h"

struct HexCellPiece
{
   SpotType m_eType;
   int m_bRevealed;//Used by bombs, not bombs, indicators(already revealed), and unknowns
   int m_nValue;//Used by bombs, not bombs, indicators, and unknowns.  Normal bombs and unknowns are initially -1
   ValueDetails m_eDetail;//Used by bombs, not bombs, indicators, and unknowns
   struct {//used by indicator types only
      IndicatorOrientation m_eOrientation;
   };
};

struct HexCellsBoard
{
   int m_nWidth;
   int m_nHeight;
   struct HexCellPiece* m_pPieces;
};

struct HexCells
{
   int m_nLastError;
   int m_nTimeElapsed;
   int m_nMistakesMade;
   int m_nBombsRemaining;//Not revealed yet
   struct HexCellsBoard* m_pBoard;
   //Message string
   //About string
};

int IndexOfDirection(IndicatorOrientation eOrientation)
{
   switch(eOrientation) {
      case IO_Left:
         return 0;
      case IO_TopLeft:
         return 1;
      case IO_Top:
         return 2;
      case IO_TopRight:
         return 3;
      case IO_Right:
         return 4;
      case IO_BottomRight:
         return 5;
      case IO_Bottom:
         return 6;
      case IO_BottomLeft:
         return 7;
   }

   assert(0);
   return 0;
}

struct HexCellPiece* GetPieceAtSpot(struct HexCellsBoard* pBoard, int nX, int nY)
{
   struct HexCellPiece* pPiece = NULL;
   DEBUG_FUNC_NAME;

   if( pBoard == NULL || nX < 0 || nY < 0 || nX >= pBoard->m_nWidth || nY >= pBoard->m_nHeight ) {
      assert(0);
      return NULL;
   }

   pPiece = pBoard->m_pPieces + nY*pBoard->m_nWidth + nX;
   return pPiece;
}

int CountBombsNotRevealed(struct HexCellsBoard* pBoard)
{
   struct HexCellPiece* pPiece = NULL;
   int x,y;
   int nCount = 0;
   DEBUG_FUNC_NAME;

   if( pBoard == NULL ) {
      assert(0);
      return -1;
   }

   for(x=0; x<pBoard->m_nWidth; x++)
      for(y=0; y<pBoard->m_nHeight; y++)
         if( GetPieceAtSpot(pBoard, x, y)->m_eType == Bomb && GetPieceAtSpot(pBoard, x, y)->m_bRevealed == HEXCELLS_NOT_REVEALED )
            nCount++;

   return nCount;
}

int IsPiece_Safe(struct HexCellsBoard* pBoard, int nX, int nY)
{
   struct HexCellPiece* pPiece;
   DEBUG_FUNC_NAME;

   if( pBoard == NULL || nX < 0 || nX >= pBoard->m_nWidth || nY < 0 || nY >= pBoard->m_nHeight ) {
      return 0;
   }

   pPiece = GetPieceAtSpot(pBoard, nX, nY);
   assert(pPiece != NULL);

   if( pPiece->m_eType != Nothing && pPiece->m_eType != Indicator )
      return 1;
   return 0;
}

int IsBomb_Safe(struct HexCellsBoard* pBoard, int nX, int nY)
{
   struct HexCellPiece* pPiece;
   DEBUG_FUNC_NAME;

   if( IsPiece_Safe(pBoard, nX, nY) == 0 ) {
      return 0;
   }

   pPiece = GetPieceAtSpot(pBoard, nX, nY);
   assert(pPiece != NULL);

   assert(pPiece->m_eType != Unknown);//TODO: Handle this :)

   if( pPiece->m_eType == Bomb )
      return 1;
   return 0;
}

int GetPiecesSurrounding(struct HexCellsBoard* pBoard, int nX, int nY)
{
   int nCount = 0;
   DEBUG_FUNC_NAME;

   if( pBoard == NULL || nX < 0 || nX >= pBoard->m_nWidth || nY < 0 || nY >= pBoard->m_nHeight ) {
      assert(0);
      return 0;
   }

   //     __
   //  __/  \__
   // /  \__/  \
   // \__/  \__/
   // /  \__/  \
   // \__/  \__/
   //    \__/
   nCount += IsPiece_Safe(pBoard, nX,   nY-2);//Above
   nCount += IsPiece_Safe(pBoard, nX-1, nY-1);//Top Left
   nCount += IsPiece_Safe(pBoard, nX+1, nY-1);//Top Right
   nCount += IsPiece_Safe(pBoard, nX-1, nY+1);//Bottom Left
   nCount += IsPiece_Safe(pBoard, nX+1, nY+1);//Bottom Right
   nCount += IsPiece_Safe(pBoard, nX,   nY+2);//Below

   return nCount;
}

int GetBombsSurrounding(struct HexCellsBoard* pBoard, int nX, int nY, int nRadius, ValueDetails* peDetails)
{
   int nCount = 0;
   DEBUG_FUNC_NAME;

   if( pBoard == NULL || nX < 0 || nX >= pBoard->m_nWidth || nY < 0 || nY >= pBoard->m_nHeight ) {
      assert(0);
      return 0;
   }

   assert(nRadius == 1 || nRadius == 2);

   //     __
   //  __/  \__
   // /  \__/  \
   // \__/  \__/
   // /  \__/  \
   // \__/  \__/
   //    \__/
   if( nRadius >= 1) {
      int arrSurroundingLoc[][2] = {
         {0,  -2},//Above
         {-1, -1},//Top Left
         {+1, -1},//Top Right
         {-1, +1},//Bottom Left
         {+1, +1},//Bottom Right
         {0,  +2},//Below
      };

      int i;

      for(i=0; i<ARR_SIZE(arrSurroundingLoc); i++) {
         nCount += IsBomb_Safe(pBoard, nX+arrSurroundingLoc[i][0],   nY+arrSurroundingLoc[i][1]);
      }

      if( peDetails ) {
         if( nCount <= 0 || GetPieceAtSpot(pBoard, nX, nY)->m_eDetail == HasNumber ) {
            *peDetails = HasNumber;
         }
         else if( nCount >= GetPiecesSurrounding(pBoard, nX, nY ) ) {
            *peDetails = Consecutive;
         }
         else {
            int nIndexBeforeBomb = -1;
            int bFoundNonBomb = 0;

            *peDetails = Consecutive;

            //Find index right before bomb
            for(i=0; i<ARR_SIZE(arrSurroundingLoc); i++) {
               if( IsPiece_Safe(pBoard, nX+arrSurroundingLoc[i][0],   nY+arrSurroundingLoc[i][1]) <= 0 )
                  continue;

               if( IsBomb_Safe(pBoard, nX+arrSurroundingLoc[i][0],   nY+arrSurroundingLoc[i][1]) <= 0 )
                  nIndexBeforeBomb = i;
               else
                  break;
            }

            assert( nIndexBeforeBomb >= 0 && nIndexBeforeBomb < ARR_SIZE(arrSurroundingLoc) );//No gap?

            //Now loop from nIndexBeforeBomb and see if you find a gap
            for(i=0; i<6; i++) {
               int nIndexConsidering = nIndexBeforeBomb + 1 + i;
               if( nIndexConsidering >= ARR_SIZE(arrSurroundingLoc) )
                  nIndexConsidering = 0;

               //Now everything better be a bomb, no piece or non-bombs the rest of the circle
               if( IsPiece_Safe(pBoard, nX+arrSurroundingLoc[nIndexConsidering][0],   nY+arrSurroundingLoc[nIndexConsidering][1]) <= 0 )
                  continue;

               if( IsBomb_Safe(pBoard, nX+arrSurroundingLoc[i][0],   nY+arrSurroundingLoc[i][1]) <= 0 )
                  bFoundNonBomb = 1;
               else {
                  if( bFoundNonBomb == 1 )
                     *peDetails = NotConsecutive;
               }
            }
         }

         assert(*peDetails == GetPieceAtSpot(pBoard, nX, nY)->m_eDetail);
      }
   }

   //        |
   //        v
   //        __
   //     __/  \__
   //  __/  \__/  \__
   // /  \__/  \__/  \
   // \__/  \__/  \__/
   // /  \__/  \__/  \  //<--
   // \__/  \__/  \__/
   // /  \__/  \__/  \
   // \__/  \__/  \__/
   //    \__/  \__/
   //       \__/

   if( nRadius >= 2 ) {
      nCount += IsBomb_Safe(pBoard, nX,   nY-4);//Above, Above
      nCount += IsBomb_Safe(pBoard, nX+1, nY-3);//Above, Above right
      nCount += IsBomb_Safe(pBoard, nX+2, nY-2);//Above right, Above right
      nCount += IsBomb_Safe(pBoard, nX+2, nY  );//Right
      nCount += IsBomb_Safe(pBoard, nX+2, nY+2);//Bottom right, bottom right
      nCount += IsBomb_Safe(pBoard, nX+1, nY+3);//Bottom, bottom right
      nCount += IsBomb_Safe(pBoard, nX,   nY+4);//Bottom, Bottom
      nCount += IsBomb_Safe(pBoard, nX-1, nY+1);//Bottom, bottom left
      nCount += IsBomb_Safe(pBoard, nX-2, nY+2);//Bottom left, bottom left
      nCount += IsBomb_Safe(pBoard, nX-2, nY  );//Left
      nCount += IsBomb_Safe(pBoard, nX-2, nY-2);//Above left, Above left
      nCount += IsBomb_Safe(pBoard, nX-1, nY-3);//Above, Above left

      if( peDetails ) {
         *peDetails = HasNumber;//Blue numbered bombs can only be normal
      }
   }

   return nCount;
}

int GetIndicatorValue(struct HexCellsBoard* pBoard, int nX, int nY, IndicatorOrientation eDirection, ValueDetails* peDetails)
{
   int dX, dY;
   int nPosX, nPosY;
   int nCount = 0;
   int nStartedBombs = 0, nSeparator = 0;
   DEBUG_FUNC_NAME;

   assert(GetPieceAtSpot(pBoard, nX, nY)->m_eType == Indicator);
   if( (eDirection & IO_Left) == IO_Left ) {
      dX = 2;
      dY = 0;
   }
   else if( (eDirection & IO_TopLeft) == IO_TopLeft ) {
      dX = 1;
      dY = 1;
   }
   else if( (eDirection & IO_Top) == IO_Top ) {
      dX = 0;
      dY = 2;
   }
   else if( (eDirection & IO_TopRight) == IO_TopRight ) {
      dX = -1;
      dY = -1;
   }
   else if( (eDirection & IO_Right) == IO_Right ) {
      dX = -2;
      dY = 0;
   }
   else if( (eDirection & IO_BottomRight) == IO_BottomRight ) {
      dX = -1;
      dY = -1;
   }
   else if( (eDirection & IO_Bottom) == IO_Bottom ) {
      dX = 0;
      dY = -2;
   }
   else if( (eDirection & IO_BottomLeft) == IO_BottomLeft ) {
      dX = 1;
      dY = -1;
   }
   else {
      assert(0);
   }

   if( GetPieceAtSpot(pBoard, nX, nY)->m_nValue >= 0 ) {
      nCount = GetPieceAtSpot(pBoard, nX, nY)->m_nValue;
   }
   else {
      nPosX = nX;
      nPosY = nY;
      while(1) {
         struct HexCellPiece* pPiece;
         nPosX += dX;
         nPosY += dY;

         if( nPosX < 0 || nPosX >= pBoard->m_nWidth || nPosY < 0 || nPosY >= pBoard->m_nHeight )
            break;

         pPiece = GetPieceAtSpot(pBoard, nPosX, nPosY);

         if( pPiece->m_eType == Bomb ) {
            nStartedBombs = 1;
            nCount++;
         }

         if( pPiece->m_eType == NotBomb && nStartedBombs == 1 ) {
            nSeparator = 1;
         }
      }
   }

   if( peDetails ) {
      struct HexCellPiece* pPiece = GetPieceAtSpot(pBoard, nX, nY);
      *peDetails = pPiece->m_eDetail;

      assert( *peDetails != Question );//Not exposing concealed for indicators
   }

   return nCount;
}

void UpdateValues(struct HexCellsBoard* pBoard)
{
   struct HexCellPiece* pPiece = NULL;
   int x, y;
   DEBUG_FUNC_NAME;

   if (pBoard == NULL) {
      assert(0);
   }

   for (x = 0; x<pBoard->m_nWidth; x++) {
      for (y = 0; y<pBoard->m_nHeight; y++) {
         pPiece = GetPieceAtSpot(pBoard, x, y);
         if (pPiece->m_eType == Indicator) {
            pPiece->m_nValue = GetIndicatorValue(pBoard, x, y, pPiece->m_eOrientation, NULL);
         }
         else if (pPiece->m_eType == NotBomb) {
            pPiece->m_nValue = GetBombsSurrounding(pBoard, x, y, 1, NULL);
         }
         else if (pPiece->m_eType == Bomb) {
            pPiece->m_nValue = GetBombsSurrounding(pBoard, x, y, 2, NULL);
         }
         else if (pPiece->m_eType == Nothing) {
         }
         else {
            assert(0);
         }
      }
   }
}

int ReadInt(char** ppstr, int* pnNum)
{
   int nRet;
   char* pstr = *ppstr;
   while(*pstr == ' ')
      pstr++;
   nRet = sscanf(*ppstr, "%d", pnNum);
   if( nRet >= 0 ) {
      pstr++;
      if( *pstr == '\0' ) {
         *ppstr = pstr;
         return nRet;
      }
      while (*pstr != ' ')
         pstr++;
   }
   *ppstr = pstr;
   return nRet;
}

int HexCellsLibCreate(HexCellsLib* api, const char* pstrGameData)
{
   struct HexCells* pH;
   struct HexCellsBoard* pBoard;
   struct HexCellPiece* pPiece;
   char buffer[255];
   char* pch;
   DEBUG_FUNC_NAME;

   pH = malloc(sizeof(struct HexCells));
   if( pH == NULL ){//Out of memory
      return HEXCELLSLIB_OUT_OF_MEMORY;
   }

   pH->m_nTimeElapsed = 0;
   pH->m_nMistakesMade = 0;

   pH->m_pBoard = malloc(sizeof(struct HexCellsBoard));
   if( pH->m_pBoard == NULL ) {
      free(pH);
      return HEXCELLSLIB_OUT_OF_MEMORY;
   }

   pBoard = pH->m_pBoard;
   pch = strstr(pstrGameData, "Hexcells level v1");
   if( pch != NULL ) {
      int nRowNum = 0;
      char* pstr = (char*)pstrGameData;
      int nRes = 1;
      int nX = 0;
      int nY = 0;

      pBoard->m_nWidth = 33;
      pBoard->m_nHeight = 33;
      pBoard->m_pPieces = malloc(pBoard->m_nWidth*pBoard->m_nHeight*sizeof(struct HexCellPiece));
      if( pBoard->m_pPieces == NULL ) {
         free(pH->m_pBoard);
         free(pH);
         return HEXCELLSLIB_OUT_OF_MEMORY;
      }

      while(nRes == 1)
      {
         nRowNum++;
         nRes = sscanf(pstr, "%[^\n]s", buffer);
         if( nRes != 1 )
            break;
         pstr += strlen(buffer) + 1/*\n*/;


         if( nRowNum >= 6 ) {
            char ch1;
            char ch2;

            char* p = buffer;
            while(1)
            {
               ch1 = *p++;
               if( ch1 == '\t' )
                  continue;
               if( ch1 == '\0' )
                  break;
               ch2 = *p++;
               if( ch2 == '\0' )
                  break;

               pPiece = GetPieceAtSpot(pBoard, nX, nY);

               pPiece->m_nValue = -1;

               if( ch1 == 'o' || ch1 == 'O' ) {
                  pPiece->m_eType = NotBomb;
               }
               else if( ch1 == 'x' || ch1 == 'X' ) {
                  pPiece->m_eType = Bomb;
               }
               else if( ch1 == '\\' || ch1 == '|' || ch1 == '/' ) {
                  pPiece->m_eType = Indicator;
               }
               else if( ch1 == '.' )
                  pPiece->m_eType = Nothing;
               else {
                  assert(0);
                  pPiece->m_eType = Unknown;
               }

               if( ch1 == 'O' || ch1 == 'X' )
                  pPiece->m_bRevealed = HEXCELLS_IS_REVEALED;
               else
                  pPiece->m_bRevealed = HEXCELLS_NOT_REVEALED;

               if( pPiece->m_eType == Indicator ) {
                  pPiece->m_eDetail = HasNumber;//Default;
                  pPiece->m_eOrientation = IO_Top;

                  if( ch1 == '\\' )
                     pPiece->m_eOrientation = IO_TopLeft;
                  else if( ch1 == '|' )
                     pPiece->m_eOrientation = IO_Top;
                  else if( ch1 == '/' )
                     pPiece->m_eOrientation = IO_TopRight;

                  if( ch2 == '+' ) {
                     pPiece->m_eDetail = HasNumber;
                  }
                  else if( ch2 == 'c' ) {
                     pPiece->m_eDetail = Consecutive;
                  }
                  else if( ch2 == 'n' ) {
                     pPiece->m_eDetail = NotConsecutive;
                  }
                  else {
                     assert(0);
                  }
               }
               else {
                  if( ch2 == '.' )
                     pPiece->m_eDetail = Question;
                  else if( ch2 == '+' )
                     pPiece->m_eDetail = HasNumber;
                  else if( ch2 == 'c' )
                     pPiece->m_eDetail = Consecutive;
                  else if( ch2 == 'n' )
                     pPiece->m_eDetail = NotConsecutive;
                  else {
                     assert(0);
                  }
               }

               nX++;
               if( nX >= pBoard->m_nWidth ) {
                  nX = 0;
               }
            }
            nY++;
         }
      }
      UpdateValues(pBoard);
      pH->m_nBombsRemaining = CountBombsNotRevealed(pBoard);
   }
   else {
      pch = strstr(pstrGameData, "Hexcells 1");
      if( pch != NULL ) {
         int x,y;
         char* pstr = (char*)pstrGameData;
         pstr += strlen("Hexcells 1");
         int nMistakesMade, nBoardWidth, nBoardHeight, nBombsNotRevealed;

         ReadInt(&pstr, &nMistakesMade);
         ReadInt(&pstr, &nBoardWidth);
         ReadInt(&pstr, &nBoardHeight);
         ReadInt(&pstr, &nBombsNotRevealed);

         pH->m_nMistakesMade = nMistakesMade;
         pBoard->m_nWidth = nBoardWidth;
         pBoard->m_nHeight = nBoardHeight;

         pBoard->m_pPieces = malloc(pBoard->m_nWidth*pBoard->m_nHeight*sizeof(struct HexCellPiece));
         if( pBoard->m_pPieces == NULL ) {
            free(pH->m_pBoard);
            free(pH);
            return HEXCELLSLIB_OUT_OF_MEMORY;
         }

         for(x=0; x<pBoard->m_nWidth; x++)
            for(y=0; y<pBoard->m_nHeight; y++)
               GetPieceAtSpot(pBoard, x, y)->m_eType = Nothing;

         while(1) {
            int res = 1;
            int x, y, nType;
            if( 1 != ReadInt(&pstr, &x) )
               break;
            if( 1 != ReadInt(&pstr, &y) )
               break;
            if( 1 != ReadInt(&pstr, &nType) )
               break;

            pPiece = GetPieceAtSpot(pBoard, x, y);
            pPiece->m_eType = nType;

            ReadInt(&pstr, &pPiece->m_bRevealed);
            assert( pPiece->m_bRevealed == HEXCELLS_IS_REVEALED || pPiece->m_bRevealed == HEXCELLS_NOT_REVEALED );

            ReadInt(&pstr, &pPiece->m_nValue);
            assert( pPiece->m_nValue >= -1 );

            int nDetails;
            ReadInt(&pstr, &nDetails);
            pPiece->m_eDetail = nDetails;

            if( pPiece->m_eType == Bomb || pPiece->m_eType == NotBomb ) {
            }
            else if( pPiece->m_eType == Indicator ) {
               int nOrientation;
               ReadInt(&pstr, &nOrientation);
               pPiece->m_eOrientation = nOrientation;
            }
            else if( pPiece->m_eType == Unknown ) {
            }
            else {
               assert(0);//Not finished yet.
            }

            if( *pstr == '\0' ) {
               break;
            }
         }

         pH->m_nBombsRemaining = nBombsNotRevealed;
      }
   }

   pH->m_nLastError = HEXCELLSLIB_OK;

   *api = pH;

   return HEXCELLSLIB_OK;
}

int HexCellsLibFree(HexCellsLib* api)
{
   struct HexCells* pH;
   struct HexCellsBoard* pBoard;
   struct HexCellPiece* pPiece;
   DEBUG_FUNC_NAME;

   pH = *api;

   pBoard = pH->m_pBoard;
   pPiece = pBoard->m_pPieces;
   free(pPiece);
   free(pBoard);
   free(pH);

   *api = NULL;
   return HEXCELLSLIB_OK;
}

int GetHexCellsLibError(HexCellsLib api)
{
   struct HexCells* pH;
   DEBUG_FUNC_NAME;

   pH = (struct HexCells*)api;
   return pH->m_nLastError;
}

void ClearHexCellsLibError(HexCellsLib api)
{
   struct HexCells* pH;
   DEBUG_FUNC_NAME;

   pH = (struct HexCells*)api;
   pH->m_nLastError = HEXCELLSLIB_OK;
}

int HexCellsSave(HexCellsLib api, char* pstr)
{
   DEBUG_FUNC_NAME;

   struct HexCells* pH = (struct HexCells*)api;

   strcpy(pstr, "Hexcells 1 ");

   char buffer[15];
   sprintf(buffer, "%d ", pH->m_nMistakesMade);
   strcat(pstr, buffer);

   sprintf(buffer, "%d ", pH->m_pBoard->m_nWidth);
   strcat(pstr, buffer);

   sprintf(buffer, "%d ", pH->m_pBoard->m_nHeight);
   strcat(pstr, buffer);

   int x=0, y=0;
   for(x=0; x<pH->m_pBoard->m_nWidth; x++) {
      for(y=0; y<pH->m_pBoard->m_nHeight; y++) {
         struct HexCellPiece* pPiece = GetPieceAtSpot(pH->m_pBoard, x, y);

         if( pPiece->m_eType == Nothing )
            continue;

         sprintf(buffer, "%d ", x);
         strcat(pstr, buffer);

         sprintf(buffer, "%d ", y);
         strcat(pstr, buffer);

         sprintf(buffer, "%d ", pPiece->m_eType);
         strcat(pstr, buffer);

         sprintf(buffer, "%d ", pPiece->m_bRevealed);
         strcat(pstr, buffer);

         sprintf(buffer, "%d ", pPiece->m_nValue);
         strcat(pstr, buffer);

         sprintf(buffer, "%d ", pPiece->m_eDetail);
         strcat(pstr, buffer);

         if( pPiece->m_eType == Indicator ) {
            sprintf(buffer, "%d ", pPiece->m_eOrientation);
            strcat(pstr, buffer);
         }
         else if (pPiece->m_eType == Bomb || pPiece->m_eType == NotBomb) {
         }
         else {
            assert(0);//Doesn't know about Unknowns yet
         }
      }
   }
   
   return HEXCELLSLIB_OK;
}

//HeartsLib related functions
int HexCellsIsGameOver(HexCellsLib api)
{
   struct HexCells* pH;
   int x,y;
   DEBUG_FUNC_NAME;

   pH = (struct HexCells*)api;

   for(x=0; x<pH->m_pBoard->m_nWidth; x++)
      for(y=0; y<pH->m_pBoard->m_nHeight; y++)
         if( GetPieceAtSpot(pH->m_pBoard, x, y)->m_eType == Bomb || GetPieceAtSpot(pH->m_pBoard, x, y)->m_eType == NotBomb || GetPieceAtSpot(pH->m_pBoard, x, y)->m_eType == Unknown )
            if( HexCellsIsRevealedSpot(api, x, y) == HEXCELLS_NOT_REVEALED )
               return HEXCELLS_NOT_GAMEOVER;

   return HEXCELLS_IS_GAMEOVER;
}

int HexCellsGetWidth(HexCellsLib api)
{
   struct HexCells* pH;
   DEBUG_FUNC_NAME;

   pH = (struct HexCells*)api;
   return pH->m_pBoard->m_nWidth;
}

int HexCellsGetHeight(HexCellsLib api)
{
   struct HexCells* pH;
   DEBUG_FUNC_NAME;

   pH = (struct HexCells*)api;
   return pH->m_pBoard->m_nHeight;
}

int HexCellsGetBombsRemaining(HexCellsLib api)
{
   struct HexCells* pH;
   DEBUG_FUNC_NAME;

   pH = (struct HexCells*)api;

   return pH->m_nBombsRemaining;
}

int HexCellGetSpotType(HexCellsLib api, int nX, int nY, SpotType* peType)
{
   struct HexCells* pH;
   DEBUG_FUNC_NAME;

   pH = (struct HexCells*)api;
   if( peType ) {
      *peType = GetPieceAtSpot(pH->m_pBoard, nX, nY)->m_eType;

      if( (*peType == Bomb || *peType == NotBomb) && HEXCELLS_NOT_REVEALED == HexCellsIsRevealedSpot(api, nX, nY) )
         *peType = Unknown;
   }

   return HEXCELLSLIB_OK;
}

int HexCellsGetRevealedSpotValue(HexCellsLib api, int nX, int nY, int* pnValue, ValueDetails* peDetails)
{
   struct HexCells* pH;
   struct HexCellPiece* pPiece;
   SpotType eType;
   DEBUG_FUNC_NAME;

   pH = (struct HexCells*)api;
   pPiece = GetPieceAtSpot(pH->m_pBoard, nX, nY);
   eType = pPiece->m_eType;

   if( eType != Bomb && eType != NotBomb )//Should only called on bombs and not-bombs; not indicators
      return HEXCELLSLIB_BADARGUMENT;

   if( HEXCELLS_IS_REVEALED != pPiece->m_bRevealed )//Should only be called on revealed pieces
      return HEXCELLSLIB_BADARGUMENT;

   if( pnValue ) {
      if( pPiece->m_nValue >= 0 ) {
         *pnValue = pPiece->m_nValue;
         if( peDetails )
            *peDetails = pPiece->m_eDetail;
         return HEXCELLSLIB_OK;
      }

      if( pPiece->m_eDetail == Question ) {
         *pnValue = -1;//
         if( peDetails )
            *peDetails = Question;
      }
      else if( eType == NotBomb ) {
         //Figure out bombs surrounding this spot
         *pnValue = GetBombsSurrounding(pH->m_pBoard, nX, nY, 1/*Radius*/, peDetails);
      }
      else if( eType == Bomb ) {
         //Figure out bombs 2x surrounding this spot
         *pnValue = GetBombsSurrounding(pH->m_pBoard, nX, nY, 2/*Radius*/, peDetails);
      }

   }

   return HEXCELLSLIB_OK;
}

int HexCellsGetIndicatorDirection(HexCellsLib api, int nX, int nY, IndicatorOrientation* peDirection)
{
   struct HexCells* pH;
   DEBUG_FUNC_NAME;

   pH = (struct HexCells*)api;

   if( GetPieceAtSpot(pH->m_pBoard, nX, nY)->m_eType != Indicator ) {
      assert(0);
      return HEXCELLSLIB_BADARGUMENT;
   }

   if( peDirection ) {
      *peDirection = GetPieceAtSpot(pH->m_pBoard, nX, nY)->m_eOrientation;
   }

   return HEXCELLSLIB_OK;
}

int HexCellsGetIndicatorValue(HexCellsLib api, int nX, int nY, IndicatorOrientation eDirection, int* pnValue, ValueDetails* peDetails)
{
   struct HexCells* pH;
   DEBUG_FUNC_NAME;

   pH = (struct HexCells*)api;

   if( GetPieceAtSpot(pH->m_pBoard, nX, nY)->m_eType != Indicator ) {
      assert(0);
      return HEXCELLSLIB_BADARGUMENT;
   }

   if( pnValue ) {
      *pnValue = GetIndicatorValue(pH->m_pBoard, nX, nY, eDirection, peDetails);
   }

   return HEXCELLSLIB_OK;
}

int HexCellsIsRevealedSpot(HexCellsLib api, int nX, int nY)
{
   struct HexCells* pH;
   DEBUG_FUNC_NAME;

   pH = (struct HexCells*)api;

   if( GetPieceAtSpot(pH->m_pBoard, nX, nY)->m_eType != Bomb && GetPieceAtSpot(pH->m_pBoard, nX, nY)->m_eType != NotBomb && GetPieceAtSpot(pH->m_pBoard, nX, nY)->m_eType != Unknown ) {
      assert(0);
      return HEXCELLS_NOT_REVEALED;
   }

   return GetPieceAtSpot(pH->m_pBoard, nX, nY)->m_bRevealed;
}

int HexCellsRevealAs(HexCellsLib api, int nX, int nY, int nAsBomb)
{
   struct HexCells* pH;
   SpotType eType;
   DEBUG_FUNC_NAME;

   pH = (struct HexCells*)api;

   eType = GetPieceAtSpot(pH->m_pBoard, nX, nY)->m_eType;
   if( eType != Bomb && eType != NotBomb && eType != Unknown )//You cannot click on an indicator or nothing
      return HEXCELLSLIB_BADARGUMENT;

   if( HexCellsIsRevealedSpot(api, nX, nY) == HEXCELLS_IS_REVEALED )
      return HEXCELLSLIB_BADARGUMENT;

   //TODO: Keep undo history :)
   if( eType == Unknown ) {
      assert(0);//Use HexCellsUnknownRevealAs
   }
   if( (eType == Bomb && nAsBomb == 1) || (eType == NotBomb && nAsBomb == 0) ) {
      GetPieceAtSpot(pH->m_pBoard, nX, nY)->m_bRevealed = HEXCELLS_IS_REVEALED;
      return HEXCELLS_CORRECT;
   }

   return HEXCELLS_INCORRECT;
}

int HexCellsUnknownRevealAs(HexCellsLib api, int nX, int nY, int nAsBomb, ValueDetails eDetails, int nValue)
{
   struct HexCells* pH;
   struct HexCellPiece* pPiece;
   DEBUG_FUNC_NAME;

   pH = (struct HexCells*)api;
   pPiece = GetPieceAtSpot(pH->m_pBoard, nX, nY);

   assert( pPiece->m_eType == Unknown );
   if( pPiece->m_eType != Unknown )
      return HEXCELLSLIB_BADARGUMENT;

   pPiece->m_bRevealed = HEXCELLS_IS_REVEALED;
   pPiece->m_eDetail = eDetails;
   pPiece->m_eType = nAsBomb == HEXCELLS_REVEAL_BOMB ? Bomb : NotBomb;
   pPiece->m_nValue = nValue;

   assert(pPiece->m_nValue >= -1);

   return HEXCELLSLIB_OK;
}


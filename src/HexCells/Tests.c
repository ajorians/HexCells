#ifdef _TINSPIRE
#include <nspireio.h>
#else
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#endif
#include "HexCellsLib/HexCellsLib.h"

#ifdef _TINSPIRE
#define PRINT  nio_printf
#else
#define PRINT  printf
#endif

#ifdef _TINSPIRE
#define PRINT_FUNC PRINT("%s", __func__);
#else
#define PRINT_FUNC PRINT("%s", __FUNCTION__);
#endif

#ifndef ARR_SIZE
#define ARR_SIZE(x) sizeof(x)/sizeof(x[0])
#endif

#define TEST_SUCCEEDED  (0)
#define TEST_FAILED     (1)

#define TEST_PERSISTING

#ifdef _TINSPIRE
int g_nRelocatedGlobals = 0;
#endif

#define SIMPLE_GAME_DATA "	Hexcells level v1\n\
	Simple Game\n\
	A.J.\n\
	\n\
	Just an easy one\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................|+..............................\n\
	................................x.................................\n\
	..............................o+..o+..............................\n\
	........................\\+..x...o+..o+............................\n\
	..........................o+..o+..x...O+..........................\n\
	............................o+..O+..o+............................\n\
	..............................x...x...............................\n\
	................................O+................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	.................................................................."

#define SIMPLE_GAME_DATA2 "	Hexcells level v1\n\
	Simple Game\n\
	A.J.\n\
	\n\
	Just an easy one\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................|+..............................\n\
	................................x.................................\n\
	..............................o+..o+..............................\n\
	........................\\+..x...o+..o+............................\n\
	..........................o+..o+..x...O+..........................\n\
	............................o+..On..o+............................\n\
	..............................x...x...............................\n\
	................................On................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	..................................................................\n\
	.................................................................."

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

#define NEW_GAME_DATA_UNKNOWN   "Hexcells 1 0 8 8 5 \
0 3 0 0 1 0 64 \
1 4 3 0 -1 1 \
2 3 3 0 -1 1 \
2 5 3 0 -1 1 \
3 2 3 0 -1 1 \
3 4 3 0 -1 1 \
3 6 3 0 -1 1 \
4 1 3 0 -1 1 \
4 3 3 0 -1 1 \
4 5 2 1 3 3 \
4 7 2 1 2 3 \
5 0 0 0 2 0 32 \
5 2 3 0 -1 1 \
5 4 3 0 -1 1 \
5 6 3 0 -1 1 \
6 3 3 0 -1 1 \
6 5 3 0 -1 1 \
7 4 2 1 0 0"

int TestConstruction()
{
   HexCellsLib api;
   PRINT_FUNC;
   if( HEXCELLSLIB_OK != HexCellsLibCreate(&api, SIMPLE_GAME_DATA) )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestBombsRemaining()
{
   HexCellsLib api;
   PRINT_FUNC;
   if( HEXCELLSLIB_OK != HexCellsLibCreate(&api, SIMPLE_GAME_DATA) )
      return TEST_FAILED;

   if( 5 != HexCellsGetBombsRemaining(api) )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestRevealing()
{
   HexCellsLib api;
   PRINT_FUNC;
   if( HEXCELLSLIB_OK != HexCellsLibCreate(&api, SIMPLE_GAME_DATA) )
      return TEST_FAILED;

   if( 5 != HexCellsGetBombsRemaining(api) )
      return TEST_FAILED;

   if( HEXCELLS_IS_GAMEOVER == HexCellsIsGameOver(api) )
      return TEST_FAILED;

   if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 16, 13, HEXCELLS_REVEAL_BOMB) )
      return TEST_FAILED;
   ///
   if( HEXCELLS_IS_GAMEOVER == HexCellsIsGameOver(api) )
      return TEST_FAILED;

   if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 15, 14, HEXCELLS_REVEAL_NOT_BOMB) )
      return TEST_FAILED;
   if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 17, 14, HEXCELLS_REVEAL_NOT_BOMB) )
      return TEST_FAILED;
   ///
   if( HEXCELLS_IS_GAMEOVER == HexCellsIsGameOver(api) )
      return TEST_FAILED;

   if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 14, 15, HEXCELLS_REVEAL_BOMB) )
      return TEST_FAILED;
   if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 16, 15, HEXCELLS_REVEAL_NOT_BOMB) )
      return TEST_FAILED;
   if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 18, 15, HEXCELLS_REVEAL_NOT_BOMB) )
      return TEST_FAILED;
   ///
   if( HEXCELLS_IS_GAMEOVER == HexCellsIsGameOver(api) )
      return TEST_FAILED;

   if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 13, 16, HEXCELLS_REVEAL_NOT_BOMB) )
      return TEST_FAILED;
   if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 15, 16, HEXCELLS_REVEAL_NOT_BOMB) )
      return TEST_FAILED;
   if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 17, 16, HEXCELLS_REVEAL_BOMB) )
      return TEST_FAILED;
   //if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 19, 16, HEXCELLS_REVEAL_NOT_BOMB) )
      //return TEST_FAILED;
   ///
   if( HEXCELLS_IS_GAMEOVER == HexCellsIsGameOver(api) )
      return TEST_FAILED;

   if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 14, 17, HEXCELLS_REVEAL_NOT_BOMB) )
      return TEST_FAILED;
   //if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 16, 17, HEXCELLS_REVEAL_NOT_BOMB) )
      //return TEST_FAILED;
   if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 18, 17, HEXCELLS_REVEAL_NOT_BOMB) )
      return TEST_FAILED;
   ///
   if( HEXCELLS_IS_GAMEOVER == HexCellsIsGameOver(api) )
      return TEST_FAILED;

   if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 15, 18, HEXCELLS_REVEAL_BOMB) )
      return TEST_FAILED;
   if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 17, 18, HEXCELLS_REVEAL_BOMB) )
      return TEST_FAILED;
   ///
   //if( HEXCELLS_IS_GAMEOVER == HexCellsIsGameOver(api) )
      //return TEST_FAILED;

   //if( HEXCELLS_CORRECT != HexCellsRevealAs(api, 16, 19, HEXCELLS_REVEAL_NOT_BOMB) )
      //return TEST_FAILED;

   if( HEXCELLS_IS_GAMEOVER != HexCellsIsGameOver(api) )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestRevealedNumbers()
{
   int nValue;
   ValueDetails eDetails;
   HexCellsLib api;
   PRINT_FUNC;
   if( HEXCELLSLIB_OK != HexCellsLibCreate(&api, SIMPLE_GAME_DATA) )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 19, 16, &nValue, &eDetails) || nValue != 0 || eDetails != HasNumber )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 16, 17, &nValue, &eDetails) || nValue != 3 || eDetails != HasNumber )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 16, 19, &nValue, &eDetails) || nValue != 2 || eDetails != HasNumber )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestIndicators()
{
   SpotType eType;
   HexCellsLib api;
   int nValue;
   ValueDetails eDetails;
   IndicatorOrientation eDirection;
   PRINT_FUNC;
   if( HEXCELLSLIB_OK != HexCellsLibCreate(&api, SIMPLE_GAME_DATA) )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellGetSpotType(api, 17, 12, &eType) || eType != Indicator )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetIndicatorDirection(api, 17, 12, &eDirection) || eDirection != IO_Top )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetIndicatorValue(api, 17, 12, IO_Top, &nValue, &eDetails) || nValue != 2 || eDetails != HasNumber )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellGetSpotType(api, 12, 15, &eType) || eType != Indicator )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetIndicatorDirection(api, 12, 15, &eDirection) || eDirection != IO_TopLeft )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetIndicatorValue(api, 12, 15, IO_TopLeft, &nValue, &eDetails) || nValue != 1 || eDetails != HasNumber )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestRevealedDetails()
{
   int nValue;
   ValueDetails eDetails;
   HexCellsLib api;
   PRINT_FUNC;
   if( HEXCELLSLIB_OK != HexCellsLibCreate(&api, SIMPLE_GAME_DATA2) )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 19, 16, &nValue, &eDetails) || nValue != 0 || eDetails != HasNumber )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 16, 17, &nValue, &eDetails) || nValue != 3 || eDetails != NotConsecutive )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 16, 19, &nValue, &eDetails) || nValue != 2 || eDetails != NotConsecutive )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestLoadNewData()
{
   int nValue;
   ValueDetails eDetails;
   SpotType eType;
   IndicatorOrientation eDirection;
   HexCellsLib api;
   PRINT_FUNC;
   if( HEXCELLSLIB_OK != HexCellsLibCreate(&api, NEW_GAME_DATA) )
      return TEST_FAILED;

   if( 5 != HexCellsGetBombsRemaining(api) )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 7, 4, &nValue, &eDetails) || nValue != 0 || eDetails != HasNumber )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 4, 5, &nValue, &eDetails) || nValue != 3 || eDetails != NotConsecutive )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 4, 7, &nValue, &eDetails) || nValue != 2 || eDetails != NotConsecutive )
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellGetSpotType(api, 5, 0, &eType) || eType != Indicator)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetIndicatorDirection(api, 5, 0, &eDirection) || eDirection != IO_Top)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetIndicatorValue(api, 5, 0, IO_Top, &nValue, &eDetails) || nValue != 2 || eDetails != HasNumber)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellGetSpotType(api, 0, 3, &eType) || eType != Indicator)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetIndicatorDirection(api, 0, 3, &eDirection) || eDirection != IO_TopLeft)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetIndicatorValue(api, 0, 3, IO_TopLeft, &nValue, &eDetails) || nValue != 1 || eDetails != HasNumber)
      return TEST_FAILED;

   if (HEXCELLS_IS_GAMEOVER == HexCellsIsGameOver(api))
      return TEST_FAILED;

   //Taking first turn
   struct
   {
      int x;
      int y;
      int nReveal;
      ValueDetails eDetails;
      int nValue;
   } arrReveal[] =
   {
      { 6, 3, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 1  },
      { 6, 5, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 2  },
      { 5, 6, HEXCELLS_REVEAL_BOMB,     Question, -1  },
      { 3, 6, HEXCELLS_REVEAL_BOMB,     Question, -1  },
      { 5, 4, HEXCELLS_REVEAL_BOMB,     Question, -1  },
      { 5, 2, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 2  },
      { 4, 3, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 2  },
      { 4, 1, HEXCELLS_REVEAL_BOMB,     Question, -1  },
      { 3, 4, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 2  },
      { 3, 2, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 2  },
      { 2, 3, HEXCELLS_REVEAL_BOMB,     Question, -1  },
      { 2, 5, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 2  }
   };

   for (int i = 0; i<ARR_SIZE(arrReveal); i++) {
      if (HEXCELLS_CORRECT != HexCellsRevealAs(api, arrReveal[i].x, arrReveal[i].y, arrReveal[i].nReveal) )
         return TEST_FAILED;

      if( HEXCELLS_IS_REVEALED != HexCellsIsRevealedSpot(api, arrReveal[i].x, arrReveal[i].y) )
         return TEST_FAILED;

      if( arrReveal[i].nReveal == HEXCELLS_REVEAL_NOT_BOMB ) {
         int nValue;
         ValueDetails eDetails;
         if( HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, arrReveal[i].x, arrReveal[i].y, &nValue, &eDetails) && nValue != arrReveal[i].nValue && eDetails != arrReveal[i].eDetails )
            return TEST_FAILED;
      }

      if (HEXCELLS_IS_GAMEOVER == HexCellsIsGameOver(api))
         return TEST_FAILED;
   }

   if (HEXCELLS_CORRECT != HexCellsRevealAs(api, 1, 4, HEXCELLS_REVEAL_NOT_BOMB))
      return TEST_FAILED;

   if (HEXCELLS_IS_GAMEOVER != HexCellsIsGameOver(api))
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestLoadNewUnknownData()
{
   int nValue;
   ValueDetails eDetails;
   SpotType eType;
   IndicatorOrientation eDirection;
   HexCellsLib api;
   PRINT_FUNC;
   if (HEXCELLSLIB_OK != HexCellsLibCreate(&api, NEW_GAME_DATA_UNKNOWN))
      return TEST_FAILED;

   if( 5 != HexCellsGetBombsRemaining(api) )
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 7, 4, &nValue, &eDetails) || nValue != 0 || eDetails != HasNumber)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 4, 5, &nValue, &eDetails) || nValue != 3 || eDetails != NotConsecutive)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 4, 7, &nValue, &eDetails) || nValue != 2 || eDetails != NotConsecutive)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellGetSpotType(api, 5, 0, &eType) || eType != Indicator)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetIndicatorDirection(api, 5, 0, &eDirection) || eDirection != IO_Top)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetIndicatorValue(api, 5, 0, IO_Top, &nValue, &eDetails) || nValue != 2 || eDetails != HasNumber)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellGetSpotType(api, 0, 3, &eType) || eType != Indicator)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetIndicatorDirection(api, 0, 3, &eDirection) || eDirection != IO_TopLeft)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetIndicatorValue(api, 0, 3, IO_TopLeft, &nValue, &eDetails) || nValue != 1 || eDetails != HasNumber)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsLibFree(&api))
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestSolveNewUnknownData()
{
   int nValue;
   ValueDetails eDetails;
   SpotType eType;
   IndicatorOrientation eDirection;
   HexCellsLib api;
   PRINT_FUNC;
   if (HEXCELLSLIB_OK != HexCellsLibCreate(&api, NEW_GAME_DATA_UNKNOWN))
      return TEST_FAILED;

   if (5 != HexCellsGetBombsRemaining(api))
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 7, 4, &nValue, &eDetails) || nValue != 0 || eDetails != HasNumber)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 4, 5, &nValue, &eDetails) || nValue != 3 || eDetails != NotConsecutive)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 4, 7, &nValue, &eDetails) || nValue != 2 || eDetails != NotConsecutive)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellGetSpotType(api, 5, 0, &eType) || eType != Indicator)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetIndicatorDirection(api, 5, 0, &eDirection) || eDirection != IO_Top)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetIndicatorValue(api, 5, 0, IO_Top, &nValue, &eDetails) || nValue != 2 || eDetails != HasNumber)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellGetSpotType(api, 0, 3, &eType) || eType != Indicator)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetIndicatorDirection(api, 0, 3, &eDirection) || eDirection != IO_TopLeft)
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsGetIndicatorValue(api, 0, 3, IO_TopLeft, &nValue, &eDetails) || nValue != 1 || eDetails != HasNumber)
      return TEST_FAILED;

   if (HEXCELLS_IS_GAMEOVER == HexCellsIsGameOver(api))
      return TEST_FAILED;

   //Taking first turn
   struct  
   {
      int x;
      int y;
      int nReveal;
      ValueDetails eDetails;
      int nValue;
   } arrReveal[] = 
   {
      { 6, 3, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 1  },
      { 6, 5, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 2  },
      { 5, 6, HEXCELLS_REVEAL_BOMB,     Question, -1  },
      { 3, 6, HEXCELLS_REVEAL_BOMB,     Question, -1  },
      { 5, 4, HEXCELLS_REVEAL_BOMB,     Question, -1  },
      { 5, 2, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 2  },
      { 4, 3, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 2  },
      { 4, 1, HEXCELLS_REVEAL_BOMB,     Question, -1  },
      { 3, 4, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 2  },
      { 3, 2, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 2  },
      { 2, 3, HEXCELLS_REVEAL_BOMB,     Question, -1  },
      { 2, 5, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 2  }
   };

   for(int i=0; i<ARR_SIZE(arrReveal); i++) {
      if (HEXCELLSLIB_OK != HexCellsUnknownRevealAs(api, arrReveal[i].x, arrReveal[i].y, arrReveal[i].nReveal, arrReveal[i].eDetails, arrReveal[i].nValue))
         return TEST_FAILED;

      if (HEXCELLS_IS_GAMEOVER == HexCellsIsGameOver(api))
         return TEST_FAILED;
   }

   if (HEXCELLSLIB_OK != HexCellsUnknownRevealAs(api, 1, 4, HEXCELLS_REVEAL_NOT_BOMB, HasNumber, 1))
      return TEST_FAILED;

   if (HEXCELLS_IS_GAMEOVER != HexCellsIsGameOver(api))
      return TEST_FAILED;

   if (HEXCELLSLIB_OK != HexCellsLibFree(&api))
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

typedef int (*testfunc)();
testfunc g_Tests[] =
{
   TestConstruction,
   TestBombsRemaining,
   TestRevealing,
   TestRevealedNumbers,
   TestIndicators,
   TestRevealedDetails,
   TestLoadNewData,
   TestLoadNewUnknownData,
   TestSolveNewUnknownData
};

void RunTests()
{
#ifdef _TINSPIRE
   if( !g_nRelocatedGlobals ) {
      nl_relocdata((unsigned*)g_Tests, sizeof(g_Tests)/sizeof(g_Tests[0]));
      g_nRelocatedGlobals = 1;
   }
#endif

   unsigned i;
   for(i=0; i<sizeof(g_Tests)/sizeof(testfunc); i++) {

      int nRet = g_Tests[i]();
      if( nRet == TEST_SUCCEEDED ) {
         PRINT("........OK\n");
      } else {
         PRINT("........FAIL\n");
      }
   }
}


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

#define NEW_GAME_DATA   "Hexcells 1 \
0 8 8 \
0 3 0 0 64 0 0 0 0 0 0 0 0 \
1 4 2 0 0 \
2 3 1 0 1 \
2 5 2 0 0 \
3 2 2 0 0 \
3 4 2 0 0 \
3 6 1 0 1 \
4 1 1 0 1 \
4 3 2 0 0 \
4 5 2 1 3 \
4 7 2 1 3 \
5 0 0 0 32 0 0 0 0 0 0 0 0 \
5 2 2 0 0 \
5 4 1 0 1 \
5 6 1 0 1 \
6 3 2 0 0 \
6 5 2 0 0 \
7 4 2 1 0 "

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
   IndicatorOrientation eDirections;
   PRINT_FUNC;
   if( HEXCELLSLIB_OK != HexCellsLibCreate(&api, SIMPLE_GAME_DATA) )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellGetSpotType(api, 17, 12, &eType) || eType != Indicator )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetIndicatorDirection(api, 17, 12, &eDirections) || eDirections != IO_Top )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetIndicatorValue(api, 17, 12, IO_Top, &nValue, &eDetails) || nValue != 2 || eDetails != HasNumber )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellGetSpotType(api, 12, 15, &eType) || eType != Indicator )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetIndicatorDirection(api, 12, 15, &eDirections) || eDirections != IO_TopLeft )
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
   HexCellsLib api;
   PRINT_FUNC;
   if( HEXCELLSLIB_OK != HexCellsLibCreate(&api, NEW_GAME_DATA) )
      return TEST_FAILED;

   //if( 5 != HexCellsGetBombsRemaining(api) )
      //return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 7, 4, &nValue, &eDetails) || nValue != 0 || eDetails != HasNumber )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 4, 5, &nValue, &eDetails) || nValue != 3 || eDetails != NotConsecutive )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsGetRevealedSpotValue(api, 4, 7, &nValue, &eDetails) || nValue != 2 || eDetails != NotConsecutive )
      return TEST_FAILED;

   if( HEXCELLSLIB_OK != HexCellsLibFree(&api) )
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
   TestLoadNewData
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


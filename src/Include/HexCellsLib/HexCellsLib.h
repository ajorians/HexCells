#ifndef HEXCELLSLIB_H_INCLUDED
#define HEXCELLSLIB_H_INCLUDED

typedef void* HexCellsLib;

#define HEXCELLSLIB_OK              (0)
#define HEXCELLSLIB_BADARGUMENT     (-1)
#define HEXCELLSLIB_OUT_OF_MEMORY   (-2)

#define HEXCELLS_NOT_REVEALED       (0)
#define HEXCELLS_IS_REVEALED        (1)

#define HEXCELLS_NOT_GAMEOVER       (0)
#define HEXCELLS_IS_GAMEOVER        (1)

#define HEXCELLS_REVEAL_NOT_BOMB      (0)
#define HEXCELLS_REVEAL_BOMB          (1)

#define HEXCELLS_INCORRECT          (0)
#define HEXCELLS_CORRECT            (1)

typedef enum {
   Indicator,
   Bomb,
   NotBomb,
   Unknown,//If not revealed
   Nothing
} SpotType;

typedef enum {
   HasNumber,//Also used with bombs and their 2x radius
   Question,//Also used with bombs to mean normal bomb
   Consecutive,
   NotConsecutive
} ValueDetails;

typedef enum {
   IO_Left        = 0x1 << 7,
   IO_TopLeft     = 0x1 << 6,
   IO_Top         = 0x1 << 5,
   IO_TopRight    = 0x1 << 4,
   IO_Right       = 0x1 << 3,
   IO_BottomRight = 0x1 << 2,
   IO_Bottom      = 0x1 << 1,
   IO_BottomLeft  = 0x1 << 0
} IndicatorOrientation;

//////////////////////////////////////////////
//Initalization/Error checking/Mode functions
//////////////////////////////////////////////
int HexCellsLibCreate(HexCellsLib* api, const char* pstrGameData);
int HexCellsLibFree(HexCellsLib* api);

int GetHexCellsLibError(HexCellsLib api);
void ClearHexCellsLibError(HexCellsLib api);

int HexCellsSave(HexCellsLib api, char* pstr);

//////////////////////////////////////////////
//HexCellsLib related functions
//////////////////////////////////////////////
int HexCellsIsGameOver(HexCellsLib api);
int HexCellsGetWidth(HexCellsLib api);
int HexCellsGetHeight(HexCellsLib api);
int HexCellsGetBombsRemaining(HexCellsLib api);
int HexCellGetSpotType(HexCellsLib api, int nX, int nY, SpotType* peType);
int HexCellsGetRevealedSpotValue(HexCellsLib api, int nX, int nY, int* pnValue, ValueDetails* peDetails);
int HexCellsGetIndicatorDirection(HexCellsLib api, int nX, int nY, IndicatorOrientation* peDirection);
int HexCellsGetIndicatorValue(HexCellsLib api, int nX, int nY, IndicatorOrientation eDirection, int* pnValue, ValueDetails* peDetails);

int HexCellsIsRevealedSpot(HexCellsLib api, int nX, int nY);
int HexCellsRevealAs(HexCellsLib api, int nX, int nY, int nAsBomb);

int HexCellsUnknownRevealAs(HexCellsLib api, int nX, int nY, int nAsBomb, ValueDetails eDetails, int nValue);


#endif

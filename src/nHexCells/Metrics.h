#ifndef METRICS_H
#define METRICS_H

extern "C"
{
#ifdef _TINSPIRE
#include <os.h>
#endif
}

class Metrics
{
public:
   Metrics();
   void SetBoardDimensions(int nWidth, int nHeight);
   int GetLeftForPiece(int nX, int nY) const;
   int GetTopForPiece(int nX, int nY) const;
   int GetPieceSize() const;

   static short GetGapAmount() { return 3; }

protected:
   int m_nSizePerPiece;
   int m_nTop;
   int m_nLeft;
};

#endif


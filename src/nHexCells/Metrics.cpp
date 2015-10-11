#include "Metrics.h"
#include "Defines.h"

Metrics::Metrics()
{
}

void Metrics::SetBoardDimensions(int nWidth, int nHeight)
{
   const short nSmallAmount = GetGapAmount();

   int nAvailableWidth = SCREEN_WIDTH - (nWidth-1)*nSmallAmount;
   int nAvailableHeight = SCREEN_HEIGHT - (nHeight-1)*nSmallAmount;

   double dWidth = (nWidth - 2) * 0.75 + 2;
   double dHeight = nHeight * 0.5;

   int nWidthPerPiece = (int)(nAvailableWidth / dWidth);
   int nHeightPerPiece = (int)(nAvailableHeight / dHeight);

   m_nSizePerPiece = min(nWidthPerPiece, nHeightPerPiece);

   m_nLeft = (SCREEN_WIDTH - (int)(dWidth * m_nSizePerPiece + (nWidth-1)*nSmallAmount))/2;
   m_nTop = (SCREEN_HEIGHT - (int)(dHeight * m_nSizePerPiece + (nHeight-1)*nSmallAmount))/2;
}

int Metrics::GetLeftForPiece(int nX, int nY) const
{
   const short nSmallAmount = GetGapAmount();

   return m_nLeft + nX*(3*m_nSizePerPiece/4) + nX*nSmallAmount;
}

int Metrics::GetTopForPiece(int nX, int nY) const
{
   const short nSmallAmount = GetGapAmount();

   return m_nTop + nY*(m_nSizePerPiece/2) + ((nY+1)/2)*nSmallAmount;
}

int Metrics::GetPieceSize() const
{
   return m_nSizePerPiece;
}


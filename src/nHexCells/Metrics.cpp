#include "Metrics.h"
#include "Defines.h"

Metrics::Metrics()
{
}

void Metrics::SetBoardDimensions(int nWidth, int nHeight)
{
   int nWidthPerPiece = SCREEN_WIDTH / nWidth;
   int nHeightPerPiece = SCREEN_HEIGHT / nHeight;

   m_nSizePerPiece = min(nWidthPerPiece, nHeightPerPiece);

   m_nLeft = (SCREEN_WIDTH - (nWidth * m_nSizePerPiece))/2;
   m_nTop = (SCREEN_HEIGHT - (nHeight * m_nSizePerPiece))/2;
}

int Metrics::GetLeftForPiece(int nX, int nY) const
{
   return m_nLeft + nX*m_nSizePerPiece;
}

int Metrics::GetTopForPiece(int nX, int nY) const
{
   return m_nTop + nY*m_nSizePerPiece;
}

int Metrics::GetPieceSize() const
{
   return m_nSizePerPiece;
}


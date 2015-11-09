#include "stdafx.h"
#include "HexCellBoardImageAnalyzer.h"
#include <gdiplus.h>

using namespace Gdiplus;

#ifndef ARR_SIZE
#define ARR_SIZE(x) sizeof(x)/sizeof(x[0])
#endif

HexCellBoardImageAnalyzer::HexCellBoardImageAnalyzer(Gdiplus::Bitmap& image)
   : m_image(image)
{
}

inline void GetPixelRGB(UINT* pixels, int nStride, int x, int y, BYTE& r, BYTE& g, BYTE& b)
{
   UINT pixel = pixels[y * nStride / 4 + x];
   r = (pixel & 0xFF0000) >> 16;
   g = (pixel & 0x00FF00) >> 8;
   b = (pixel & 0x0000FF);
}


bool HexCellBoardImageAnalyzer::TestHexagon(UINT* pixels, int nStride, int nTopLeftX, int nTopLeftY)
{
   BYTE rOrig, gOrig, bOrig;
   GetPixelRGB(pixels, nStride, nTopLeftX, nTopLeftY, rOrig, gOrig, bOrig);

   HexagonImg::SpotType eType;

   if( rOrig == 255 && gOrig == 159 && bOrig == 0 ){//Unknown
      eType = HexagonImg::Unknown;
   }
   else if( rOrig == 44  && gOrig == 47  && bOrig == 49 ){//Revealed number
      eType = HexagonImg::Revealed;
   }
   else if( rOrig == 20  && gOrig == 156 && bOrig == 216 ){//Bomb spot
      eType = HexagonImg::Bomb;
   }
   else {
      return false;
   }

   //Check if can top top/left with same color and if so fail this
   {
      BYTE r,g,b;
      GetPixelRGB(pixels, nStride, nTopLeftX-1, nTopLeftY, r, g, b);
      if( r == rOrig && g == gOrig && b == bOrig ) {
         return false;
      }

      GetPixelRGB(pixels, nStride, nTopLeftX, nTopLeftY-1, r, g, b);
      if( r == rOrig && g == gOrig && b == bOrig ) {
         return false;
      }

      GetPixelRGB(pixels, nStride, nTopLeftX+1, nTopLeftY-1, r, g, b);
      if( r == rOrig && g == gOrig && b == bOrig ) {
         return false;
      }

      GetPixelRGB(pixels, nStride, nTopLeftX-1, nTopLeftY-1, r, g, b);
      if( r == rOrig && g == gOrig && b == bOrig ) {
         return false;
      }
   }

   int x, y;
   //     ____
   //    /    \
   //   /      \
   //   \      /
   //    \____/
   CPoint ptTopLeft, ptTopRight, ptRightMost, ptBottomRight, ptBottomLeft, ptLeftMost;
   ptTopLeft = {nTopLeftX, nTopLeftY};

   //Figure where the top ends
   x = nTopLeftX;
   y = nTopLeftY;
   while(true) {
      BYTE r,g,b;
      GetPixelRGB(pixels, nStride, x+1, y, r, g, b);
      if( r == rOrig && g == gOrig && b == bOrig ) {
         x++;
         continue;
      }

      //x,y is where it ends
      ptTopRight = {x, y};
      break;
   }

   //Upper-Right side
   x = ptTopRight.x;
   y = ptTopRight.y;
   while(true) {
      BYTE r,g,b;
      GetPixelRGB(pixels, nStride, x+1, y, r, g, b);
      if( r == rOrig && g == gOrig && b == bOrig ) {
         x++;
         continue;
      }
      GetPixelRGB(pixels, nStride, x, y+1, r, g, b);
      if( r == rOrig && g == gOrig && b == bOrig ) {
         y++;
         continue;
      }

      //x,y is where it ends
      ptRightMost = {x, y};
      break;
   }

   //Lower-Right side
   x = ptRightMost.x;
   y = ptRightMost.y;
   while(true) {
      BYTE r,g,b;
      GetPixelRGB(pixels, nStride, x, y+1, r, g, b);
      if( r == rOrig && g == gOrig && b == bOrig ) {
         y++;
         continue;
      }
      GetPixelRGB(pixels, nStride, x-1, y, r, g, b);
      if( (x-1) >= ptTopRight.x && r == rOrig && g == gOrig && b == bOrig ) {
         x--;
         continue;
      }

      //x,y is where it ends
      ptBottomRight = {x, y};
      break;
   }

   //Bottom side
   x = ptBottomRight.x;
   y = ptBottomRight.y;
   while(true) {
      BYTE r,g,b;
      GetPixelRGB(pixels, nStride, x-1, y, r, g, b);
      if( r == rOrig && g == gOrig && b == bOrig ) {
         x--;
         continue;
      }

      //x,y is where it ends
      ptBottomLeft = {x, y};
      break;
   }

   //Lower-Left side
   x = ptBottomLeft.x;
   y = ptBottomLeft.y;
   while(true) {
      BYTE r,g,b;
      GetPixelRGB(pixels, nStride, x-1, y, r, g, b);
      if( r == rOrig && g == gOrig && b == bOrig ) {
         x--;
         continue;
      }
      GetPixelRGB(pixels, nStride, x, y-1, r, g, b);
      if( r == rOrig && g == gOrig && b == bOrig ) {
         y--;
         continue;
      }

      //x,y is where it ends
      ptLeftMost = {x, y};
      break;
   }

   //Upper-Left side
   x = ptLeftMost.x;
   y = ptLeftMost.y;
   while(true) {
      BYTE r,g,b;
      GetPixelRGB(pixels, nStride, x, y-1, r, g, b);
      if( r == rOrig && g == gOrig && b == bOrig ) {
         y--;
         continue;
      }
      GetPixelRGB(pixels, nStride, x+1, y, r, g, b);
      if( (x+1) <= ptTopLeft.x && r == rOrig && g == gOrig && b == bOrig ) {
         x++;
         continue;
      }

      //x,y is where it ends
      //Compare this to ptTopLeft
      if( x == ptTopLeft.x && y == ptTopLeft.y ) {
         //Yay
         m_arrHexagons.push_back( { ptTopLeft, ptTopRight, ptRightMost, ptBottomRight, ptBottomLeft, ptLeftMost, eType } );
         return true;
      }
      break;
   }

   return false;
}

bool HexCellBoardImageAnalyzer::TestIndicator(UINT* pixels, int nStride, int nTopLeftX, int nTopLeftY)
{
   const int nLower = 70;
   const int nHigher = 100;

   BYTE rOrig, gOrig, bOrig;
   GetPixelRGB(pixels, nStride, nTopLeftX, nTopLeftY, rOrig, gOrig, bOrig);

   if( rOrig >= nLower && rOrig <= nHigher && gOrig >= nLower && gOrig <= nHigher && bOrig >= nLower && bOrig <= nHigher ) {}
   else
      return false;

   //Find all the pixels that match in this rectangle
   CRect rectBound = { nTopLeftX - 11, nTopLeftY - 11, nTopLeftX + 11, nTopLeftY + 11 };

#if 1
   //Shorten Top
   while(true) {
      int y = rectBound.top;
      bool bFoundAnyMatches = false;
      for(int x=rectBound.left; x<= rectBound.right; x++) {
         BYTE r,g,b;
         GetPixelRGB(pixels, nStride, x, y, r, g, b);

         if( r >= nLower && r <= nHigher && g >= nLower && g <= nHigher && b >= nLower && b <= nHigher ) {
            bFoundAnyMatches = true;
         }
      }

      if( bFoundAnyMatches )
         break;

      rectBound.top++;
   }

   //Shorten Bottom
   while(true) {
      int y = rectBound.bottom;
      bool bFoundAnyMatches = false;
      for(int x=rectBound.left; x<= rectBound.right; x++) {
         BYTE r,g,b;
         GetPixelRGB(pixels, nStride, x, y, r, g, b);

         if( r >= nLower && r <= nHigher && g >= nLower && g <= nHigher && b >= nLower && b <= nHigher ) {
            bFoundAnyMatches = true;
         }
      }

      if( bFoundAnyMatches )
         break;

      rectBound.bottom--;
   }

   //Shorten Left
   while(true) {
      int x = rectBound.left;
      bool bFoundAnyMatches = false;
      for(int y=rectBound.top; y<= rectBound.bottom; y++) {
         BYTE r,g,b;
         GetPixelRGB(pixels, nStride, x, y, r, g, b);

         if( r >= nLower && r <= nHigher && g >= nLower && g <= nHigher && b >= nLower && b <= nHigher ) {
            bFoundAnyMatches = true;
         }
      }

      if( bFoundAnyMatches )
         break;

      rectBound.left++;
   }

   //Shorten Right
   while(true) {
      int x = rectBound.right;
      bool bFoundAnyMatches = false;
      for(int y=rectBound.top; y<= rectBound.bottom; y++) {
         BYTE r,g,b;
         GetPixelRGB(pixels, nStride, x, y, r, g, b);

         if( r >= nLower && r <= nHigher && g >= nLower && g <= nHigher && b >= nLower && b <= nHigher ) {
            bFoundAnyMatches = true;
         }
      }

      if( bFoundAnyMatches )
         break;

      rectBound.right--;
   }
#endif

   

   if( rectBound.left >= rectBound.right || rectBound.top >= rectBound.bottom )
      return false;

   if( rectBound.Width() > 11 || rectBound.Height() > 11 ) {
      return false;
   }

   if( rectBound.Width() < 2 || rectBound.Height() < 3 ) {
      return false;
   }

   CPoint arrPoints[] = {
      {rectBound.left,   rectBound.top},
      {rectBound.right,  rectBound.top},
      {rectBound.left,   rectBound.bottom},
      {rectBound.right,  rectBound.bottom}
   };
   //Check to see if too close to existing indicator
   //const int nCloseHoriz = 6;
   //const int nCloseVert = 10;
   for(auto indicator : m_arrIndicators) {
      RECT r = indicator.GetRect();
      for(int i=0; i<ARR_SIZE(arrPoints); i++) {
         //Check if r contains any of these points
         if( r.left <= arrPoints[i].x && r.right >= arrPoints[i].x &&
             r.top <= arrPoints[i].y && r.bottom >= arrPoints[i].y )
            return false;
      }
   }

   m_arrIndicators.push_back(rectBound);

   return true;
}

bool HexCellBoardImageAnalyzer::Analyze()
{
   m_arrHexagons.clear();
   m_arrIndicators.clear();
   BitmapData* bitmapData = new BitmapData;
   UINT nWidth = m_image.GetWidth(), nHeight = m_image.GetHeight();
   Gdiplus::Rect rect = {0, 0, (INT)nWidth, (INT)nHeight};

   m_image.LockBits(
      &rect,
      ImageLockModeRead,
      PixelFormat32bppARGB,
      bitmapData);

   UINT* pixels = (UINT*)bitmapData->Scan0;

   bool bPartOfChain = false;
   int nTopLeftX = 0, nTopLeftY = 0;
   for(UINT row = 0; row < nHeight; ++row)
   {
      for(UINT col = 0; col < nWidth; ++col)
      {
         BYTE r,g,b;
         GetPixelRGB(pixels, bitmapData->Stride, col, row, r, g, b);

         if(  (r == 255 && g == 159 && b == 0  )//Unknown
            ||(r == 44  && g == 47  && b == 49 )//Revealed number
            ||(r == 20  && g == 156 && b == 216))//Bomb spot
         {
            if( !bPartOfChain ) {
               if( TestHexagon(pixels, bitmapData->Stride, col, row) ) {
                  bPartOfChain = true;
               }
            }
         }
         else {
            bPartOfChain = false;
         }

         TestIndicator(pixels, bitmapData->Stride, col, row);
      }
   }

   m_image.UnlockBits(bitmapData);

   delete bitmapData;
   return false;
}

int HexCellBoardImageAnalyzer::GetNumberOfHexagons() const
{
   return (int)m_arrHexagons.size();
}

int HexCellBoardImageAnalyzer::GetNumberOfIndicators() const
{
   return (int)m_arrIndicators.size();
}

RECT HexCellBoardImageAnalyzer::GetIndicatorRect(int nIndex) const
{
   return m_arrIndicators[nIndex].GetRect();
}



#pragma once

#include <vector>
#include <utility>

namespace Gdiplus
{
   class Image;
}

class HexagonImg
{
public:
   enum SpotType{
      Bomb,
      Unknown,
      Revealed
   };
public:
   HexagonImg(const CPoint& ptTopLeft, const CPoint& ptTopRight, const CPoint& ptRightMost, 
      const CPoint& ptBottomRight, const CPoint& ptBottomLeft, const CPoint& ptLeftMost, SpotType eType)
      : m_ptTopLeft(ptTopLeft), m_ptTopRight(ptTopRight), m_ptRightMost(ptRightMost),
        m_ptBottomRight(ptBottomRight), m_ptBottomLeft(ptBottomLeft), m_ptLeftMost(ptLeftMost),
        m_eType(eType)
   {
   }

protected:
   CPoint m_ptTopLeft, m_ptTopRight, m_ptRightMost, m_ptBottomRight, m_ptBottomLeft, m_ptLeftMost;
   SpotType m_eType;
};

class IndicatorImg
{
public:
   IndicatorImg(RECT rect)
      : m_rect(rect) {}

   RECT GetRect() const { return m_rect; }

protected:
   RECT m_rect;
};

class HexCellBoardImageAnalyzer
{
public:
   HexCellBoardImageAnalyzer(Gdiplus::Bitmap& image);

   bool Analyze();
   int GetNumberOfHexagons() const;
   int GetNumberOfIndicators() const;

   RECT GetIndicatorRect(int nIndex) const;

protected:
   bool TestHexagon(UINT* pixels, int nStride, int nTopLeftX, int nTopLeftY);
   bool TestIndicator(UINT* pixels, int nStride, int nTopLeftX, int nTopLeftY);

protected:
   Gdiplus::Bitmap& m_image;
   std::vector<HexagonImg> m_arrHexagons;
   std::vector<IndicatorImg> m_arrIndicators;
};

#include "stdafx.h"
#include "SelectorWindow.h"
#include "ISelectorWnd.h"

BEGIN_MESSAGE_MAP(SelectorWindow, CWnd)
   ON_WM_PAINT()
   ON_WM_ERASEBKGND()
   ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

SelectorWindow::SelectorWindow(CWnd* pParent, ISelectorWnd* pSelectorParent)
   : m_rectWindow(0, 0, 800, 600), m_pSelectorParent(pSelectorParent)
{
   Create(NULL, _T("SelectorWindow"), WS_OVERLAPPED | WS_VISIBLE, m_rectWindow, CWnd::GetDesktopWindow(), 0 );
   ::SetWindowPos( this->GetSafeHwnd(), HWND_TOPMOST, 0, 0 , 0 , 0, SWP_NOMOVE | SWP_NOSIZE );
}

void SelectorWindow::OnPaint() 
{
   CPaintDC dc(this);

   CDC MemDC;
   if (!MemDC.CreateCompatibleDC(&dc))
      return;

   // Get Size of Display area
   CRect rect;
   GetClientRect(rect);

#if 0
   CBrush brush(RGB(255,0,0));

   dc.FillRect(rect, &brush);
#endif

   dc.DrawEdge(&rect, BDR_SUNKENINNER, BF_RECT);

   return CWnd::OnPaint();
}

BOOL SelectorWindow::OnEraseBkgnd(CDC* pDC) 
{
    return CWnd::OnEraseBkgnd(pDC);
}

void SelectorWindow::OnMouseMove(UINT nFlags, CPoint point)
{
   if( (nFlags & MK_LBUTTON) == MK_LBUTTON ) {
      ClientToScreen(&point);
      m_rectWindow.MoveToXY(point.x-m_rectWindow.Width()/2, point.y-m_rectWindow.Height()/2);
      MoveWindow(&m_rectWindow, FALSE);
      m_pSelectorParent->WindowMoved();
      return;
   }
   return __super::OnMouseMove(nFlags, point);
}


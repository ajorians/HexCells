#pragma once

class ISelectorWnd;

class SelectorWindow : public CWnd
{
public:
   SelectorWindow(CWnd* pParent, ISelectorWnd* pSelectorParent);

protected:
   DECLARE_MESSAGE_MAP();
   afx_msg void OnPaint( );
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);

protected:
   CRect m_rectWindow;
   ISelectorWnd* m_pSelectorParent;
};

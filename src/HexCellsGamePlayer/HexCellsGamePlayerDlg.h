
// HexCellsGamePlayerDlg.h : header file
//

#pragma once

#include "ISelectorWnd.h"
#include "SelectorWindow.h"

class SelectorWindow;

// CHexCellsGamePlayerDlg dialog
class CHexCellsGamePlayerDlg : public CDialogEx, public ISelectorWnd
{
// Construction
public:
	CHexCellsGamePlayerDlg(CWnd* pParent = NULL);	// standard constructor
   ~CHexCellsGamePlayerDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HEXCELLSGAMEPLAYER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

   SelectorWindow m_SelectorWindow;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedBtnTakePicture();

   // Inherited via ISelectorWnd
   virtual void WindowMoved() override;
};

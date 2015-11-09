
// HexCellsGamePlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HexCellsGamePlayer.h"
#include "HexCellsGamePlayerDlg.h"
#include "afxdialogex.h"
#include "SelectorWindow.h"
#include "HexCellBoardImageAnalyzer.h"
#include <gdiplus.h>
using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHexCellsGamePlayerDlg dialog


ULONG_PTR gdiplusToken;
CHexCellsGamePlayerDlg::CHexCellsGamePlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HEXCELLSGAMEPLAYER_DIALOG, pParent), m_SelectorWindow(this, this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

   GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

CHexCellsGamePlayerDlg::~CHexCellsGamePlayerDlg()
{
   GdiplusShutdown(gdiplusToken);
}

void CHexCellsGamePlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHexCellsGamePlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
   ON_BN_CLICKED(IDC_BTN_TAKE_PICTURE, &CHexCellsGamePlayerDlg::OnBnClickedBtnTakePicture)
END_MESSAGE_MAP()


// CHexCellsGamePlayerDlg message handlers

BOOL CHexCellsGamePlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

   WindowMoved();
   ::SetWindowPos( this->GetSafeHwnd(), HWND_TOPMOST, 0, 0 , 0 , 0, SWP_NOMOVE | SWP_NOSIZE );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHexCellsGamePlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHexCellsGamePlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHexCellsGamePlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }
   }
   free(pImageCodecInfo);
   return -1;  // Failure
}

//#define USE_TESTING_IMAGE

void CHexCellsGamePlayerDlg::OnBnClickedBtnTakePicture()
{
#ifdef USE_TESTING_IMAGE
   Bitmap* pBitmap = Bitmap::FromFile(L"Example.png");
   HexCellBoardImageAnalyzer analyzer(*pBitmap);

#else
   CRect rectSelector;
   m_SelectorWindow.GetWindowRect(&rectSelector);

   HDC hDCScreen = ::GetDC(NULL);

   HDC hDCMem = CreateCompatibleDC(hDCScreen);

   HBITMAP hBitmap = CreateCompatibleBitmap(hDCScreen, rectSelector.Width(), rectSelector.Height());

   SelectObject(hDCMem, hBitmap);

   ::BitBlt(hDCMem, 0, 0, rectSelector.Width(), rectSelector.Height(), hDCScreen, rectSelector.left, rectSelector.top, SRCCOPY);

   Bitmap image(hBitmap, NULL);

#ifdef _DEBUG

    CLSID myClsId;
    int retVal = GetEncoderClsid(L"image/png", &myClsId);

    image.Save(L"output.png", &myClsId, NULL);
#endif

    HexCellBoardImageAnalyzer analyzer(image);
#endif

    analyzer.Analyze();

    SetDlgItemInt(IDC_STATIC_HEXAGONS_FOUND, analyzer.GetNumberOfHexagons());
    GetDlgItem(IDC_STATIC_HEXAGONS_FOUND)->ShowWindow(SW_SHOW);

    SetDlgItemInt(IDC_STATIC_INDICATORS_FOUND, analyzer.GetNumberOfIndicators());
    GetDlgItem(IDC_STATIC_INDICATORS_FOUND)->ShowWindow(SW_SHOW);

    //m_SelectorWindow.Invalidate();

#ifdef USE_TESTING_IMAGE
    delete pBitmap;
#endif
}

void CHexCellsGamePlayerDlg::WindowMoved()
{
   CRect rectSelector;
   m_SelectorWindow.GetWindowRect(&rectSelector);

   CRect rect;
   this->GetWindowRect(&rect);
   rect.MoveToXY(rectSelector.left + (rectSelector.Width() - rect.Width())/2, rectSelector.bottom);
   MoveWindow(&rect, FALSE);
}

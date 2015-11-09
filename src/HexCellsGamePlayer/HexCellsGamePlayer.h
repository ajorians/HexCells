
// HexCellsGamePlayer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CHexCellsGamePlayerApp:
// See HexCellsGamePlayer.cpp for the implementation of this class
//

class CHexCellsGamePlayerApp : public CWinApp
{
public:
	CHexCellsGamePlayerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CHexCellsGamePlayerApp theApp;
#pragma once

#include "..\\build\\win32\\resource.h"
#include <afxwin.h>

class CCrackApp : public CWinApp
{
public:
	CCrackApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrackApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCrackApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#pragma once

#include <afxctl.h>
#include "utils.h"

class CCrackDlg : public CDialog
{
public:
	CCrackDlg(CWnd* pParent = NULL);
	int DoCrack();
	CrackContext m_ctx;

	//{{AFX_DATA(CCrackDlg)
	enum { IDD = IDD_CrackDlg };
	HANDLE m_hThread;
	CString	m_edtInputFileName;
	CString	m_edtDictFileName;
	CString	m_txtCrackRate;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCrackDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCrackDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnbtnOpenDictFile();
	afx_msg void OnbtnOpenInputFile();
	afx_msg void OnbtnRemove();
	afx_msg void OnbtnStart();
	afx_msg void OnbtnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

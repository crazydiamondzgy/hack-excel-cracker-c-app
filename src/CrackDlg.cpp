#include <afxdlgs.h>
#include "Crack.h"
#include "CrackDlg.h"
#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCrackDlg::CCrackDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCrackDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCrackDlg)
	m_edtInputFileName = _T("");
	m_edtDictFileName = _T("");
	m_txtCrackRate = _T("");
	//}}AFX_DATA_INIT
}

void CCrackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCrackDlg)
	DDX_Text(pDX, IDC_edtInputFileName, m_edtInputFileName);
	DDX_Text(pDX, IDC_edtDictFileName,  m_edtDictFileName);
	DDX_Text(pDX, IDC_RATE, m_txtCrackRate);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCrackDlg, CDialog)
	//{{AFX_MSG_MAP(CCrackDlg)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_btnOpenDictFile,  OnbtnOpenDictFile)
	ON_BN_CLICKED(IDC_btnOpenInputFile, OnbtnOpenInputFile)
	ON_BN_CLICKED(IDC_btnRemove,        OnbtnRemove)
	ON_BN_CLICKED(IDC_btnStart,         OnbtnStart)
	ON_BN_CLICKED(IDC_btnStop,          OnbtnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCrackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
#ifdef MODE_WORD
	matrix_crack_register(&word_cracker);
#endif
	
#ifdef MODE_EXCEL
	matrix_crack_register(&excel_cracker);
#endif

#ifdef MODE_ACCESS
	matrix_crack_register(&access_cracker);
#endif

#ifdef MODE_OUTLOOK
	matrix_crack_register(&outlook_cracker);
#endif

#ifdef MODE_ZIP
	matrix_crack_register(&zip_cracker);
#endif
	
#ifdef MODE_RAR
	matrix_crack_register(&rar_cracker);
#endif

#ifdef MODE_PDF
	matrix_crack_register(&pdf_cracker);
#endif

	//
	// init data
	//

	ZeroMemory(&m_ctx, sizeof(m_ctx));
	UpdateData(false);
		
	return TRUE;
}

HRESULT do_crack(LPVOID p)
{
	CCrackDlg *pCrackDlg = (CCrackDlg *)p;
	return pCrackDlg->DoCrack();
}

int CCrackDlg::DoCrack()
{
	if (matrix_crack_open(&m_ctx, m_edtInputFileName.GetBuffer(0)) < 0)
	{
		return -1;
	}

	int ret = matrix_crack(&m_ctx);
	KillTimer(1);
	if (ret)
	{
		MessageBox(m_ctx.pw, _T("Found password:"), MB_OK);
	}

	matrix_crack_close(&m_ctx);
	
	return 0;
}

void CCrackDlg::OnTimer(UINT nIDEvent) 
{
	static int s = 0;
	m_txtCrackRate.Format("%d", m_ctx.pw_num/++s);
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}

int CCrackDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	return 0;
}

void CCrackDlg::OnbtnOpenDictFile() 
{
	CFileDialog dlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "dictionary file(*.txt)|*.txt|all file(*.*)|*.*||");
	if (dlg.DoModal() == IDOK)
	{
		m_edtDictFileName = dlg.GetPathName();
		m_ctx.dict_filename = m_edtDictFileName.GetBuffer(05);
		UpdateData(FALSE);
	}
}

void CCrackDlg::OnbtnOpenInputFile() 
{
	CFileDialog dlg(true,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
#if defined MODE_RAR
	 "rar文件(*.rar)|*.rar|all files(*.*)|*.*||");
#elif defined MODE_WORD
	"word 文件(*.doc)|*.doc|all files(*.*)|*.*||");
#elif defined MODE_EXCEL
	"excel 文件(*.xls)|*.xls|所有文件(*.*)|*.*||");
#elif defined MODE_OUTLOOK
	"outlook 文件(*.pst)|*.pst|所有文件(*.*)|*.*||");
#elif defined MODE_ACCESS
	"access 文件(*.mdb)|*.mdb|所有文件(*.*)|*.*||");
#elif defined MODE_ZIP
	"zip文件(*.zip)|*.zip|所有文件(*.*)|*.*||");
#elif defined MODE_PDF
	"pdf文件(*.pdf)|*.pdf|所有文件(*.*)|*.*||");
#else
# error "please indicate a crack mode"
#endif
	dlg.DoModal();
	m_edtInputFileName = dlg.GetPathName();
	UpdateData(FALSE);
}

void CCrackDlg::OnbtnRemove() 
{
#if defined MODE_OFFICE
	char str[4];
	strcpy(str, &filepath[strlen(filepath)-3]);
	
	if (strncmp(str, "doc", 4) == 0)
	{
		if (remove_word() == 0)
			MessageBox("写保护密码成功清除，保存到文件foo.doc");}
	else if (strncmp(str, "xls", 4) == 0)
	{
		if (remove_excel() == 0)
			MessageBox("写保护密码成功清除，保存到文件foo.xls");
	}
#endif
}

void CCrackDlg::OnbtnStart() 
{
	if(m_hThread)
	{
		TerminateThread(m_hThread, 0);
	}
	
	m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)do_crack, this, 0, NULL);
	KillTimer(1);
	SetTimer(1, 1000, NULL);
}

void CCrackDlg::OnbtnStop() 
{
}

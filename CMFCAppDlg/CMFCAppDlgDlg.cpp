
// CMFCAppDlgDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CMFCAppDlg.h"
#include "CMFCAppDlgDlg.h"
#include "afxdialogex.h"

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


// CCMFCAppDlgDlg dialog

// Forward declarations of the DLL functions
extern "C" {
	__declspec(dllimport) void* CreateReportViewer();
	__declspec(dllimport) void DestroyReportViewer(void* instance);
	__declspec(dllimport) void InitializeReportViewer(void* instance, HWND hwndParent);
	__declspec(dllimport) void LoadReport(void* instance, const wchar_t* reportPath);
}

CCMFCAppDlgDlg::CCMFCAppDlgDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CMFCAPPDLG_DIALOG, pParent)
	, m_pReportViewerInstance(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCMFCAppDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLACEHOLDER, m_ctrlPlaceholder);
}

BEGIN_MESSAGE_MAP(CCMFCAppDlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CCMFCAppDlgDlg message handlers

BOOL CCMFCAppDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: Add extra initialization here

	// Create the ReportViewer instance
	m_pReportViewerInstance = CreateReportViewer();

	// Get the placeholder window handle
	CRect rect;
	if (m_ctrlPlaceholder.GetSafeHwnd() == nullptr) {
		AfxMessageBox(_T("Placeholder control not found!"));
		return FALSE;  // Ensure control is found
	}

	m_ctrlPlaceholder.GetWindowRect(&rect);
	ScreenToClient(&rect);

	// Initialize the .NET control
	InitializeReportViewer(m_pReportViewerInstance, m_ctrlPlaceholder.GetSafeHwnd());

	// Load the report
	CString reportPath = L"C:\\Users\\ggagnon\\source\\CrystalReportDisplay\\BlankReport.rpt";
	if (PathFileExists(reportPath)) {
		LoadReport(m_pReportViewerInstance, reportPath.GetString());
	}
	else {
		AfxMessageBox(_T("Report file not found!"));
		return FALSE;  // Ensure report file exists
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCMFCAppDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCMFCAppDlgDlg::OnPaint()
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
HCURSOR CCMFCAppDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


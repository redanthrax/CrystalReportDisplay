
// CMFCAppDlgDlg.h : header file
//

#pragma once

// CCMFCAppDlgDlg dialog
class CCMFCAppDlgDlg : public CDialogEx
{
// Construction
public:
	CCMFCAppDlgDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMFCAPPDLG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void* m_pReportViewerInstance;
	CWnd m_ctrlPlaceholder;
};

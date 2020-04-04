// TestTcpDlg.h : header file
//

#if !defined(AFX_SERVERDLG_H__8F6AF78C_0465_11D4_B84C_00A0C9C8257C__INCLUDED_)
#define AFX_SERVERDLG_H__8F6AF78C_0465_11D4_B84C_00A0C9C8257C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog


class CSrvSock;
class CHostSock;
class CClntSock;

typedef CArray<CClntSock*,CClntSock*> ClntSockArray;

class CServerDlg : public CDialog
{
// Construction
public:
	void GetSetProfile(BOOL bSave);
	BOOL ClientConnect();
	void CloseSockets();
	CServerDlg(CWnd* pParent = NULL);	// standard constructor
	~CServerDlg();	// standard constructor
	CSrvSock * m_pSrvSock;
	CHostSock * m_pHostSock;
	CPtrList m_ClntList;
  void ShowConnection(CClntSock *pSocket);
  void HideConnection(CSocket *pSocket);
  CString GetSendText();
  ClntSockArray* GetSelectedSockets();
  void RestoreWindPos(WINDOWPLACEMENT& wpl);

// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DIALOG };
	CStatic	c_ServerIcon;
	CStatic	c_ClientIcon;
	CEdit	c_ClientReceive;
	CEdit	c_ReceiveText;
	CListBox	c_ListBox;
	UINT	m_ServerPort;
	CString	m_SendText;
	CString	m_HostAddress;
	UINT	m_HostPort;
	CString	m_ClientSendText;
	CString	m_ServerAddress;
	CString	m_ReceiveText;
	CString	m_ClientReceive;
	CString	m_ResponceHeadr;
	BOOL	m_bAddHeadr;
	BOOL	m_bAddLength;
	UINT	m_LocalPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonCreateServerSock();
	afx_msg void OnButtonSend();
	afx_msg void OnConnectToProxi();
	afx_msg void OnButtonClientSend();
	afx_msg void OnClearServerRcv();
	afx_msg void OnClearClientRcv();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnButtonServerClose();
	afx_msg void OnButtonServerCloseConnection();
	afx_msg void OnButtonSendToSelcted();
	afx_msg void OnButtonClientClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__8F6AF78C_0465_11D4_B84C_00A0C9C8257C__INCLUDED_)

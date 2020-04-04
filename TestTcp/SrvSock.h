#if !defined(AFX_SRVSOCK_H__8F6AF794_0465_11D4_B84C_00A0C9C8257C__INCLUDED_)
#define AFX_SRVSOCK_H__8F6AF794_0465_11D4_B84C_00A0C9C8257C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrvSock.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CSrvSock command target
class CServerDlg;

class CSrvSock : public CSocket
{
// Attributes
public:

// Operations
public:
	CSrvSock();
	CSrvSock(CServerDlg* pDlg);
	CServerDlg* m_pDlg;
	virtual ~CSrvSock();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSrvSock)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSrvSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRVSOCK_H__8F6AF794_0465_11D4_B84C_00A0C9C8257C__INCLUDED_)

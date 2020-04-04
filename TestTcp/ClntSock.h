#if !defined(AFX_CLNTSOCK_H__8F6AF796_0465_11D4_B84C_00A0C9C8257C__INCLUDED_)
#define AFX_CLNTSOCK_H__8F6AF796_0465_11D4_B84C_00A0C9C8257C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClntSock.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CClntSock command target

class CClntSock : public CSocket
{
// Attributes
public:

// Operations
public:
	CClntSock();
	virtual ~CClntSock();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClntSock)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClntSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLNTSOCK_H__8F6AF796_0465_11D4_B84C_00A0C9C8257C__INCLUDED_)

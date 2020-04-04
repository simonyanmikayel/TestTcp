#if !defined(AFX_HOSTSOCK_H__8F6AF795_0465_11D4_B84C_00A0C9C8257C__INCLUDED_)
#define AFX_HOSTSOCK_H__8F6AF795_0465_11D4_B84C_00A0C9C8257C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HostSock.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CHostSock command target

class CHostSock : public CSocket
{
// Attributes
public:

// Operations
public:
	CHostSock();
	virtual ~CHostSock();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHostSock)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CHostSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOSTSOCK_H__8F6AF795_0465_11D4_B84C_00A0C9C8257C__INCLUDED_)

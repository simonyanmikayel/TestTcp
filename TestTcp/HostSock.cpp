// HostSock.cpp : implementation file
//

#include "stdafx.h"
#include "TestTcp.h"
#include "HostSock.h"
#include "ClntSock.h"
#include "TestTcpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHostSock
extern CTestTcpApp theApp;
extern CServerDlg* pDialog;
UINT ClntRcvCnt = 0;

CHostSock::CHostSock()
{
}

CHostSock::~CHostSock()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CHostSock, CSocket)
	//{{AFX_MSG_MAP(CHostSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CHostSock member functions


void CHostSock::OnClose(int nErrorCode) 
{
	pDialog->m_pHostSock = NULL;
	CSocket::OnClose(nErrorCode);
  pDialog->c_ClientIcon.SetIcon(theApp.LoadIcon(IDR_MAINFRAME) );
  pDialog->HideConnection(this);
}

void CHostSock::OnReceive(int nErrorCode) 
{
	TCHAR buff[4096];
	int nRead = Receive(buff, 4096);

  ClntRcvCnt++;
  CString str;str.Format("\r\n%u______________________\r\n",ClntRcvCnt);
  pDialog->m_ClientReceive += str;


  buff[nRead] = 0;
  pDialog->m_ClientReceive += buff;

  pDialog->c_ClientReceive.SetWindowText(pDialog->m_ClientReceive);
	CSocket::OnReceive(nErrorCode);
}

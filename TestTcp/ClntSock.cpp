// ClntSock.cpp : implementation file
//

#include "stdafx.h"
#include "TestTcp.h"
#include "TestTcpDlg.h"
#include "ClntSock.h"
#include "HostSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClntSock
UINT SrvrRcvCnt = 0;

extern CServerDlg* pDialog;

CClntSock::CClntSock()
{
}

CClntSock::~CClntSock()
{

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClntSock, CSocket)
	//{{AFX_MSG_MAP(CClntSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClntSock member functions

void CClntSock::OnReceive(int nErrorCode) 
{
	TCHAR buff[4096];

	int nRead = Receive(buff, 4096);

  SrvrRcvCnt++;
  CString str;
  str.Format("\r\n%u______________________\r\n",SrvrRcvCnt);
  pDialog->m_ReceiveText += str;

  buff[nRead] = 0;
  pDialog->m_ReceiveText += buff;


  pDialog->c_ReceiveText.SetWindowText(pDialog->m_ReceiveText);
	CSocket::OnReceive(nErrorCode);
}

void CClntSock::OnClose(int nErrorCode) 
{
  pDialog->HideConnection(this);

	POSITION pos = pDialog->m_ClntList.Find(this);
	if (pos != NULL)
		pDialog->m_ClntList.RemoveAt(pos);

	CSocket::OnClose(nErrorCode);
}


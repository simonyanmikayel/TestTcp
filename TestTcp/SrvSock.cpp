// SrvSock.cpp : implementation file
//

#include "stdafx.h"
#include "TestTcp.h"
#include "SrvSock.h"
#include "TestTcpDlg.h"
#include "ClntSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSrvSock
BOOL BindSock(CSocket* pSock,UINT nPort,LPCSTR strAddress);
BOOL ReBindSock(CSocket* pSock,UINT nPort,LPCSTR strAddress);

extern CServerDlg* pDialog;

CSrvSock::CSrvSock(CServerDlg* pDlg)
{
	m_pDlg = pDlg;
}

CSrvSock::CSrvSock()
{
}

CSrvSock::~CSrvSock()
{
}



// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSrvSock, CSocket)
	//{{AFX_MSG_MAP(CSrvSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSrvSock member functions

void CSrvSock::OnAccept(int nErrorCode) 
{
	CSocket::OnAccept(nErrorCode);
	CClntSock* pSocket = new CClntSock;

	if (Accept(*pSocket))
	{
		pDialog->m_ClntList.AddHead(pSocket);
    pDialog->ShowConnection(pSocket);
	}
	else
		delete pSocket;

}

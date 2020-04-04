// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestTcp.h"
#include "TestTcpDlg.h"

#include "SrvSock.h"
#include "ClntSock.h"
#include "HostSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
BOOL BindSock(CSocket* pSock,UINT nPort,LPCSTR strAddress);
BOOL ReBindSock(CSocket* pSock,UINT nPort,LPCSTR strAddress);
extern CTestTcpApp theApp;
CServerDlg* pDialog;
void ConvertCRLF(CString* str);

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog
CServerDlg::~CServerDlg()
{
	CloseSockets();
}

void CServerDlg::OnClose() 
{
	CDialog::OnClose();
}

void CServerDlg::OnDestroy() 
{
  GetSetProfile(TRUE);
	CDialog::OnDestroy();		
}

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
	m_ServerPort = 80;
	m_SendText = _T("");
	m_HostAddress = _T("127.0.0.1");
	m_HostPort = 80;
	m_ClientSendText = _T("");
	m_ServerAddress = _T("127.0.0.1");
	m_ReceiveText = _T("");
	m_ClientReceive = _T("");
	m_ResponceHeadr = _T("");
	m_bAddHeadr = FALSE;
	m_bAddLength = FALSE;
	m_LocalPort = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pSrvSock = NULL;
	m_pHostSock = NULL;
	m_ClntList.RemoveAll();
	pDialog = this;
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
	DDX_Control(pDX, IDC_SERVER_ICON, c_ServerIcon);
	DDX_Control(pDX, IDC_CLIENT_ICON, c_ClientIcon);
	DDX_Control(pDX, IDC_EDIT_CLIENT_RECIVED_TEXT, c_ClientReceive);
	DDX_Control(pDX, IDC_EDIT_RECEIVE_TEXT, c_ReceiveText);
	DDX_Control(pDX, IDC_LIST_CONNECTIONS, c_ListBox);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_ServerPort);
	DDX_Text(pDX, IDC_EDIT_SEND_TEXT, m_SendText);
	DDX_Text(pDX, IDC_EDIT_CONNECT_ADDR, m_HostAddress);
	DDX_Text(pDX, IDC_EDIT_CONNECT_PORT, m_HostPort);
	DDX_Text(pDX, IDC_EDIT_CLIENT_SEND_TEXT, m_ClientSendText);
	DDX_Text(pDX, IDC_EDIT_SERVER_ADDR, m_ServerAddress);
	DDX_Text(pDX, IDC_EDIT_RESPONCE_HEADR, m_ResponceHeadr);
	DDX_Check(pDX, IDC_CHECK_ADD_HEADR, m_bAddHeadr);
	DDX_Check(pDX, IDC_CHECK_ADD_LENGTH, m_bAddLength);
	DDX_Text(pDX, IDC_EDIT_LOCAL_PORT, m_LocalPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CREATE_SERVER_SOCK, OnButtonCreateServerSock)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_CONNECT_TO_PROXI, OnConnectToProxi)
	ON_BN_CLICKED(IDC_BUTTON_CLIENT_SEND, OnButtonClientSend)
	ON_BN_CLICKED(IDC_CLEAR_SERVER_RCV, OnClearServerRcv)
	ON_BN_CLICKED(IDC_CLEAR_CLIENT_RCV, OnClearClientRcv)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SERVER_CLOSE, OnButtonServerClose)
	ON_BN_CLICKED(IDC_BUTTON_SERVER_CLOSE_CONNECTION, OnButtonServerCloseConnection)
	ON_BN_CLICKED(IDC_BUTTON_SEND_TO_SELCTED, OnButtonSendToSelcted)
	ON_BN_CLICKED(IDC_BUTTON_CLIENT_CLOSE, OnButtonClientClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
  GetSetProfile(FALSE);
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	
	//OnButtonCreateServerSock() ;	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CServerDlg::OnButtonCreateServerSock() 
{
	SetIcon(theApp.LoadIcon(IDR_MAINFRAME),FALSE);
  c_ServerIcon.SetIcon(theApp.LoadIcon(IDR_MAINFRAME) );

	UpdateData();

	CloseSockets();

	m_pSrvSock = new CSrvSock(this);

	if (!BindSock(m_pSrvSock,m_ServerPort,m_ServerAddress))
		return;

	SetWindowText("");
	c_ReceiveText.SetWindowText("");
  c_ListBox.ResetContent();
	if (m_pSrvSock->Listen())
  {
	  SetIcon(theApp.LoadIcon(IDI_LISTEN),FALSE);
    c_ServerIcon.SetIcon(theApp.LoadIcon(IDI_LISTEN) );
  }
  else
  {
		int err = WSAGetLastError ();
		CString str;
		str.Format("Listen Error = %u",err);
    ::MessageBox(NULL,str,"",MB_OK);
  }
}


CString CServerDlg::GetSendText()
{
	UpdateData();
	CString str;

  if (m_bAddHeadr)
  {
    str = m_ResponceHeadr;
    str.TrimLeft();
    str.TrimRight();
    str += "\r\n";
    if (m_bAddLength)
    {
      CString strLength;
      strLength.Format("Content-Length: %u\r\n", m_SendText.GetLength());
      str += strLength;
    }
    str += "\r\n";
  }
	str += m_SendText;
  return str;
}

void CServerDlg::OnButtonSend() 
{
	if (m_pSrvSock == NULL)
	{
		AfxMessageBox("Server Socket Has not Created");
		return;
	}

  CString str = GetSendText();

	POSITION pos;
	for( pos = pDialog->m_ClntList.GetHeadPosition(); pos != NULL; )
	{
		CClntSock* pSocket = (CClntSock*)pDialog->m_ClntList.GetAt(pos);
		pSocket->Send(str,str.GetLength());
		pDialog->m_ClntList.GetNext( pos );
	}
}

void CServerDlg::OnButtonSendToSelcted() 
{
	if (m_pSrvSock == NULL)
	{
		AfxMessageBox("Server Socket Has not Created");
		return;
	}

  CString str = GetSendText();

  ClntSockArray* pArr = GetSelectedSockets();
  for (int i = 0; i < pArr->GetSize(); i++)
  {
    CClntSock* pSocket = pArr->GetAt(i);
		pSocket->Send(str,str.GetLength());
  }
  delete pArr;
}

void CServerDlg::OnConnectToProxi() 
{
  ClientConnect();
}

void CServerDlg::OnButtonClientSend() 
{
	if (m_pHostSock == NULL && !ClientConnect())
	{
		return;
	}

	UpdateData();

	CString str = m_ClientSendText;
//  ConvertCRLF(&str);
	m_pHostSock->Send(str,str.GetLength());

}


BOOL BindSock(CSocket* pSock,UINT nPort,LPCSTR strAddress)
{
	SOCKET m_S;
	if((m_S = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		AfxMessageBox("Can not create Server socket");
		return  FALSE;
	}
	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));
  
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(strAddress);
	sockAddr.sin_port = htons((u_short)nPort);
	BOOL b = TRUE;

	unsigned long  S_addr = sockAddr.sin_addr.s_addr;
	unsigned short S_port = sockAddr.sin_port;

	if (setsockopt(m_S, SOL_SOCKET, SO_REUSEADDR,  (LPCSTR)&b, sizeof(b)) == SOCKET_ERROR ||
    bind(m_S, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
	{
		int err = WSAGetLastError ();
		CString str;
		str.Format("BIND ERROR = %u",err);
	    ::MessageBox(NULL,str,strAddress,MB_OK);
		closesocket(m_S);
		return  FALSE ;
	}

	if (!pSock->Attach(m_S))
	{
		AfxMessageBox("Can not Attach Server socket");
		return FALSE;
	}
	return TRUE;
}

BOOL ReBindSock(CSocket* pSock,UINT nPort,LPCSTR strAddress)
{
	SOCKET m_S = pSock->m_hSocket;

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));
  
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(strAddress);
	sockAddr.sin_port = htons((u_short)nPort);
	BOOL b = TRUE;

	unsigned long  S_addr = sockAddr.sin_addr.s_addr;
	unsigned short S_port = sockAddr.sin_port;

	if (bind(m_S, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
	{
		int err = WSAGetLastError ();
		CString str;
		str.Format("BIND ERROR = %u",err);
	    ::MessageBox(NULL,str,strAddress,MB_OK);
		closesocket(m_S);
		return  FALSE ;
	}

	return TRUE;
}

void CServerDlg::CloseSockets()
{
	while (m_ClntList.GetCount())
	{	
    CClntSock* ptr = (CClntSock*)m_ClntList.RemoveTail();
		delete ptr;
	}

	if (m_pSrvSock != NULL)
  {
		delete m_pSrvSock;
    m_pSrvSock = NULL;
  }

}

void ConvertCRLF(CString *str)
{
  int nCount = str->GetLength(),j=0;
  char* x_str = new char[nCount];
  x_str[nCount] = 0;

  for (int i=0 ; i<nCount; i++)
  {
    if ((*str)[i] == '\\' && i<nCount-1 && (*str)[i+1] == 'n')
      {x_str[j++] = '\n';i++;}
    else if ((*str)[i] == '\\' && i<nCount-1 && (*str)[i+1] == 'r')
      {x_str[j++] = '\r';i++;}
    else
      x_str[j++] = (*str)[i];
  }
  x_str[j] = 0;
  (*str) = x_str;
}

void CServerDlg::OnClearServerRcv() {m_ReceiveText="";c_ReceiveText.SetWindowText("");}
void CServerDlg::OnClearClientRcv() {m_ClientReceive="";c_ClientReceive.SetWindowText("");}

BOOL CServerDlg::ClientConnect()
{
	UpdateData();
	SetIcon(theApp.LoadIcon(IDR_MAINFRAME),FALSE);
  c_ClientIcon.SetIcon(theApp.LoadIcon(IDR_MAINFRAME) );

	if (m_pHostSock != NULL)
  {
    HideConnection(m_pHostSock);
		delete m_pHostSock;
  }
  m_pHostSock = NULL;

	m_pHostSock = new CHostSock;

	if (!m_pHostSock->Create(m_LocalPort, SOCK_STREAM))
	{
		int err = WSAGetLastError ();
		CString str;
		str.Format("CREATE ERROR = %u",err);
	    ::MessageBox(NULL,str,"",MB_OK);
		delete m_pHostSock;
		m_pHostSock = NULL;
		return FALSE;
	}

	if (!m_pHostSock->Connect(m_HostAddress,m_HostPort))
	{
		int err = WSAGetLastError ();
		CString str;
		str.Format("CONNECT ERROR = %u",err);
	    ::MessageBox(NULL,str,"",MB_OK);
		delete m_pHostSock;
		m_pHostSock = NULL;
		return FALSE;
	}
	SetIcon(theApp.LoadIcon(IDI_CONNECT),FALSE);
  c_ClientIcon.SetIcon(theApp.LoadIcon(IDI_CONNECT) );


  return TRUE;
}

char lpDefaultHeadr[] = "HTTP/1.0 200 OK\r\n"\
                        "Connection: keep-alive\r\n"\
                        "Content-Type: text/html\r\n"\
                        "Pragma: no-cache";

void CServerDlg::GetSetProfile(BOOL bSave)
{
  CWinApp* pApp = AfxGetApp();
  CString strSection = "ServerSettings";
  if (bSave)
  {
    UpdateData();
    pApp->WriteProfileString(strSection, "HostAddress", m_HostAddress);
    pApp->WriteProfileString(strSection, "ServerAddress", m_ServerAddress);
    pApp->WriteProfileInt(strSection, "ServerPort", m_ServerPort);
    pApp->WriteProfileInt(strSection, "HostPort", m_HostPort);
    pApp->WriteProfileInt(strSection, "AddHeadr", m_bAddHeadr);
    pApp->WriteProfileInt(strSection, "AddLength", m_bAddLength );
    pApp->WriteProfileInt(strSection, "LocalPort", m_LocalPort);
    try
    {
      int i = m_ResponceHeadr.GetLength();
      m_ResponceHeadr.TrimLeft();
      m_ResponceHeadr.TrimRight();
      int j = m_ResponceHeadr.GetLength();
      CFile cFile("Responce_Headr.txt", CFile::modeCreate | CFile::modeWrite);
      cFile.Write(m_ResponceHeadr, m_ResponceHeadr.GetLength());
    }
    catch(...)
    {
    }
  }
  else
  {
    m_HostAddress = pApp->GetProfileString(strSection, "HostAddress", "127.0.0.1");
    m_ServerAddress = pApp->GetProfileString(strSection, "ServerAddress", "127.0.0.1");
    m_ServerPort = pApp->GetProfileInt(strSection, "ServerPort", 80);
    m_HostPort   = pApp->GetProfileInt(strSection, "HostPort", 80);
    m_bAddHeadr   = pApp->GetProfileInt(strSection, "AddHeadr", 1);
    m_bAddLength  = pApp->GetProfileInt(strSection, "AddLength", 1);
    m_LocalPort   = pApp->GetProfileInt(strSection, "LocalPort", 0);
    try
    {
      m_ResponceHeadr = "";
      CFile cFile("Responce_Headr.txt", CFile::modeRead);
      char ch;
      while(cFile.Read(&ch, 1))
      {
        m_ResponceHeadr += ch;
      }
    }
    catch(...)
    {
    }
    if (m_ResponceHeadr.GetLength() == 0)
      m_ResponceHeadr = lpDefaultHeadr;
  }
}

void CServerDlg::ShowConnection(CClntSock *pSocket)
{
  CString str; 
  CString strAddress1, strAddress2;  UINT uPort1, uPort2 ;
  pSocket->GetSockName( strAddress1, uPort1 );
  pSocket->GetPeerName( strAddress2, uPort2 );
  str.Format("%s %u - %s %u",strAddress1,uPort1,strAddress2,uPort2);
    
  int iIndex = c_ListBox.AddString(str);
  c_ListBox.SetItemDataPtr(iIndex, pSocket);
}

void CServerDlg::HideConnection(CSocket *pSocket)
{
  CString str; 
  CString strAddress1, strAddress2;  UINT uPort1, uPort2 ;
  pSocket->GetSockName( strAddress1, uPort1 );
  pSocket->GetPeerName( strAddress2, uPort2 );
  str.Format("%s %u - %s %u",strAddress1,uPort1,strAddress2,uPort2);

	UINT nIndex = c_ListBox.FindString(0,str);
	if (nIndex >= 0)
		c_ListBox.DeleteString(nIndex);
}

ClntSockArray* CServerDlg::GetSelectedSockets()
{
  ClntSockArray* pArr = new ClntSockArray;
  int nCount = c_ListBox.GetSelCount();
  CArray<int,int> aryListBoxSel;

  if (nCount > 0)
  {
    aryListBoxSel.SetSize(nCount);
    pArr->SetSize(nCount);
    c_ListBox.GetSelItems(nCount, aryListBoxSel.GetData()); 
    for (int i = 0; i < nCount; i++)
    {
      CClntSock* pSocket = (CClntSock*)c_ListBox.GetItemDataPtr(aryListBoxSel[i]);
      pArr->SetAt(i, pSocket);
    }
  }
  return pArr;
}

void CServerDlg::OnButtonServerClose() 
{

	POSITION pos;
	for( pos = pDialog->m_ClntList.GetHeadPosition(); pos != NULL; )
	{
		CClntSock* pSocket = (CClntSock*)pDialog->m_ClntList.GetNext( pos );
    pSocket->OnClose(0);
		pSocket->Close();		
	}

	if (m_pSrvSock != NULL)
  {
	  SetIcon(theApp.LoadIcon(IDR_MAINFRAME),FALSE);
    c_ServerIcon.SetIcon(theApp.LoadIcon(IDR_MAINFRAME) );
		delete m_pSrvSock;
    m_pSrvSock = NULL;
  }
}

void CServerDlg::OnButtonServerCloseConnection() 
{	
  ClntSockArray* pArr = GetSelectedSockets();
  for (int i = 0; i < pArr->GetSize(); i++)
  {
    CClntSock* pSocket = pArr->GetAt(i);
    pSocket->OnClose(0);
    pSocket->Close();
  }
  delete pArr;
}


void CServerDlg::OnButtonClientClose() 
{
	SetIcon(theApp.LoadIcon(IDR_MAINFRAME),FALSE);
  c_ClientIcon.SetIcon(theApp.LoadIcon(IDR_MAINFRAME) );

	if (m_pHostSock != NULL)
  {
		delete m_pHostSock;
    m_pHostSock = NULL;
  }	
}

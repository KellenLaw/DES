
// SockDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Sock.h"
#include "SockDlg.h"
#include "afxdialogex.h"
#include "Des.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSockDlg 对话框



CSockDlg::CSockDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSockDlg::IDD, pParent)
	, m_iType(0)
	, m_strAddr(_T(""))
	, m_iPort(0)
	, m_strMessage(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RCLIENT, m_iType);
	//DDX_Text(pDX, IDC_ESERVADDR, m_strAddr);
	DDX_Text(pDX, IDC_IPADDRESS, m_strAddr);
	DDX_Text(pDX, IDC_ESERVPORT, m_iPort);
	DDX_Control(pDX, IDC_BCONNECT, m_ctlConnect);
	DDX_Text(pDX, IDC_EMSG, m_strMessage);
	DDX_Control(pDX, IDC_LSENT, m_ctlSent);
	DDX_Control(pDX, IDC_LRECVD, m_ctlRecvd);
	DDX_Text(pDX, IDC_KEY, key);
	DDX_Control(pDX, IDC_LCRECVD, m_ctlLCRecvd);
	DDX_Control(pDX, IDC_LMSEN, m_ctlMSent);
}

BEGIN_MESSAGE_MAP(CSockDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RCLIENT, &CSockDlg::OnBnClickedRclient)
	ON_BN_CLICKED(IDC_RSERVER, &CSockDlg::OnBnClickedRserver)
	ON_BN_CLICKED(IDC_BCONNECT, &CSockDlg::OnBnClickedBconnect)
	ON_BN_CLICKED(IDC_BSEND, &CSockDlg::OnBnClickedBsend)
	ON_BN_CLICKED(IDC_BCLOSE, &CSockDlg::OnBnClickedBclose)
	ON_EN_CHANGE(IDC_KEY, &CSockDlg::OnEnChangeKey)
END_MESSAGE_MAP()


// CSockDlg 消息处理程序

BOOL CSockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// 初始化控件变量
	m_iType = 0; // 默认类型为客户端程序
	//m_strAddr = "loopback"; // 特殊的服务器名 = 127.0.0.1（本机IP地址）
	m_iPort = 4000; // 服务端口号
	m_bConnected = FALSE; // 初始时未连接
	// 更新控件，FALSE=对话框初始化数据，TRUE=从对话框获取数据
	UpdateData(FALSE);

	// 设置套接字对话框指针
	m_sConnectSocket.SetParent(this); // 设置连接套接字的父窗口为对话框
	m_sListenSocket.SetParent(this); // 设置监听套接字的父窗口也为对话框

	BYTE f0,f1,f2,f3;
	CString name;
	CMySocket::GetLocalHostName(name);
	CMySocket::GetIpAddress(name,f0,f1,f2,f3);
	((CIPAddressCtrl *)(GetDlgItem(IDC_IPADDRESS)))->SetAddress(f0,f1,f2,f3);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSockDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSockDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSockDlg::OnAccept() {
	if (m_bConnected) { // 已连接
		CAsyncSocket sRjctSock; // 创建拒绝套接字
		// 创建要发送的消息
		CString strMsg = "Too many connections, try again later."; 
		m_sListenSocket.Accept(sRjctSock); // 接受对拒绝套接字的使用
		// 发送拒绝消息
		sRjctSock.Send(LPCTSTR(strMsg), strMsg.GetLength());
		sRjctSock.Close(); // 关闭套接字
	}
	else { // 未连接
		// 接受连接请求
		m_sListenSocket.Accept(m_sConnectSocket);
		m_bConnected = TRUE;
		// 激活消息和发送控件
		GetDlgItem(IDC_EMSG)->EnableWindow(TRUE);
		GetDlgItem(IDC_BSEND)->EnableWindow(TRUE);
	}
}

void CSockDlg::OnReceive() {
	char *pBuf = new char[1025];
	int iBufSize = 1024;
	int iRcvd;
	CString strRecvd;

	// 接收消息
	iRcvd = m_sConnectSocket.Receive(pBuf, iBufSize);
	// 是否成功收到消息？
	if (iRcvd == SOCKET_ERROR)
 		MessageBox("接收失败！", "套接字错误");
	else {
		// 截去消息串尾
		pBuf[iRcvd] = NULL;
		// 复制消息到串变量
		strRecvd = pBuf;
		m_ctlLCRecvd.AddString(strRecvd);

		//char key[8]={1,2,3,4,5,6,7,8};
		char ckey[8]={key[0],key[1],key[2],key[3],key[4],key[5],key[6],key[7]};
		DesSetKey(ckey);
		char in[9]={0},out[1024]={0},temp[9]={0};
		int j=0,m=0,i,k;
		UpdateData(true);
		for(i=strRecvd.GetLength();i>=8;i=i-8)
		{
			for(k=0;k<=7;k++,j++)
			    in[k]=strRecvd.GetAt(j);
			Des(temp,in,1);
			for(k=0;k<=7;k++,m++)
				out[m]=temp[k];
		}
		
		
		strRecvd=out;
		UpdateData(false);
		// 添加消息到已接收列表框
		m_ctlRecvd.AddString(strRecvd);
		// 使变量与控件同步
		UpdateData(FALSE);
	}
}

void CSockDlg::OnSend() {
}

void CSockDlg::OnClose() {
	// 关闭已连接的套接字
	m_sConnectSocket.Close();
	m_bConnected = FALSE;
	// 禁用消息发送控件
	GetDlgItem(IDC_EMSG)->EnableWindow(FALSE);
	GetDlgItem(IDC_BSEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BCLOSE)->EnableWindow(FALSE);
	// 是否运行在客户模式？
	if (m_iType == 0) {
		// 是，激活连接配置控件
		GetDlgItem(IDC_BCONNECT)->EnableWindow(TRUE);
		//GetDlgItem(IDC_ESERVADDR)->EnableWindow(TRUE);
		GetDlgItem(IDC_KEY)->EnableWindow(TRUE);
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_ESERVPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RCLIENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RSERVER)->EnableWindow(TRUE);
	}
}


void CSockDlg::OnConnect() {
	// 激活消息、发送和关闭控件
	GetDlgItem(IDC_EMSG)->EnableWindow(TRUE);
	GetDlgItem(IDC_BSEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_BCLOSE)->EnableWindow(TRUE);
}


void CSockDlg::OnBnClickedRclient()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 使控件与变量同步
	// 我们在哪种模式？
	if (m_iType == 0)	// 在按钮上设置合适的文本
		m_ctlConnect.SetWindowText("连接(&O)");
	else
		m_ctlConnect.SetWindowText("监听(&I)");

}


void CSockDlg::OnBnClickedRserver()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 使控件与变量同步
	// 我们在哪种模式？
	if (m_iType == 0)	// 在按钮上设置合适的文本
		m_ctlConnect.SetWindowText("连接(&O)");
	else
		m_ctlConnect.SetWindowText("监听(&I)");

}




void CSockDlg::OnBnClickedBconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 使变量与控件同步
	// 禁用连接和类型控件
	GetDlgItem(IDC_BCONNECT)->EnableWindow(FALSE);
	//GetDlgItem(IDC_ESERVADDR)->EnableWindow(FALSE);
	GetDlgItem(IDC_KEY)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
	GetDlgItem(IDC_ESERVPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RCLIENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RSERVER)->EnableWindow(FALSE);
	// 以客户还是服务器方式运行？
	if (m_iType == 0) { // 客户，创建缺省套接字
		/*
		m_sConnectSocket.Create();
		// 打开与服务器的连接
		m_sConnectSocket.Connect(m_strAddr, m_iPort);
		*/
		BYTE f0,f1,f2,f3;
		CString name;
		((CIPAddressCtrl *)(GetDlgItem(IDC_IPADDRESS)))->GetAddress(f0,f1,f2,f3);
		CString ip;
		ip.Format("%d.%d.%d.%d",f0,f1,f2,f3);
		m_sConnectSocket.Create();
		m_sConnectSocket.Connect(ip, m_iPort);
	}
	else { // 服务器，创建绑定指定端口的套接字
		m_sListenSocket.Create(m_iPort);
		m_sListenSocket.Listen(); // 监听连接请求
	}

}


void CSockDlg::OnBnClickedBsend()
{
	// TODO: 在此添加控件通知处理程序代码
	int iLen, iSent; // 设置保存消息长度的变量
	// 使控件与变量同步
	UpdateData(TRUE);
	// 有消息要发送？
	CString m_strSend;
	//char key[8]={1,2,3,4,5,6,7,8};
	char ckey[8]={key[0],key[1],key[2],key[3],key[4],key[5],key[6],key[7]};
	char in[8]={0};
	char out[1000]={0};
	char temp[8]={0};
	int j=0,m=0,i,k;
	DesSetKey(ckey);
	for(i=m_strMessage.GetLength();i>=8;i=i-8)
		{
			for(k=0;k<=7;k++,j++)
			    in[k]=m_strMessage.GetAt(j);
			  Des(temp,in,0);
			for(k=0;k<=7;k++,m++)
				out[m]=temp[k];
		}
			for(k=0;k<i;k++,j++)
				in[k]=m_strMessage.GetAt(j);
			for(;k<=7;k++)
				in[k]=NULL;  //不足整数8的用1进行填充
			 Des(temp,in,0);
			for(k=0;k<=7;k++,m++)
				out[m]=temp[k];
		
			m_strSend=out;
			UpdateData(false);
	
	if (m_strSend != "") {
		// 获取消息长度
		iLen = m_strSend.GetLength();
		// 发送消息
		iSent = m_sConnectSocket.Send(LPCTSTR(m_strSend), iLen);
		// 是否发送成功？
		if (iSent == SOCKET_ERROR)
 			MessageBox("发送失败！", "套接字错误");
		else {
			// 添加消息到列表框
			m_ctlMSent.AddString(m_strMessage);
			m_ctlSent.AddString(m_strSend);
			m_strMessage="";
			// 使控件与变量同步
			UpdateData(FALSE);
		}
	}

}


void CSockDlg::OnBnClickedBclose()
{
	// TODO: 在此添加控件通知处理程序代码
	// 调用OnClose函数
	   OnClose();
	  
}


void CSockDlg::OnEnChangeKey()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。


	// TODO:  在此添加控件通知处理程序代码
	this->UpdateData(true);
	if(key.GetLength()>8)
	{
		//this->MessageBox("Limited to 8 Bytes Only!");
		this->MessageBox("请输入8字节数据!");
		key.Delete(8,key.GetLength()-8);  //清除多余字节
		this->UpdateData(false);
	}
}


BOOL CSockDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
              OnBnClickedBsend();
	return false;
}
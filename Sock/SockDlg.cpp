
// SockDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Sock.h"
#include "SockDlg.h"
#include "afxdialogex.h"
#include "Des.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSockDlg �Ի���



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


// CSockDlg ��Ϣ�������

BOOL CSockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	// ��ʼ���ؼ�����
	m_iType = 0; // Ĭ������Ϊ�ͻ��˳���
	//m_strAddr = "loopback"; // ����ķ������� = 127.0.0.1������IP��ַ��
	m_iPort = 4000; // ����˿ں�
	m_bConnected = FALSE; // ��ʼʱδ����
	// ���¿ؼ���FALSE=�Ի����ʼ�����ݣ�TRUE=�ӶԻ����ȡ����
	UpdateData(FALSE);

	// �����׽��ֶԻ���ָ��
	m_sConnectSocket.SetParent(this); // ���������׽��ֵĸ�����Ϊ�Ի���
	m_sListenSocket.SetParent(this); // ���ü����׽��ֵĸ�����ҲΪ�Ի���

	BYTE f0,f1,f2,f3;
	CString name;
	CMySocket::GetLocalHostName(name);
	CMySocket::GetIpAddress(name,f0,f1,f2,f3);
	((CIPAddressCtrl *)(GetDlgItem(IDC_IPADDRESS)))->SetAddress(f0,f1,f2,f3);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSockDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSockDlg::OnAccept() {
	if (m_bConnected) { // ������
		CAsyncSocket sRjctSock; // �����ܾ��׽���
		// ����Ҫ���͵���Ϣ
		CString strMsg = "Too many connections, try again later."; 
		m_sListenSocket.Accept(sRjctSock); // ���ܶԾܾ��׽��ֵ�ʹ��
		// ���;ܾ���Ϣ
		sRjctSock.Send(LPCTSTR(strMsg), strMsg.GetLength());
		sRjctSock.Close(); // �ر��׽���
	}
	else { // δ����
		// ������������
		m_sListenSocket.Accept(m_sConnectSocket);
		m_bConnected = TRUE;
		// ������Ϣ�ͷ��Ϳؼ�
		GetDlgItem(IDC_EMSG)->EnableWindow(TRUE);
		GetDlgItem(IDC_BSEND)->EnableWindow(TRUE);
	}
}

void CSockDlg::OnReceive() {
	char *pBuf = new char[1025];
	int iBufSize = 1024;
	int iRcvd;
	CString strRecvd;

	// ������Ϣ
	iRcvd = m_sConnectSocket.Receive(pBuf, iBufSize);
	// �Ƿ�ɹ��յ���Ϣ��
	if (iRcvd == SOCKET_ERROR)
 		MessageBox("����ʧ�ܣ�", "�׽��ִ���");
	else {
		// ��ȥ��Ϣ��β
		pBuf[iRcvd] = NULL;
		// ������Ϣ��������
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
		// �����Ϣ���ѽ����б��
		m_ctlRecvd.AddString(strRecvd);
		// ʹ������ؼ�ͬ��
		UpdateData(FALSE);
	}
}

void CSockDlg::OnSend() {
}

void CSockDlg::OnClose() {
	// �ر������ӵ��׽���
	m_sConnectSocket.Close();
	m_bConnected = FALSE;
	// ������Ϣ���Ϳؼ�
	GetDlgItem(IDC_EMSG)->EnableWindow(FALSE);
	GetDlgItem(IDC_BSEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BCLOSE)->EnableWindow(FALSE);
	// �Ƿ������ڿͻ�ģʽ��
	if (m_iType == 0) {
		// �ǣ������������ÿؼ�
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
	// ������Ϣ�����ͺ͹رտؼ�
	GetDlgItem(IDC_EMSG)->EnableWindow(TRUE);
	GetDlgItem(IDC_BSEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_BCLOSE)->EnableWindow(TRUE);
}


void CSockDlg::OnBnClickedRclient()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); // ʹ�ؼ������ͬ��
	// ����������ģʽ��
	if (m_iType == 0)	// �ڰ�ť�����ú��ʵ��ı�
		m_ctlConnect.SetWindowText("����(&O)");
	else
		m_ctlConnect.SetWindowText("����(&I)");

}


void CSockDlg::OnBnClickedRserver()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); // ʹ�ؼ������ͬ��
	// ����������ģʽ��
	if (m_iType == 0)	// �ڰ�ť�����ú��ʵ��ı�
		m_ctlConnect.SetWindowText("����(&O)");
	else
		m_ctlConnect.SetWindowText("����(&I)");

}




void CSockDlg::OnBnClickedBconnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); // ʹ������ؼ�ͬ��
	// �������Ӻ����Ϳؼ�
	GetDlgItem(IDC_BCONNECT)->EnableWindow(FALSE);
	//GetDlgItem(IDC_ESERVADDR)->EnableWindow(FALSE);
	GetDlgItem(IDC_KEY)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
	GetDlgItem(IDC_ESERVPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RCLIENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RSERVER)->EnableWindow(FALSE);
	// �Կͻ����Ƿ�������ʽ���У�
	if (m_iType == 0) { // �ͻ�������ȱʡ�׽���
		/*
		m_sConnectSocket.Create();
		// ���������������
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
	else { // ��������������ָ���˿ڵ��׽���
		m_sListenSocket.Create(m_iPort);
		m_sListenSocket.Listen(); // ������������
	}

}


void CSockDlg::OnBnClickedBsend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iLen, iSent; // ���ñ�����Ϣ���ȵı���
	// ʹ�ؼ������ͬ��
	UpdateData(TRUE);
	// ����ϢҪ���ͣ�
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
				in[k]=NULL;  //��������8����1�������
			 Des(temp,in,0);
			for(k=0;k<=7;k++,m++)
				out[m]=temp[k];
		
			m_strSend=out;
			UpdateData(false);
	
	if (m_strSend != "") {
		// ��ȡ��Ϣ����
		iLen = m_strSend.GetLength();
		// ������Ϣ
		iSent = m_sConnectSocket.Send(LPCTSTR(m_strSend), iLen);
		// �Ƿ��ͳɹ���
		if (iSent == SOCKET_ERROR)
 			MessageBox("����ʧ�ܣ�", "�׽��ִ���");
		else {
			// �����Ϣ���б��
			m_ctlMSent.AddString(m_strMessage);
			m_ctlSent.AddString(m_strSend);
			m_strMessage="";
			// ʹ�ؼ������ͬ��
			UpdateData(FALSE);
		}
	}

}


void CSockDlg::OnBnClickedBclose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ����OnClose����
	   OnClose();
	  
}


void CSockDlg::OnEnChangeKey()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�


	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(true);
	if(key.GetLength()>8)
	{
		//this->MessageBox("Limited to 8 Bytes Only!");
		this->MessageBox("������8�ֽ�����!");
		key.Delete(8,key.GetLength()-8);  //��������ֽ�
		this->UpdateData(false);
	}
}


BOOL CSockDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
              OnBnClickedBsend();
	return false;
}
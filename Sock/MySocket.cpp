// MySocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Sock.h"
#include "MySocket.h"
#include "SockDlg.h"

// CMySocket

CMySocket::CMySocket()
{
}

CMySocket::~CMySocket()
{
}


// CMySocket 成员函数

void CMySocket::SetParent(CDialog *pWnd) {
	m_pWnd = pWnd; // 设置成员指针
}

void CMySocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (nErrorCode == 0) // 无错误，调用对话框的OnAccept函数
		((CSockDlg*)m_pWnd)->OnAccept();

	//CAsyncSocket::OnAccept(nErrorCode);
}


void CMySocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (nErrorCode == 0) // 无错误，调用对话框的OnClose函数
		((CSockDlg*)m_pWnd)->OnClose();

	//CAsyncSocket::OnClose(nErrorCode);
}


void CMySocket::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (nErrorCode == 0) // 无错误，调用对话框的OnConnect函数
		((CSockDlg*)m_pWnd)->OnConnect();

	//CAsyncSocket::OnConnect(nErrorCode);
}


void CMySocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (nErrorCode == 0) // 无错误，调用对话框的OnReceive函数
		((CSockDlg*)m_pWnd)->OnReceive();

	//CAsyncSocket::OnReceive(nErrorCode);
}


void CMySocket::OnSend(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (nErrorCode == 0) // 无错误，调用对话框的OnSend函数
		((CSockDlg*)m_pWnd)->OnSend();

	//CAsyncSocket::OnSend(nErrorCode);
}

//获得本地计算机名称
int CMySocket::GetLocalHostName(CString &sHostName)
{
	char szHostName[256];
	int nRetCode;
	nRetCode=gethostname(szHostName,sizeof(szHostName));
	if(nRetCode!=0)//产生错误
	{
		sHostName=_T("没有取得");
		return GetLastError();
	}
	sHostName=szHostName;
	return 0;
}

//获得本地IP地址
int CMySocket::GetIpAddress(const CString &sHostName,CString &sIpAddress)
{
	struct hostent FAR * lpHostEnt=gethostbyname(sHostName);
	//返回对应于给定主机名的包含主机名字和地址信息的hostent结构指针
    //struct hostent
	//{
    // char FAR* h_name;//正规的主机名字(PC)
    // char FAR*FAR* h_aliases;//一个以空指针结尾的可选主机名队列
    // short h_addrtype;//返回地址类型，对于Winsock,这个域总是AF_INET
    // short h_length;//每个地址的长度
    // char FAR*FAR* h_addr_list;//以空指针结尾的主机地址列表，返回的地址是以网络顺序排列的
    //}
	if(lpHostEnt==NULL)
	{
		//产生错误
		sIpAddress=_T("");
		return GetLastError();
	}
	//获取IP
	LPSTR lpAddr=lpHostEnt->h_addr_list[0];
	if(lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr,lpAddr,4);
		//转换为标准格式
		sIpAddress=inet_ntoa(inAddr);
		if(sIpAddress.IsEmpty())
			sIpAddress=_T("没有取得");
	}
	return 0;
}

//获取本机IP
int CMySocket::GetIpAddress(const CString &sHostName,BYTE &f0,BYTE &f1,BYTE &f2,BYTE &f3)
{
	struct hostent FAR * lpHostEnt=gethostbyname(sHostName);
	if(lpHostEnt==NULL)
	{
		//产生错误
		f0=f1=f2=f3=0;
		return GetLastError();
	}

	//获取IP
	LPSTR lpAddr=lpHostEnt->h_addr_list[0];
	if(lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr,lpAddr,4);
		f0=inAddr.S_un.S_un_b.s_b1;
		f1=inAddr.S_un.S_un_b.s_b2;
		f2=inAddr.S_un.S_un_b.s_b3;
		f3=inAddr.S_un.S_un_b.s_b4;
	}
	return 0;
}
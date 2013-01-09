// MySocket.cpp : ʵ���ļ�
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


// CMySocket ��Ա����

void CMySocket::SetParent(CDialog *pWnd) {
	m_pWnd = pWnd; // ���ó�Աָ��
}

void CMySocket::OnAccept(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (nErrorCode == 0) // �޴��󣬵��öԻ����OnAccept����
		((CSockDlg*)m_pWnd)->OnAccept();

	//CAsyncSocket::OnAccept(nErrorCode);
}


void CMySocket::OnClose(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (nErrorCode == 0) // �޴��󣬵��öԻ����OnClose����
		((CSockDlg*)m_pWnd)->OnClose();

	//CAsyncSocket::OnClose(nErrorCode);
}


void CMySocket::OnConnect(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (nErrorCode == 0) // �޴��󣬵��öԻ����OnConnect����
		((CSockDlg*)m_pWnd)->OnConnect();

	//CAsyncSocket::OnConnect(nErrorCode);
}


void CMySocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (nErrorCode == 0) // �޴��󣬵��öԻ����OnReceive����
		((CSockDlg*)m_pWnd)->OnReceive();

	//CAsyncSocket::OnReceive(nErrorCode);
}


void CMySocket::OnSend(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (nErrorCode == 0) // �޴��󣬵��öԻ����OnSend����
		((CSockDlg*)m_pWnd)->OnSend();

	//CAsyncSocket::OnSend(nErrorCode);
}

//��ñ��ؼ��������
int CMySocket::GetLocalHostName(CString &sHostName)
{
	char szHostName[256];
	int nRetCode;
	nRetCode=gethostname(szHostName,sizeof(szHostName));
	if(nRetCode!=0)//��������
	{
		sHostName=_T("û��ȡ��");
		return GetLastError();
	}
	sHostName=szHostName;
	return 0;
}

//��ñ���IP��ַ
int CMySocket::GetIpAddress(const CString &sHostName,CString &sIpAddress)
{
	struct hostent FAR * lpHostEnt=gethostbyname(sHostName);
	//���ض�Ӧ�ڸ����������İ����������ֺ͵�ַ��Ϣ��hostent�ṹָ��
    //struct hostent
	//{
    // char FAR* h_name;//�������������(PC)
    // char FAR*FAR* h_aliases;//һ���Կ�ָ���β�Ŀ�ѡ����������
    // short h_addrtype;//���ص�ַ���ͣ�����Winsock,���������AF_INET
    // short h_length;//ÿ����ַ�ĳ���
    // char FAR*FAR* h_addr_list;//�Կ�ָ���β��������ַ�б����صĵ�ַ��������˳�����е�
    //}
	if(lpHostEnt==NULL)
	{
		//��������
		sIpAddress=_T("");
		return GetLastError();
	}
	//��ȡIP
	LPSTR lpAddr=lpHostEnt->h_addr_list[0];
	if(lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr,lpAddr,4);
		//ת��Ϊ��׼��ʽ
		sIpAddress=inet_ntoa(inAddr);
		if(sIpAddress.IsEmpty())
			sIpAddress=_T("û��ȡ��");
	}
	return 0;
}

//��ȡ����IP
int CMySocket::GetIpAddress(const CString &sHostName,BYTE &f0,BYTE &f1,BYTE &f2,BYTE &f3)
{
	struct hostent FAR * lpHostEnt=gethostbyname(sHostName);
	if(lpHostEnt==NULL)
	{
		//��������
		f0=f1=f2=f3=0;
		return GetLastError();
	}

	//��ȡIP
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
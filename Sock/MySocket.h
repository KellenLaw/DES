#pragma once

// CMySocket ����Ŀ��

#include "afxsock.h"

class CMySocket : public CAsyncSocket
{
public:
	CMySocket();
	virtual ~CMySocket();
	void SetParent(CDialog *pWnd);

private:
	CDialog* m_pWnd;

public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);

	static int GetLocalHostName(CString &sHostName);  //��ñ��ؼ��������
	static int GetIpAddress(const CString &sHostName,CString &sIpAddress);  //��ñ��ؼ���IP��ַ
	static int GetIpAddress(const CString &sHostName,BYTE &f0,BYTE &f1,BYTE &f2,BYTE &f3);// ��ñ��ؼ����IP��ַ
};



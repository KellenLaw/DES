
// SockDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "MySocket.h"
#include <string>
// CSockDlg �Ի���
class CSockDlg : public CDialogEx
{
// ����
public:
	CSockDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SOCK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_iType;
	CString m_strAddr;
	int m_iPort;
	CButton m_ctlConnect;
	CString m_strMessage;
	CListBox m_ctlSent;     //��ʾ�ѷ��͵�������Ϣ
	CListBox m_ctlMSent;    //��ʾ�ѷ��͵�������Ϣ
	CListBox m_ctlRecvd;    //��ʾ�ѽ������Ľ��ܺ���Ϣ
	CListBox m_ctlLCRecvd;  //��ʾ�ѽ���������Ϣ
	afx_msg void OnBnClickedRclient();
	afx_msg void OnBnClickedRserver();

private:
	CMySocket m_sListenSocket, m_sConnectSocket;
	BOOL m_bConnected;

public:
	afx_msg void OnBnClickedBconnect();
	void OnAccept();
	void OnConnect();
	void OnReceive();
	void OnSend();
	void OnClose();
	afx_msg void OnBnClickedBsend();
	afx_msg void OnBnClickedBclose();
	CString key;
	afx_msg void OnEnChangeKey();

	BOOL PreTranslateMessage(MSG* pMsg);
};

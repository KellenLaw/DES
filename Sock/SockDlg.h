
// SockDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "MySocket.h"
#include <string>
// CSockDlg 对话框
class CSockDlg : public CDialogEx
{
// 构造
public:
	CSockDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SOCK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	CListBox m_ctlSent;
	CListBox m_ctlRecvd;
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
};

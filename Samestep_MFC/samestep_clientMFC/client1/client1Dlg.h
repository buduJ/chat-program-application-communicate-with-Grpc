
// client1Dlg.h: 头文件
//

#pragma once
#include "client_source.cpp"

#define WM_UPDATE_MESSAGE (WM_USER+100)
// Cclient1Dlg 对话框
class Cclient1Dlg : public CDialogEx
{
// 构造
public:
	Cclient1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT1_DIALOG };
#endif

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
	// 本机ID	// 本机ID
	int MFC_id;
	// 要发送的客户端id	// 要发送的客户端id
	int MFC_next_id;
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT OnUpdateMessage(WPARAM wParam, LPARAM lParam);

	// 收到的消息	// 收到的消息
	CString g_msg;
	// 发送的消息	// 发送的消息
	CString p_msg;

	ChatClient *client;
	bool over = false;
	bool flag = false;
	void update();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};

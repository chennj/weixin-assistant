
// MFCWebDemoDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "explorer1.h"


// CMFCWebDemoDlg 对话框
class CMFCWebDemoDlg : public CDialogEx
{
// 构造
public:
	CMFCWebDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCWEBDEMO_DIALOG };
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
	CEdit m_EditAddress;
	CExplorer1 m_WebIE;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

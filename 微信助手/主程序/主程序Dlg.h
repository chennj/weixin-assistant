
// 主程序Dlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// C主程序Dlg 对话框
class C主程序Dlg : public CDialogEx
{
// 构造
public:
	C主程序Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_DIALOG };
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
	CListCtrl m_List;
	CString m_Hex;
	CString m_String;
	afx_msg void OnBnClickedOk();
	BOOL MyPrivilegeValue();
	afx_msg void OnBnClickedBtnTiquan();

	LRESULT GetThreadIdMsg(WPARAM wparam, LPARAM lparam);
	afx_msg void OnBnClickedBtnPause();
private:
	DWORD m_ThreadId;
public:
	afx_msg void OnBnClickedBtnRecontinue();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnFreedll();
private:
	LRESULT MyFreeLibrary(WPARAM wparam, LPARAM lparam);
	LRESULT MyFreeLibrarySuccess(WPARAM wparam, LPARAM lparam);
	LRESULT MyFreeLibraryFailed(WPARAM wparam, LPARAM lparam);
	BOOL FreeMyDll(HMODULE hMod);
public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	// 参数
	CEdit m_Param;
	afx_msg void OnBnClickedBtnMemText();
	afx_msg void OnBnClickedBtnTest();
};

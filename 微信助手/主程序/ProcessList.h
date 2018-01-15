#pragma once
#include "afxcmn.h"

#include <TlHelp32.h>

extern DWORD gPid;

// CProcessList 对话框

class CProcessList : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessList)

public:
	CProcessList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcessList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROCESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_Process;
	void GetListProcess();
	afx_msg void OnBnClickedOk();
};


// 主程序Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "主程序.h"
#include "主程序Dlg.h"
#include "afxdialogex.h"
#include "ApiUtils.h"
#include "StringUtils.h"

#include "ProcessList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static wchar_t* trans(const char * ch, int type = CP_ACP) {
	int len = MultiByteToWideChar(type, 0, ch, -1, nullptr, 0);
	wchar_t *str = new wchar_t[len + 1];
	wmemset(str, 0, len + 1);
	MultiByteToWideChar(type, 0, ch, -1, str, len);
	return str;
}

// 当type为CP_ACP时，UNICODE转化为GBK；当type为CP_UTF8时，UNICODE转化为UTF8  
static char* trans(const wchar_t * wch, int type = CP_ACP) {
	int len = WideCharToMultiByte(type, 0, wch, -1, nullptr, 0, nullptr, nullptr);
	char *str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(type, 0, wch, -1, str, len, nullptr, nullptr);
	return str;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C主程序Dlg 对话框



C主程序Dlg::C主程序Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MY_DIALOG, pParent)
	, m_Hex(_T(""))
	, m_String(_T(""))
	, m_ThreadId(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C主程序Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LIST, m_List);
	DDX_Text(pDX, IDC_EDIT_HEX, m_Hex);
	DDX_Text(pDX, IDC_EDIT_STRING, m_String);
	DDX_Control(pDX, IDC_EDIT_PARAM, m_Param);
}

BEGIN_MESSAGE_MAP(C主程序Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &C主程序Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_TIQUAN, &C主程序Dlg::OnBnClickedBtnTiquan)
	ON_MESSAGE(WM_THREAD_ID, &C主程序Dlg::GetThreadIdMsg)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &C主程序Dlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_RECONTINUE, &C主程序Dlg::OnBnClickedBtnRecontinue)
	ON_BN_CLICKED(IDCANCEL, &C主程序Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_FREEDLL, &C主程序Dlg::OnBnClickedBtnFreedll)
	ON_MESSAGE(WM_FREELIBRARY, &C主程序Dlg::MyFreeLibrary)
	ON_MESSAGE(WM_FREESUCCESS, &C主程序Dlg::MyFreeLibrarySuccess)
	ON_MESSAGE(WM_FORCEEXIT, &C主程序Dlg::MyFreeLibraryFailed)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BTN_MEM_TEXT, &C主程序Dlg::OnBnClickedBtnMemText)
	ON_BN_CLICKED(IDC_BTN_TEST, &C主程序Dlg::OnBnClickedBtnTest)
END_MESSAGE_MAP()


// C主程序Dlg 消息处理程序

BOOL C主程序Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_List.InsertColumn(0, TEXT("内容"), 0, 600);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C主程序Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C主程序Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C主程序Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void C主程序Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();

	CProcessList dlg;
	dlg.DoModal();
}

//提升主程序的权限
BOOL C主程序Dlg::MyPrivilegeValue()
{
	BOOL bRet;
	HANDLE token_handle = NULL;
	
	bRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &token_handle);
	if (bRet) {

		LUID luid;
		bRet = LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);
		if (bRet) {

			TOKEN_PRIVILEGES tokenpr = { 0 };
			tokenpr.PrivilegeCount = 1;
			tokenpr.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			bRet = AdjustTokenPrivileges(token_handle, FALSE, &tokenpr, sizeof(tokenpr), NULL, NULL);
		}
		CloseHandle(token_handle);
	}
	return bRet;
}


void C主程序Dlg::OnBnClickedBtnTiquan()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MyPrivilegeValue()) {
		AfxMessageBox(TEXT("提权成功"));
	}
	else {
		AfxMessageBox(TEXT("提权失败"));
	}

}

LRESULT C主程序Dlg::GetThreadIdMsg(WPARAM wparam, LPARAM lparam) {

	m_ThreadId = (DWORD)lparam;

	CString str_thid;
	str_thid.Format(TEXT("注入 DLL Thread 成功 ID 是：%d"), m_ThreadId);
	AfxMessageBox(str_thid);

	return NULL;
}

void C主程序Dlg::OnBnClickedBtnPause()
{
	// TODO: 在此添加控件通知处理程序代码

	BOOL bRet = ::PostThreadMessage(m_ThreadId, WM_PAUSE_RECORD, (WPARAM)0, (LPARAM)0);
	if (!bRet) {
		CString err;
		err.Format(TEXT("发送失败:%d"), GetLastError());
		AfxMessageBox(err);
	}
}


void C主程序Dlg::OnBnClickedBtnRecontinue()
{
	// TODO: 在此添加控件通知处理程序代码

	BOOL bRet = ::PostThreadMessage(m_ThreadId, WM_RECONTINUE_RECORD, (WPARAM)0, (LPARAM)0);
	if (!bRet) {
		CString err;
		err.Format(TEXT("发送失败:%d"), GetLastError());
		AfxMessageBox(err);
	}

}

void C主程序Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	BOOL bRet = ::PostThreadMessage(m_ThreadId, WM_FREEDLL, (WPARAM)0, (LPARAM)0);
	//if (!bRet) {
	//	CString err;
	//	err.Format(TEXT("发送失败:%d"), GetLastError());
	//	AfxMessageBox(err);
	//}

	CDialogEx::OnCancel();
}


void C主程序Dlg::OnBnClickedBtnFreedll()
{
	// TODO: 在此添加控件通知处理程序代码
	HMODULE hFreeMod = NULL;
	HANDLE hModHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, gPid);
	if (hModHandle) {

		MODULEENTRY32W lpme = { 0 };
		lpme.dwSize = sizeof(MODULEENTRY32W);
		BOOL bRet = Module32First(hModHandle,&lpme);
		while (bRet) {

			if (lstrcmpW(lpme.szModule, TEXT("微信助手.dll")) == 0) {

				hFreeMod = (HMODULE)lpme.modBaseAddr;
				if (FreeMyDll(hFreeMod)) {
					//AfxMessageBox(TEXT("卸载成功"));
				}
				else {
					AfxMessageBox(TEXT("卸载失败"));
				}

				break;
			}
			bRet = Module32Next(hModHandle, &lpme);
		}
		CloseHandle(hModHandle);
	}
	else {
		AfxMessageBox(TEXT("[OnBnClickedBtnFreedll]遍历失败"));
	}
}


LRESULT C主程序Dlg::MyFreeLibrary(WPARAM wparam, LPARAM lparam)
{
	
	AfxMessageBox(TEXT("HOOK线程正在退出"));

	return 0;
}

LRESULT C主程序Dlg::MyFreeLibrarySuccess(WPARAM wparam, LPARAM lparam)
{
	AfxMessageBox(TEXT("HOOK线程成功退出"));
	return LRESULT();
}

LRESULT C主程序Dlg::MyFreeLibraryFailed(WPARAM wparam, LPARAM lparam)
{
	AfxMessageBox(TEXT("HOOK线程强制退出"));
	return LRESULT();
}

BOOL C主程序Dlg::FreeMyDll(HMODULE hMod) {

	BOOL bRet = FALSE;
	HANDLE hPro = OpenProcess(PROCESS_ALL_ACCESS, FALSE, gPid);
	if (hPro) {
	
		PVOID pLoad = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "FreeLibrary");
	
		HANDLE hRemote = CreateRemoteThread(
			hPro,
			NULL,
			0,
			(LPTHREAD_START_ROUTINE)pLoad,
			hMod,
			0,
			NULL);
	
		if (hRemote) {
	
			CloseHandle(hRemote);
			bRet = TRUE;
		}
		CloseHandle(hPro);
	}
	return bRet;
}

BOOL C主程序Dlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	char* pbuf = new char[pCopyDataStruct->cbData];
	ZeroMemory(pbuf, pCopyDataStruct->cbData);
	memcpy(pbuf, pCopyDataStruct->lpData, pCopyDataStruct->cbData);

	CString cstr;
	//cstr.Format(TEXT("%s"), trans(pbuf));
	cstr.Format(TEXT("%s"), trans(pbuf));
	int idx = m_List.GetItemCount();
	m_List.InsertItem(idx, cstr);

	cstr.ReleaseBuffer();
	free(pbuf);

	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}


void C主程序Dlg::OnBnClickedBtnMemText()
{
	// TODO: 在此添加控件通知处理程序代码
	CString param;
	m_Param.GetWindowTextW(param);

	DWORD pid = ApiUtils::get_pid_by_name(TEXT("WeChat.exe"));
	if (NULL == pid) {
		m_String = "获取WeChat.exe PID失败;";
		goto end;
	}

	DWORD module_base_addr = ApiUtils::get_module_baseaddr(pid, TEXT("WeChatWin.dll"));

	if (NULL == module_base_addr) {
		m_String  = "获取WeChatWin.dll基址失败;";
		goto end;
	}

	int offset_addr = StringUtils::hexstr_to_int(param, param.GetLength());

	DWORD addr = module_base_addr + offset_addr;
	char * ptx = ApiUtils::mem_read_text(pid, addr, 100);
	m_String = CString(ptx);

end:

	UpdateData(FALSE);
}

//static int getlen(char *p) {
//
//	int count = 0;
//	while (*p++)count++;
//	return count;
//}
//static LONG  pwndproc;
static VOID MyWindowProc(
	_In_ HWND hwnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {

	switch (uMsg) {

	case WM_LBUTTONDOWN:
	{

		COPYDATASTRUCT cData = { 0 };
		char text[64];
		int len = sprintf_s(text, "0x%08X-%d-%d-%d", hwnd, uMsg, wParam, lParam);
		cData.lpData = text;
		cData.cbData = len;
		::SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&cData);
		break;
	}

	case WM_SYSCOMMAND:
	{

		COPYDATASTRUCT cData = { 0 };
		char * text = "你想结束窗体？没可能\0";
		cData.lpData = text;
		cData.cbData = strlen(text);
		::SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&cData);
		break;
	}

	case WM_CLOSE:
	{

		COPYDATASTRUCT cData = { 0 };
		char * text = "你想结束窗体？没可能\0";
		cData.lpData = text;
		cData.cbData = strlen(text);
		::SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&cData);
		break;
	}

	case WM_MOVE:
	{

		COPYDATASTRUCT cData = { 0 };
		char * text = "你移动窗口了！\0";
		cData.lpData = text;
		cData.cbData = strlen(text);
		::SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&cData);
		break;
	}

	}
}

void C主程序Dlg::OnBnClickedBtnTest()
{
	// TODO: 在此添加控件通知处理程序代码

	//pwndproc = ::GetWindowLong(ApiUtils::get_self_ancestor_handle(), GWLP_WNDPROC);
	//::SetWindowLong(ApiUtils::get_self_ancestor_handle(), GWLP_WNDPROC, (DWORD)MyWindowProc);
	ApiUtils::hook_windproc(MyWindowProc);
}

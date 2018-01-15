// 微信助手.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "微信助手.h"

#include <process.h>
#include "Inc.h"
#include "ApiUtils.h"
#include "StringUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// C微信助手App

BEGIN_MESSAGE_MAP(C微信助手App, CWinApp)
END_MESSAGE_MAP()


// C微信助手App 构造

C微信助手App::C微信助手App()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 C微信助手App 对象

C微信助手App theApp;


// C微信助手App 初始化

BOOL C微信助手App::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	bPauseRecord = FALSE;

	_beginthread(MyWorkThread, 0, NULL);

	return TRUE;
}

//全局变量
DWORD	mSendJmp = 0;
DWORD	mLoginJmp = 0;
DWORD	g_ThreadId = 0;
HANDLE	g_WorkThreadHandle = NULL;
BOOL	debug = TRUE;
//----------------------

static VOID MyWindowProc(
	_In_ HWND hwnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {

	if (bPauseRecord) {
		return;
	}
		
	HWND hWndTarget = ::FindWindow(NULL, TEXT("CNJ注入工具主程序"));

	switch (uMsg) {

	case WM_LBUTTONDOWN:
	{

		COPYDATASTRUCT cData = { 0 };
		char text[128];
		int len = sprintf_s(text, "WM_LBUTTONDOWN = hwnd:[0x%08X]-msg:[%d]-wp:[%d]-lp:[%d]", hwnd, uMsg, wParam, lParam);
		cData.lpData = text;
		cData.cbData = len;
		::SendMessage(hWndTarget, WM_COPYDATA, 0, (LPARAM)&cData);
		break;
	}

	case WM_SYSCOMMAND:
	{

		COPYDATASTRUCT cData = { 0 };
		char text[128];
		int len = sprintf_s(text, "WM_SYSCOMMAND = hwnd:[0x%08X]-msg:[%d]-wp:[%d]-lp:[%d]", hwnd, uMsg, wParam, lParam);
		cData.lpData = text;
		cData.cbData = len;
		::SendMessage(hWndTarget, WM_COPYDATA, 0, (LPARAM)&cData);
		break;
	}

	case WM_CLOSE:
	{

		COPYDATASTRUCT cData = { 0 };
		char text[128];
		int len = sprintf_s(text, "WM_CLOSE = hwnd:[0x%08X]-msg:[%d]-wp:[%d]-lp:[%d]", hwnd, uMsg, wParam, lParam);
		cData.lpData = text;
		cData.cbData = len;
		::SendMessage(hWndTarget, WM_COPYDATA, 0, (LPARAM)&cData);
		break;
	}

	case WM_MOVE:
	{

		COPYDATASTRUCT cData = { 0 };
		char text[128];
		int len = sprintf_s(text, "WM_MOVE = hwnd:[0x%08X]-msg:[%d]-wp:[%d]-lp:[%d]", hwnd, uMsg, wParam, lParam);
		cData.lpData = text;
		cData.cbData = len;
		::SendMessage(hWndTarget, WM_COPYDATA, 0, (LPARAM)&cData);
		break;
	}

	}

	//if (uMsg == 132 || uMsg == 32 || uMsg == 874 || uMsg == 512 || 
	//	uMsg == 160 || uMsg == 15 || uMsg == 646 || uMsg == 642 ||
	//	uMsg == 258 || uMsg == 674 || uMsg == 675) return;

	//COPYDATASTRUCT cData = { 0 };
	//char text[128];
	//int len = sprintf_s(text, "hwnd:[0x%08X]-msg:[%d]-wp:[%d]-lp:[%d]", hwnd, uMsg, wParam, lParam);
	//cData.lpData = text;
	//cData.cbData = len;
	//::SendMessage(hWndTarget, WM_COPYDATA, 0, (LPARAM)&cData);

}

//hook 发送------------------------------------------
void _declspec(naked)MySend() {

	_asm {
		mov edi, edi
		push ebp
		mov ebp, esp
		pushad
		pushfd

		push[ebp + 10h]
		push[ebp + 0ch]
		push[ebp + 08h]
		call MyGetSendData

		popfd
		popad
		jmp mSendJmp
	}
}

void WINAPI MyGetSendData(SOCKET sock, char * pData, int nSize) {

	//static int times = 0;
	//if (times++ < 5) {
	//	CString cstr = ((LPWSABUF)pData)->buf;
	//	AfxMessageBox(cstr);
	//}

	//return;

	//获取窗口句柄
	HWND hwnd = ::FindWindow(NULL, TEXT("CNJ注入工具主程序"));
	if (NULL == hwnd) {
		return;
	}

	//判断记录还是暂停记录
	if (!bPauseRecord && debug) {

		//如果hook的是WSASend就使用下面的代码
		COPYDATASTRUCT cData = { 0 };
		cData.lpData = ((LPWSABUF)pData)->buf;
		cData.cbData = ((LPWSABUF)pData)->len;

		//如果hook的是send
		//COPYDATASTRUCT cData = { 0 };
		//cData.lpData = pData;
		//cData.cbData = nSize;

		::SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&cData);
	}

	//获取连接ip地址
	sockaddr_in peersockaddr;
	int nsocksize = sizeof(sockaddr_in);
	getpeername(sock, (sockaddr*)&peersockaddr, &nsocksize);
}

//卸载发送钩子
void UnhookWSASend(PVOID mApiAddr, PVOID mOldMem) {

	DWORD flOldProtect = 0;

	//改变HOOK的内存属性
	VirtualProtect(mApiAddr, 5, PAGE_EXECUTE_READWRITE, &flOldProtect);
	memcpy(mApiAddr, mOldMem, 5);
	VirtualProtect(mApiAddr, 5, flOldProtect, &flOldProtect);
}


//hook 登陆-------------------------------------------
void _declspec(naked)MyLogin() {

	_asm {
		jmp mLoginJmp;
	}
}

void WINAPI MyGetLoginData(char * pData, int nSize)
{

	return;
}

void UnhookLogin(PVOID mApiAddr, PVOID mOldMem) {

	DWORD flOldProtect = 0;

	//改变HOOK的内存属性
	VirtualProtect(mApiAddr, 5, PAGE_EXECUTE_READWRITE, &flOldProtect);
	memcpy(mApiAddr, mOldMem, 5);
	VirtualProtect(mApiAddr, 5, flOldProtect, &flOldProtect);
}
//------------------------


//工作线程
void MyWorkThread(LPVOID lp) {

	hWnd = ::FindWindow(NULL, TEXT("CNJ注入工具主程序"));
	if (hWnd) {

		//CWinThread *winthread = AfxBeginThread(MyControllingFunction, NULL);
		//g_ThreadId = winthread->m_nThreadID;
		g_WorkThreadHandle = CreateThread(NULL, 0, MyControllingFunction, NULL, 0, &g_ThreadId);
		if (g_WorkThreadHandle) {

			::SendMessage(hWnd, WM_THREAD_ID, 0, (LPARAM)g_ThreadId);
		}
	}

	//装载钩子
	HMODULE hMod = LoadLibrary("ws2_32.dll");

	if (hMod) {
		//测试exe使用的是WSASend
		pWSASend = GetProcAddress(hMod, "WSASend");
		//微信使用的是send
		//pWSASend = GetProcAddress(hMod, "send");
		mSendJmp = (DWORD)pWSASend + 5;	//返回地址，就是mov ebp, esp下一个指令的地址
										//hook api
		memcpy(OldWSASendAddress, pWSASend, 5);
		MyHookApi(pWSASend, MySend);
	}

	//测试窗口子类化,勾住窗口处理函数
	ApiUtils::hook_windproc(MyWindowProc);
}

//hook函数
void MyHookApi(PVOID mApiAddr, PVOID mOurAddr) {

	DWORD flOldProtect = 0;

	//改变HOOK的内存属性
	VirtualProtect(mApiAddr, 5, PAGE_EXECUTE_READWRITE, &flOldProtect);
	*(PBYTE)mApiAddr = 0xE9;											//jmp远跳转的机器指令的操作码
	*(DWORD*)((DWORD)mApiAddr + 1) = (DWORD)mOurAddr - (DWORD)mApiAddr - 5;	//jmp远跳转的偏移地址
																			//恢复HOOK的内存属性
	VirtualProtect(mApiAddr, 5, flOldProtect, &flOldProtect);
}

//命令接收函数
DWORD WINAPI MyControllingFunction(LPVOID pParam) {

	MSG msg;

	while (TRUE) {

		if (::PeekMessage(&msg, NULL, WM_THREAD_MIN, WM_THREAD_MAX, PM_REMOVE)) {

			switch (msg.message)
			{
			case WM_PAUSE_RECORD:
				bPauseRecord = TRUE;
				//AfxMessageBox(TEXT("接收到控制端的暂停记录的命令"));
				break;
			case WM_RECONTINUE_RECORD:
				bPauseRecord = FALSE;
				break;
			case WM_FREEDLL:
				//_asm int 3
				//UnhookWSASend(pWSASend, OldWSASendAddress);
				//将模块基址发送回控制端
				//::PostMessage(hWnd, WM_FREELIBRARY, 0, (LPARAM)AfxGetInstanceHandle());
				//AfxEndThread(0);
				CloseHandle(g_WorkThreadHandle);
				return 0;
			default:
				break;
			}
		}

		Sleep(100);

	}
	return 0;
}

int C微信助手App::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类

	//断点 使用windbg进行调试时使用
	//_asm int 3

	//结束掉线程
	if (g_WorkThreadHandle) {

		//通知命令接收线程退出
		::PostThreadMessage(g_ThreadId, WM_FREEDLL, 0, 0);

		if (WAIT_TIMEOUT == WaitForSingleObject(g_WorkThreadHandle, 800)) {
			TerminateThread(g_WorkThreadHandle, -1);
			::PostMessage(hWnd, WM_FORCEEXIT, 0, 0);
		}
		else {
			::PostMessage(hWnd, WM_FREESUCCESS, 0, 0);
		}
		g_WorkThreadHandle = NULL;

	}

	//卸载钩子
	UnhookWSASend(pWSASend, OldWSASendAddress);

	//恢复窗口过程
	ApiUtils::unhook_windproc();

	return CWinApp::ExitInstance();
}
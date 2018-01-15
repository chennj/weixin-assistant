#include "stdafx.h"
#include "ApiUtils.h"


LONG ApiUtils::old_windproc = 0;

ApiUtils::PWINDPROC ApiUtils::m_windproc = 0;

ApiUtils::ApiUtils()
{
}


ApiUtils::~ApiUtils()
{
}

//内存读取文本,内存_读文本
char * 
ApiUtils::mem_read_text(DWORD pid, DWORD mem_addr, int size) {

	if (0 >= size) {
		size = 100;
	}

	char * pbuf = new char[size];
	SIZE_T read_size = 0;
	BOOL bret = FALSE;

	HANDLE hPro = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (INVALID_HANDLE_VALUE == hPro) {
		return NULL;
	}

 	bret = ReadProcessMemory(hPro, (LPCVOID)mem_addr, (LPVOID)pbuf, size, &read_size);

	CloseHandle(hPro);

	if (!bret) {
		return NULL;
	}

	return pbuf;
}

//根据名字获取进程ID,进程_名取ID
DWORD 
ApiUtils::get_pid_by_name(CString name) {

	HANDLE hPro = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

	if (INVALID_HANDLE_VALUE != hPro) {

		PROCESSENTRY32 pro32 = { 0 };
		pro32.dwSize = sizeof(PROCESSENTRY32);

		BOOL bRet = Process32First(hPro, &pro32);

		while (bRet) {

			//if (lstrcmpW(pro32.szExeFile, name) == 0) {
			if (name.CompareNoCase(pro32.szExeFile) == 0){
				return pro32.th32ProcessID;
			}

			bRet = Process32Next(hPro, &pro32);
		}

		CloseHandle(hPro);
	}

	return NULL;

}

//获取进程模块基地址,进程_取模块基址
DWORD
ApiUtils::get_module_baseaddr(DWORD pid, CString module_name) {

	if (module_name.IsEmpty())
		return NULL;

	int i;
	int size;
	DWORD dwret;
	std::vector<MODULEENTRY32> vec_pmi;


	if (0 < (size = get_proc_modules_by_pid(pid, vec_pmi))) {

		for (i = 0; i < size; i++) {
			
			if (0 == module_name.CompareNoCase(vec_pmi[i].szModule)) {
				dwret = (DWORD)vec_pmi[i].modBaseAddr;
				return dwret;
			}
		}

		vec_pmi.clear();
	}
	
	return NULL;
}


//获取进程模块句柄数组,进程_ID取模块
//失败为0，成功返回模块数量(该命令为高级成员命令)
int 
ApiUtils::get_proc_modules_by_pid(
	DWORD pid,										//为空 则取当前进程模块
	std::vector<MODULEENTRY32> & vec_pmi			//返回的模块信息 数组
) {

	HANDLE module_handle_snapshot;	//快照句柄
	MODULEENTRY32 tmp_pme;			//模块信息
	BOOL  bret;						//进程句柄

	tmp_pme = { 0 };
	tmp_pme.dwSize = sizeof(MODULEENTRY32);

	vec_pmi.clear();

	module_handle_snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (INVALID_HANDLE_VALUE == module_handle_snapshot) {
		return 0;
	}
	
	bret = ::Module32First(module_handle_snapshot, &tmp_pme);
	while (bret) {

		vec_pmi.push_back(tmp_pme);
		bret = ::Module32Next(module_handle_snapshot, &tmp_pme);
	}

	CloseHandle(module_handle_snapshot);

	return vec_pmi.size();
}

//取自身进程祖句柄
HWND
ApiUtils::get_self_ancestor_handle() {

	DWORD self_pid;
	int i;
	DWORD pid;
	int size;
	std::vector<HWND> vec_hwnd;

	self_pid = ::GetCurrentProcessId();

	size = enum_window_proc(vec_hwnd);

	if (0 <size)
		for (i = 0; i < size; i++) {
			
			GetWindowThreadProcessId(vec_hwnd[i], &pid);
			if (pid == self_pid) {
				return ::GetAncestor(vec_hwnd[i], GA_ROOTOWNER);
			}
		}
	return 0;
}

//枚举窗口句柄
int
ApiUtils::enum_window_proc(std::vector<HWND> & vec_hwnd) {

	BOOL is_success = FALSE;

	vec_hwnd.clear();

	is_success = ::EnumWindows(ApiUtils::EnumWindowsProc_1, (LPARAM)(&vec_hwnd));
	
	if (is_success)
		return vec_hwnd.size();
	else
		return 0;
}

//枚举窗口过程1
BOOL
ApiUtils::EnumWindowsProc_1(HWND hwnd, LPARAM lparam) {

	if (NULL == hwnd)
		return FALSE;
	std::vector<HWND> * pvec_hwnd = (std::vector<HWND>*)lparam;
	pvec_hwnd->push_back(hwnd);
	return TRUE;
}

//窗口子类化,窗口处理函数钩子
LRESULT CALLBACK 
ApiUtils::my_hook_windproc(
	_In_ HWND hwnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	//switch (uMsg) {

	//case WM_LBUTTONDOWN:
	//	AfxMessageBox(TEXT("你点击了窗体"));
	//	break;

	//case WM_SYSCOMMAND:
	//	AfxMessageBox(TEXT("你想销毁窗体?没可能"));
	//	break;

	//case WM_CLOSE:
	//	AfxMessageBox(TEXT("你想结束窗体?没可能"));
	//	break;

	//case WM_MOVE:
	//	AfxMessageBox(TEXT("你移动窗口了!"));
	//	break;
	//}

	if (0 != m_windproc) {

		m_windproc(hwnd, uMsg, wParam, lParam);
	}

	return ::CallWindowProc((WNDPROC)(old_windproc), hwnd, uMsg, wParam, lParam);

}

VOID
ApiUtils::hook_windproc(PWINDPROC youhookproc) {

	m_windproc = youhookproc;
	old_windproc = ::GetWindowLong(get_self_ancestor_handle(), GWLP_WNDPROC);
	::SetWindowLong(get_self_ancestor_handle(), GWLP_WNDPROC, (DWORD)my_hook_windproc);
}

VOID
ApiUtils::unhook_windproc() {

	::SetWindowLong(get_self_ancestor_handle(), GWLP_WNDPROC, (DWORD)old_windproc);
}

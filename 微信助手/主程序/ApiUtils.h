#pragma once

#include <TlHelp32.h>
#include <vector>

class ApiUtils
{

	typedef VOID (* PWINDPROC)(
		_In_ HWND hwnd,
		_In_ UINT uMsg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
		);

public:
	ApiUtils();
	~ApiUtils();

public:
	static char * mem_read_text(DWORD pid, DWORD mem_addr, int size);
	static DWORD get_pid_by_name(CString pname);
	static DWORD get_module_baseaddr(DWORD pid, CString module_name);
	static int get_proc_modules_by_pid(DWORD pid, std::vector<MODULEENTRY32> & vec_pmi);
	static HWND get_self_ancestor_handle();
	static int enum_window_proc(std::vector<HWND> & vec_hwnd);
	
	static VOID hook_windproc(PWINDPROC youhookproc = 0);
	static VOID unhook_windproc();

protected:
	static LRESULT CALLBACK my_hook_windproc(
		_In_ HWND hwnd,
		_In_ UINT uMsg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);

protected:
	static PWINDPROC m_windproc;
	static LONG old_windproc;

private:
	static BOOL CALLBACK EnumWindowsProc_1(HWND hwnd, LPARAM lparam);
};


// 微信助手.h : 微信助手 DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// C微信助手App
// 有关此类实现的信息，请参阅 微信助手.cpp
//

class C微信助手App : public CWinApp
{
public:
	C微信助手App();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

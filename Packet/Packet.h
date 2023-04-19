
// Packet.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

/************************************/
/*			用户自定义消息映射		*/	
/************************************/
#define WM_READ_COM_MSG			WM_USER+1

#include "resource.h"		// 主符号

// CPacketApp:
// 有关此类的实现，请参阅 Packet.cpp
//

class CPacketApp : public CWinApp
{
public:
	CPacketApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CPacketApp theApp;

// Packet.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

/************************************/
/*			�û��Զ�����Ϣӳ��		*/	
/************************************/
#define WM_READ_COM_MSG			WM_USER+1

#include "resource.h"		// ������

// CPacketApp:
// �йش����ʵ�֣������ Packet.cpp
//

class CPacketApp : public CWinApp
{
public:
	CPacketApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPacketApp theApp;
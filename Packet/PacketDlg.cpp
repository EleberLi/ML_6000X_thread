
// PacketDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Packet.h"
#include "PacketDlg.h"
#include "afxdialogex.h"
#include "ML_6000X.h"
#include "SerialComm.h"

#include <Dbt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPacketDlg �Ի���

CPacketDlg::CPacketDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPacketDlg::IDD, pParent)
	, m_radio_SerialPort_On_Off(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPacketDlg::~CPacketDlg()
{
	if (sCom->IsOpenCom)
	{
		sCom->CloseCom();
	}
	delete sCom;

}

void CPacketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo1_Com);
	DDX_Control(pDX, IDC_COMBO2, m_combo2_Baud);
	DDX_Control(pDX, IDC_COMBO3, m_combo3_DataBit);
	DDX_Control(pDX, IDC_COMBO4, m_combo4_StopBit);
	DDX_Control(pDX, IDC_COMBO5, m_combo5_Parity);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
	DDX_Control(pDX, IDC_BUTTON3, m_button3);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_EditB1);
	DDX_Control(pDX, IDC_MFCEDITBROWSE2, m_EditB2);
	DDX_Control(pDX, IDC_RADIO1, m_radio_SerialPort);
	DDX_Control(pDX, IDC_CHECK1, m_checkbox_CheckSum);
}

BEGIN_MESSAGE_MAP(CPacketDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO1, &CPacketDlg::OnBnClickedRadio1)
	ON_WM_DEVICECHANGE()
	ON_BN_CLICKED(IDC_BUTTON3, &CPacketDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CPacketDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CPacketDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK2, &CPacketDlg::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CPacketDlg ��Ϣ�������



BOOL CPacketDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	sCom = new SerialComm();

	/*������*/
	m_combo2_Baud.InsertString(0, "9600");
	m_combo2_Baud.InsertString(1, "19200");
	m_combo2_Baud.InsertString(2, "38400");
	m_combo2_Baud.SetCurSel(0);
	/*����λ*/
	m_combo3_DataBit.InsertString(0, "8");
	m_combo3_DataBit.SetCurSel(0);
	/*ֹͣλ*/
	m_combo4_StopBit.InsertString(0, "1");
	m_combo4_StopBit.SetCurSel(0);
	/*У��λ*/
	m_combo5_Parity.InsertString(0, "None");
	m_combo5_Parity.SetCurSel(0);
	/*����*/
	m_radio_SerialPort.SetWindowTextA("�򿪴���");
	/*У���*/
	m_checkbox_CheckSum.SetWindowTextA("����У���");
	m_radio_SerialPort_On_Off = true;
	m_radio_SerialPort.SetCheck(false);
	RefreshCom();									//serial port automatically detect
	for(int i=IDC_BUTTON1; i<=IDC_BUTTON3; i++)
	{
		GetDlgItem(i)->EnableWindow(false);			//travesal all the pushButton item to make it to grey
	}

	HANDLE thread_hd = CreateThread(NULL, 0, Read_ThreadPro, &Para, 0, NULL);
	SetTimer(1, 100, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPacketDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

BOOL CPacketDlg::OnDeviceChange(UINT nEventType, DWORD dwData)		//����豸�Ľ���ͶϿ�
{
	switch (nEventType)
	{
	case DBT_DEVICEREMOVECOMPLETE: RefreshCom(); break;
	case DBT_DEVICEARRIVAL: RefreshCom(); break;
	default: break;
	}
	return TRUE;
}
void CPacketDlg::RefreshCom(void)									//���´�����Ϣ
{
	CStringArray k;
	int size = 0;
	int index = 0;
	m_combo1_Com.ResetContent();        //����ؼ���������
	for (int i = 1; i < 15; i++)		//���¼���ȫ���ַ���	
	{
		CString str;
		str.Format(_T("COM%d"), i);
		m_combo1_Com.AddString(str);
	}
	EnumSerialPort(k);
	size = k.GetSize();
	for (int i = 0; i < size; i++)
	{
		CString str = k.GetAt(i);
		index = m_combo1_Com.FindStringExact(0, str);			//
		if (index != CB_ERR)
		{
			m_combo1_Com.DeleteString(index);
			m_combo1_Com.InsertString(index, str + "(���ô���)");
		}
	}
	m_combo1_Com.SetCurSel(index);			// dynamically set the max index of available serial port
}
BOOL CPacketDlg::EnumSerialPort(CStringArray& saCom)						//ö��ȫ������
{
	HKEY				hKey;
	DWORD				dwIndex;
	LONG				lResult;
	DWORD				dwType;
	DWORD				dwKeyNameLen;
	CHAR				szKeyName[256];
	DWORD				dwKeyDataLen;
	UCHAR				szKeyData[80];
	CString				strCom, str;
	LPCTSTR				lpSubKey;

	saCom.RemoveAll();

	lpSubKey = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\");

	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_READ, &hKey);

	if (lResult != ERROR_SUCCESS)
	{
		str.Format(_T("[Main] RegOpenKeyEx()������ʧ�ܣ�"));
		AfxMessageBox(str);
		return FALSE;
	}

	dwIndex = 0;

	while (1)
	{
		memset(szKeyName, 0x0, sizeof(szKeyName));
		memset(szKeyData, 0x0, sizeof(szKeyData));

		dwKeyNameLen = sizeof(szKeyName);
		dwKeyDataLen = sizeof(szKeyData);

		lResult = RegEnumValue(hKey, dwIndex++, szKeyName, &dwKeyNameLen, NULL, &dwType, szKeyData, &dwKeyDataLen);

		if ((lResult == ERROR_SUCCESS) || (lResult == ERROR_MORE_DATA))
		{
			strCom = (char*)szKeyData;
			saCom.Add(strCom);
		}
		else
		{
			break;
		}
	}
	RegCloseKey(hKey);
	return true;
}

//���û��϶�С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPacketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
�¼�������
*/

/*���ڿ���*/
void CPacketDlg::OnBnClickedCheck2()
{
	bool temp;
	int port, baud, i;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_checkbox_SerialPort.GetCheck())	//Open SerialPort
	{
		m_checkbox_SerialPort.SetWindowTextA("�رմ���");		//SerialPort Open radiobutton text change "�رմ���"
		for (i = IDC_BUTTON1; i <= IDC_BUTTON3; i++)
		{
			GetDlgItem(i)->EnableWindow(true);			//travesal all the pushButton item to make it to colorful
		}
		port = m_combo1_Com.GetCurSel();			//get m_combo_SerialPort value

		switch (m_combo2_Baud.GetCurSel())				//set BaudRate bps
		{
		case 0:baud = 9600; break;
		case 1:baud = 19200; break;
		case 2:baud = 38400; break;
		}

		temp = sCom->OpenCom(6, baud);					//open SerialPort 
		if (!temp) {

		}

		temp = sCom->SetCom(6, baud);					//set SerialPort parameters
		if (!temp) {

		}
	}
	else			//�رմ���
	{
		m_radio_SerialPort.SetWindowTextA("��������");			//SerialPort Open radiobutton text change "�򿪴���"
		for (i = IDC_COMBO1; i <= IDC_COMBO5; i++)
		{
			GetDlgItem(i)->EnableWindow(true);			//trave all the combo item to make it to colorful
		}
		for (i = IDC_BUTTON1; i <= IDC_BUTTON3; i++)
		{
			GetDlgItem(i)->EnableWindow(false);			//travesal all the pushButton item to make it to grey
		}
		sCom->CloseCom();
	}
}


/*����*/
void CPacketDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//Э����
	ML_6000X *machine_protocol = new ML_6000X();
	CString command;
	m_EditB1.GetWindowTextA(command);
	bool CheckSum = m_checkbox_CheckSum.GetCheck();
	CStringA sendchar = machine_protocol->Pack(command, CheckSum);
	//��������
	if(sCom->WriteCom(sendchar, 512))
	{ 

	}
	delete machine_protocol;
}

/*��շ�����*/
void CPacketDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_EditB1.SetWindowTextA("");
}

/*��ս�����*/
void CPacketDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_EditB2.SetWindowTextA("");
}

/*������*/
DWORD WINAPI CPacketDlg::Read_ThreadPro(LPVOID Para)
{
	CStringA rece;
	char receive[128];
	DWORD  receLength = 0;
	unsigned int i = 0;
	Read_thread_para* Pa = (Read_thread_para*)Para;
	while (1)
	{
		Pa->sCom.ReadCom(receive, sizeof(receive), receLength);
		for (i = 0; i < receLength + 1; i++)
		{
			Pa->rece += receive[i];
		}
	}
}

void CPacketDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnTimer(nIDEvent);

	m_EditB2.SetWindowTextA(rece);
}



// PacketDlg.h : ͷ�ļ�
//
#include "SerialComm.h"
#pragma once

struct Read_thread_para {
	SerialComm sCom;
	CStringA rece;
};

// CPacketDlg �Ի���
class CPacketDlg : public CDialogEx
{
// ����
public:
	CPacketDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CPacketDlg();


// �Ի�������
	enum { IDD = IDD_PACKET_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	SerialComm* sCom;
	CStringA rece;
	Read_thread_para Para;


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	//ʵ�ִ����Ȳ�� 
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);		//����豸�Ľ���ͶϿ�
	afx_msg void RefreshCom(void);									//���´�����Ϣ
	BOOL EnumSerialPort(CStringArray& saCom);						//ö��ȫ������
	DECLARE_MESSAGE_MAP()

public:
	//��ȡ����
	static DWORD WINAPI Read_ThreadPro(LPVOID Para);

	// �˿�
	CComboBox m_combo1_Com;
	// ������
	CComboBox m_combo2_Baud;
	// ����λ
	CComboBox m_combo3_DataBit;
	// ֹͣλ
	CComboBox m_combo4_StopBit;
	// ��żУ��λ
	CComboBox m_combo5_Parity;
	CButton m_radio_SerialPort;
	afx_msg void OnBnClickedRadio1();

	// ����
	CButton m_button1;
	// ��շ�����
	CButton m_button2;
	// ��ս�����
	CButton m_button3;
	// ������
	CMFCEditBrowseCtrl m_EditB1;
	// ������
	CMFCEditBrowseCtrl m_EditB2;
	// У���
	CButton m_checkbox_CheckSum;
	// �ж��Ƿ�򿪴���
	BOOL m_radio_SerialPort_On_Off;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	// ���ڿ���SerialPort Switch
	CButton m_checkbox_SerialPort;
	afx_msg void OnBnClickedCheck2();
};

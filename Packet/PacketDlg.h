
// PacketDlg.h : 头文件
//
#include "SerialComm.h"
#pragma once

struct Read_thread_para {
	SerialComm sCom;
	CStringA rece;
};

// CPacketDlg 对话框
class CPacketDlg : public CDialogEx
{
// 构造
public:
	CPacketDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CPacketDlg();


// 对话框数据
	enum { IDD = IDD_PACKET_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	SerialComm* sCom;
	CStringA rece;
	Read_thread_para Para;


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	//实现串口热插拔 
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);		//检测设备的接入和断开
	afx_msg void RefreshCom(void);									//更新串口信息
	BOOL EnumSerialPort(CStringArray& saCom);						//枚举全部串口
	DECLARE_MESSAGE_MAP()

public:
	//读取串口
	static DWORD WINAPI Read_ThreadPro(LPVOID Para);

	// 端口
	CComboBox m_combo1_Com;
	// 波特率
	CComboBox m_combo2_Baud;
	// 数据位
	CComboBox m_combo3_DataBit;
	// 停止位
	CComboBox m_combo4_StopBit;
	// 奇偶校验位
	CComboBox m_combo5_Parity;
	CButton m_radio_SerialPort;
	afx_msg void OnBnClickedRadio1();

	// 发送
	CButton m_button1;
	// 清空发送区
	CButton m_button2;
	// 清空接收区
	CButton m_button3;
	// 发送区
	CMFCEditBrowseCtrl m_EditB1;
	// 接收区
	CMFCEditBrowseCtrl m_EditB2;
	// 校验和
	CButton m_checkbox_CheckSum;
	// 判断是否打开串口
	BOOL m_radio_SerialPort_On_Off;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	// 串口开关SerialPort Switch
	CButton m_checkbox_SerialPort;
	afx_msg void OnBnClickedCheck2();
};

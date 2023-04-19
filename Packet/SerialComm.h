#pragma once
class SerialComm
{

private:
	OVERLAPPED ReadovReady, WriteovReady;		//异步读写事件句柄
	HANDLE hComm;								//文件句柄

public:
	bool IsOpenCom;								//通信串口是否被打开

	bool OpenCom(int Port, DWORD BaudRate);		//打开通道
	void CloseCom();							//关闭通道
	bool WriteCom(CString sendchar, int sendsize);
	bool ReadCom(char* ReceiveData, int byteToRead, DWORD &ReceiveLength);
	bool SetCom(int Port, DWORD BaudRate);			//这里可以设置的是串口号和波特率

	SerialComm(void);
	SerialComm(const SerialComm& sCom);
	SerialComm& operator=(const SerialComm);

	virtual ~SerialComm(void);
};



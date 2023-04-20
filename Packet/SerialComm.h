#pragma once
class SerialComm
{

private:
	OVERLAPPED ReadovReady, WriteovReady;		//�첽��д�¼����
	HANDLE hComm;								//�ļ����

public:
	bool IsOpenCom;								//ͨ�Ŵ����Ƿ񱻴�

	bool OpenCom(int Port, DWORD BaudRate);		//��ͨ��
	void CloseCom();							//�ر�ͨ��
	bool WriteCom(CString sendchar, int sendsize);
	bool ReadCom(char* ReceiveData, int byteToRead, DWORD &ReceiveLength);
	bool SetCom(int Port, DWORD BaudRate);			//����������õ��Ǵ��ںźͲ�����

	SerialComm(void);
	SerialComm(const SerialComm& sCom);
	SerialComm& operator=(const SerialComm);

	virtual ~SerialComm(void);
};



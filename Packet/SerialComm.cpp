#include "StdAfx.h"
#include "SerialComm.h"

const int Read_TimeOut = 1000;
const int Write_TimeOut = 1000;

SerialComm::SerialComm()
{
	IsOpenCom = false;
}

SerialComm::SerialComm(const SerialComm& sCom)
{
	
}

SerialComm::~SerialComm(void)
{
	this->CloseCom();
}

bool SerialComm::OpenCom(int Port, DWORD BaudRate)
{
	if (IsOpenCom)			//����Ѿ�������رմ���
    {
        this->CloseCom();
        IsOpenCom = false;
    }
	CString ComNo;
	ComNo.Format("COM%d",Port);
	/*����һ��COM%���ļ����ɶ���д, �첽ͨ�ţ�������򿪷����½���*/
	hComm = CreateFile( ComNo,										//COM%d							
						GENERIC_READ|GENERIC_WRITE,					//Readable��Writeable
						0,
						NULL,
						OPEN_EXISTING,								//Create if not exist
						FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,	//asynchronous
						NULL
						);

	if (hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}        
    if (!SetupComm(hComm, 1024, 512))        
		return false;
    
    COMMTIMEOUTS commtimeouts;\
    commtimeouts.ReadIntervalTimeout = 2;
    commtimeouts.ReadTotalTimeoutConstant =2;
    commtimeouts.ReadTotalTimeoutMultiplier =2;
    commtimeouts.WriteTotalTimeoutConstant =2;
    commtimeouts.WriteTotalTimeoutMultiplier=2;
    
    if (!SetCommTimeouts(hComm, &commtimeouts))
		return false;

    memset(&ReadovReady,0,sizeof(OVERLAPPED));
    memset(&WriteovReady,0,sizeof(OVERLAPPED));
    ReadovReady.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);		//������ɼ̳У��ֶ���ԭ�¼������ź�״̬�� ���ź�״̬�������¼�
    WriteovReady.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);	//������ɼ̳У��ֶ���ԭ�¼������ź�״̬�� ���ź�״̬�������¼�
    
    SECURITY_ATTRIBUTES sa;
    sa.nLength=sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor=NULL;
    sa.bInheritHandle=TRUE;
/*  
    DCB dcb;
    GetCommState(hComm, &dcb);
    dcb.fBinary = TRUE;
    dcb.fParity = TRUE;
    dcb.BaudRate = CBR_9600;         // ������ 9600
    dcb.ByteSize = 8;          z      // 8 λ����λ
    dcb.Parity = NOPARITY;            // ��У��
    dcb.StopBits = ONESTOPBIT;        // 1 ��ֹͣλ
    if (!SetCommState(hComm, &dcb ))return false;
*/    
    IsOpenCom = true;
    return IsOpenCom;	
}

bool SerialComm::SetCom(int Port, DWORD BaudRate)
{
	if (!IsOpenCom) return false;			//���ڱ�����������ֵ��
	DCB dcb;
	GetCommState(hComm, &dcb);
	dcb.BaudRate = BaudRate;	//��������9600��19200, 38400
	//dcb.Parity = NOPARITY;		//û����żУ��
	dcb.ByteSize = 8;			//8������λ
	dcb.StopBits = 0;			//һ��ֹͣλ
	if (!SetCommState(hComm, &dcb))
	{
		int temp = GetLastError();
		return false;			//���ô���״̬ʧ��

	}
	return true;				//���ô���״̬�ɹ�
}

void SerialComm:: CloseCom()
{
	CloseHandle(hComm);		//�ر��ļ����
	hComm = NULL;

	CloseHandle(ReadovReady.hEvent);		//�رն��¼������������Linux�Ķ�д�ļ�������
	ReadovReady.hEvent = NULL;
	CloseHandle(WriteovReady.hEvent);		//�ر�д�¼����
	WriteovReady.hEvent = NULL;
	IsOpenCom = false;
}

bool SerialComm::WriteCom(CStringA sendchar, int sendsize) //��ð������������sendchar, sendsize��Ӧ������ַ�������
{
	if(!IsOpenCom)		//û�б����򷵻�false
	{
		return false;
	}
	DWORD BytesSent, resD;

	PurgeComm(hComm, PURGE_TXCLEAR|PURGE_TXABORT);			//������뻺��
	BytesSent = 0;
	BOOL wfd = WriteFile(hComm,						//�򿪵Ĵ��ھ�����ļ���������
						sendchar,					//���͵�����
						sendsize,			//���ݳ���
						&BytesSent,		//
						&WriteovReady);			//
	if(!wfd)		//д���쳣�͹���״̬������£�����ֱ�ӷ��صĴ��󣬣�
	{
		if(GetLastError()!=ERROR_IO_PENDING)		//�����飬
		{						//IOֱ�ӷ��ش���
			return false;				
		}
		else							
		{						//IO�ȴ�����(�ȴ���һ���豸��д���)
			resD = WaitForSingleObject(WriteovReady.hEvent, 1000);			//�ȴ�������Ը����һ���ȴ�״̬��ֱ���ض��ں˶������Ѿ�֪ͨ״̬
		}

		switch(resD)
		{
		case WAIT_OBJECT_0:			//resD == WAIT_OBJECT_0 ָ���Ķ����Ѿ���ɣ������ͷ���
			{
				if(!GetOverlappedResult(hComm, &WriteovReady, &BytesSent, false))
					return false;
				else
					return true;	//GetOverlappedResult�ɹ�
			}
		default:					//resD == ��WAIT_OBJECT_0�򷵻�false
			return false;
			break;
		}
	}
	return true;
}

bool SerialComm::ReadCom(char* ReceiveData, int byteToRead, DWORD &ReceiveLength)
{
	DWORD dwRes;
	if(!IsOpenCom)	return false;
	if(ReadovReady.hEvent == NULL) return false;

	ReceiveLength = 0;
	//PurgeComm(hComm, PURGE_RXCLEAR | PURGE_RXABORT);
	BOOL rfd = ReadFile(hComm, ReceiveData, byteToRead, &ReceiveLength, &ReadovReady);			//rfd �յ��˴���ķ���ֵ

	if (rfd == FALSE) 
	{
		dwRes = GetLastError();
		if (dwRes != ERROR_IO_PENDING)
			return false;
	}

	dwRes = WaitForSingleObject(ReadovReady.hEvent, Read_TimeOut);
	switch(dwRes)
	{
	case WAIT_OBJECT_0:					//resD == WAIT_OBJECT_0 ָ���Ķ����Ѿ��������ź�
		if (!GetOverlappedResult(hComm, &ReadovReady, &ReceiveLength, FALSE))    return false;
		break;

	case WAIT_TIMEOUT:					//��ʱ
		break;                

	default:
		break;
	}

	if(ReceiveLength == 0)    return false;
	ReceiveData[ReceiveLength] = 0;
	PurgeComm(hComm, PURGE_RXCLEAR | PURGE_RXABORT);		//��ֹд������������������
	return true;
}




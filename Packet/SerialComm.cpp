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
	if (IsOpenCom)			//如果已经开启则关闭串口
    {
        this->CloseCom();
        IsOpenCom = false;
    }
	CString ComNo;
	ComNo.Format("COM%d",Port);
	/*创建一个COM%的文件，可读可写, 异步通信，存在则打开否则新建，*/
	hComm = CreateFile( ComNo,										//COM%d							
						GENERIC_READ|GENERIC_WRITE,					//Readable，Writeable
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
    ReadovReady.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);		//句柄不可继承，手动复原事件到无信号状态， 无信号状态，无名事件
    WriteovReady.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);	//句柄不可继承，手动复原事件到无信号状态， 无信号状态，无名事件
    
    SECURITY_ATTRIBUTES sa;
    sa.nLength=sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor=NULL;
    sa.bInheritHandle=TRUE;
/*  
    DCB dcb;
    GetCommState(hComm, &dcb);
    dcb.fBinary = TRUE;
    dcb.fParity = TRUE;
    dcb.BaudRate = CBR_9600;         // 波特率 9600
    dcb.ByteSize = 8;          z      // 8 位数据位
    dcb.Parity = NOPARITY;            // 奇校验
    dcb.StopBits = ONESTOPBIT;        // 1 个停止位
    if (!SetCommState(hComm, &dcb ))return false;
*/    
    IsOpenCom = true;
    return IsOpenCom;	
}

bool SerialComm::SetCom(int Port, DWORD BaudRate)
{
	if (!IsOpenCom) return false;			//串口被打开则不能设置值，
	DCB dcb;
	GetCommState(hComm, &dcb);
	dcb.BaudRate = BaudRate;	//波特率是9600，19200, 38400
	//dcb.Parity = NOPARITY;		//没有奇偶校验
	dcb.ByteSize = 8;			//8个数据位
	dcb.StopBits = 0;			//一个停止位
	if (!SetCommState(hComm, &dcb))
	{
		int temp = GetLastError();
		return false;			//设置串口状态失败

	}
	return true;				//设置串口状态成功
}

void SerialComm:: CloseCom()
{
	CloseHandle(hComm);		//关闭文件句柄
	hComm = NULL;

	CloseHandle(ReadovReady.hEvent);		//关闭读事件句柄，类似与Linux的读写文件描述符
	ReadovReady.hEvent = NULL;
	CloseHandle(WriteovReady.hEvent);		//关闭写事件句柄
	WriteovReady.hEvent = NULL;
	IsOpenCom = false;
}

bool SerialComm::WriteCom(CStringA sendchar, int sendsize) //打好包的请求包放在sendchar, sendsize对应请求包字符串长度
{
	if(!IsOpenCom)		//没有被打开则返回false
	{
		return false;
	}
	DWORD BytesSent, resD;

	PurgeComm(hComm, PURGE_TXCLEAR|PURGE_TXABORT);			//清空输入缓存
	BytesSent = 0;
	BOOL wfd = WriteFile(hComm,						//打开的串口句柄（文件描述符）
						sendchar,					//发送的数据
						sendsize,			//数据长度
						&BytesSent,		//
						&WriteovReady);			//
	if(!wfd)		//写入异常和挂起状态的情况下（包括直接返回的错误，）
	{
		if(GetLastError()!=ERROR_IO_PENDING)		//错误检查，
		{						//IO直接返回错误
			return false;				
		}
		else							
		{						//IO等待挂起(等待上一个设备读写完成)
			resD = WaitForSingleObject(WriteovReady.hEvent, 1000);			//等待函数自愿进入一个等待状态，直到特定内核对象变成已经通知状态
		}

		switch(resD)
		{
		case WAIT_OBJECT_0:			//resD == WAIT_OBJECT_0 指定的对象已经完成，并且释放了
			{
				if(!GetOverlappedResult(hComm, &WriteovReady, &BytesSent, false))
					return false;
				else
					return true;	//GetOverlappedResult成功
			}
		default:					//resD == ！WAIT_OBJECT_0则返回false
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
	BOOL rfd = ReadFile(hComm, ReceiveData, byteToRead, &ReceiveLength, &ReadovReady);			//rfd 收到了错误的返回值

	if (rfd == FALSE) 
	{
		dwRes = GetLastError();
		if (dwRes != ERROR_IO_PENDING)
			return false;
	}

	dwRes = WaitForSingleObject(ReadovReady.hEvent, Read_TimeOut);
	switch(dwRes)
	{
	case WAIT_OBJECT_0:					//resD == WAIT_OBJECT_0 指定的对象已经发出了信号
		if (!GetOverlappedResult(hComm, &ReadovReady, &ReceiveLength, FALSE))    return false;
		break;

	case WAIT_TIMEOUT:					//超时
		break;                

	default:
		break;
	}

	if(ReceiveLength == 0)    return false;
	ReceiveData[ReceiveLength] = 0;
	PurgeComm(hComm, PURGE_RXCLEAR | PURGE_RXABORT);		//终止写操作，清空输出缓冲区
	return true;
}




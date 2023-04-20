#pragma once
#include <cstring>

enum Command						//CH��channel����д
{
    Release,								//0000 ��ʱģʽ��һ������Ͱ�ʱ���趨�ͷ����壬�ֶ�ģʽ����һ�ο�ʼ�ͷţ��ڶ��ν����ͷ�
	StepSignal,								//0001
	PresetSignal,							//0002
	CHSet,									//0003
	CHCopy,									//0004
	UsrCountRst,							//0005
	AllCHRst,								//0006 ȫ��ͨ���ָ�Ĭ��ֵ
	RstExcpLangNComSpeed,					//0007 ����ȫ�����ó������Ժ�ͨ���ٶ�
	ReleasePressZero,						//0008 �³�ѹ������
	VaccumePressZero,						//0009 ���ѹ������
	/*Set Channel Release Parameter*/
	SetCHReleaseTimePara,					//1000 �����³�ģʽ ,ʱ��ĵ�λ����ֵ
	SetCHReleasePressPara,					//1001 �����³�������ѹ��������ֵ
	SetCHReleaseLogPara,					//1002 ͳһ����ָ�����³�ͨ������־���
	SetCHReleasePressALM,					//1003 ����ѹ�����湦��
	SetMaterialALM,							//1004 ����ʣ�������湦��
	SetKeyLock,								//1005 ���ð��������Լ�����
	SetPeriodReleasePara,					//1006 ���ü���³�������
	SetEnvPara1,							//1007 ����ѹ����λ�����ԣ�����У��λ
	SetEnvPara2,							//1008 3P&16P,���������Ϣ��Enable���ܣ�10P&23P
	SetMagLock,								//1009 ���õ�ŷ��ٽ�ֵ
	SetDate,								//1010
	/*Get Channel Release Parameter*/
	GetCHReleaseTimePara,					//2000 ����³�ģʽ ,ʱ��ĵ�λ����ֵ
	GetCHReleasePressPara,					//2001 ����³�������ѹ��������ֵ
	GetCHReleaseLogPara,				    //2002 ͳһ����ָ�����³�ͨ������־���
	GetCHReleasePressALM,					//2003 ����ѹ�����湦��
	GetMaterialALM,							//2004 ����ʣ�������湦��
    GetKeyLock,								//2005 ��ð��������Լ�����
	GetPeriodReleasePara,					//1006 ���ü���³�������
	GetEnvPara1,							//2007 ����ѹ����λ�����ԣ�����У��λ
	GetEnvPara2,							//2008 3P&16P,���������Ϣ��Enable���ܣ�10P&23P
	GetMagLock,								//2009 ���õ�ŷ��ٽ�ֵ
	GetDate,								//2010 
	/*Get Measure Parameter*/
	GetCtlStatus,							//3000 ��ȡ������״̬
	GetPressValue,							//3001 ��ȡѹ��ֵ
	GetReleaseCH,							//3002 ��ȡ�����³���ͨ����
	GetALMStatus,							//3003 ��ȡ����״̬
	GetCounterValue,						//3004 ��ȡ������ֵ
	GetSecond,								//3005 ��ȡ���ֵ
	GetNewLog,								//3006 ��ȡ�仯����־���ֵ�����µ���־��
	GetPointLog,							//3007 ��ȡָ������־
	/*Get Commend Infomation*/
	GetCommandInfo,							//9998��ȡ������Ϣ
	GetMachineInfo							//9999��ȡ������Ϣ
};	

class ML_6000X
{

private:
    const char* SendHeader;
    const char* ReceHeader;
	const char* tail;
	const char* delimiter;
	int CheckSum;

	CString command;
	char *data[7];
public:
	ML_6000X(void);
	~ML_6000X(void);
	CString Pack(CString command, int *data, bool CheckSum_ON);
	CString Pack(CString command, bool CheckSum_ON);
	void UnPack();
	CString CompareSum(CString command, int *data);
	int OCTtoHEX(int oct);
};


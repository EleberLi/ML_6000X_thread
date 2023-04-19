#pragma once
#include <cstring>

enum Command						//CH是channel的缩写
{
    Release,								//0000 定时模式，一次命令就按时间设定释放流体，手动模式，第一次开始释放，第二次结束释放
	StepSignal,								//0001
	PresetSignal,							//0002
	CHSet,									//0003
	CHCopy,									//0004
	UsrCountRst,							//0005
	AllCHRst,								//0006 全部通道恢复默认值
	RstExcpLangNComSpeed,					//0007 重置全部设置除了语言和通信速度
	ReleasePressZero,						//0008 吐出压力归零
	VaccumePressZero,						//0009 真空压力归零
	/*Set Channel Release Parameter*/
	SetCHReleaseTimePara,					//1000 设置吐出模式 ,时间的单位和数值
	SetCHReleasePressPara,					//1001 设置吐出的正负压上下限数值
	SetCHReleaseLogPara,					//1002 统一设置指定的吐出通道的日志输出
	SetCHReleasePressALM,					//1003 设置压力警告功能
	SetMaterialALM,							//1004 设置剩余量警告功能
	SetKeyLock,								//1005 设置按键锁，以及密码
	SetPeriodReleasePara,					//1006 设置间隔吐出的条件
	SetEnvPara1,							//1007 设置压力单位，语言，有无校验位
	SetEnvPara2,							//1008 3P&16P,报错输出信息，Enable功能，10P&23P
	SetMagLock,								//1009 设置电磁阀临界值
	SetDate,								//1010
	/*Get Channel Release Parameter*/
	GetCHReleaseTimePara,					//2000 获得吐出模式 ,时间的单位和数值
	GetCHReleasePressPara,					//2001 获得吐出的正负压上下限数值
	GetCHReleaseLogPara,				    //2002 统一设置指定的吐出通道的日志输出
	GetCHReleasePressALM,					//2003 设置压力警告功能
	GetMaterialALM,							//2004 设置剩余量警告功能
    GetKeyLock,								//2005 获得按键锁，以及密码
	GetPeriodReleasePara,					//1006 设置间隔吐出的条件
	GetEnvPara1,							//2007 设置压力单位，语言，有无校验位
	GetEnvPara2,							//2008 3P&16P,报错输出信息，Enable功能，10P&23P
	GetMagLock,								//2009 设置电磁阀临界值
	GetDate,								//2010 
	/*Get Measure Parameter*/
	GetCtlStatus,							//3000 获取控制器状态
	GetPressValue,							//3001 获取压力值
	GetReleaseCH,							//3002 获取现在吐出的通道号
	GetALMStatus,							//3003 获取报警状态
	GetCounterValue,						//3004 获取计数器值
	GetSecond,								//3005 获取秒表值
	GetNewLog,								//3006 获取变化点日志最大值（最新的日志）
	GetPointLog,							//3007 获取指定的日志
	/*Get Commend Infomation*/
	GetCommandInfo,							//9998获取命令信息
	GetMachineInfo							//9999获取本机信息
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


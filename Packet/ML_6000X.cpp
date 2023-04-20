#include "StdAfx.h"
#include "ML_6000X.h"
#include <cstdio>

char* command[42] = {
	"0000",
	"0001",
	"0002",
	"0003",
	"0004",
	"0005",
	"0006",
	"0007",
	"0008",
	"0009",
	"1000",
	"1001",
	"1002",
	"1003",
	"1004",
	"1005",
	"1006",
	"1007",
	"1008",
	"1009",
	"1010",
	"2000",
	"2001",
	"2002",
	"2003",
	"2004",
	"2005",
	"2006",
	"2007",
	"2008",
	"2009",
	"2010",
	"3000",
	"3001",
	"3002",
	"3003",
	"3004",
	"3005",
	"3006",
	"3007",
	"9998",
	"9999"
};

ML_6000X::ML_6000X(void):SendHeader("M"),ReceHeader("A"),tail("\r\n"),delimiter(","),CheckSum(0x00)
{
	
}

ML_6000X::~ML_6000X(void)
{
}

CString ML_6000X::Pack(CString command, int *data, bool CheckSum_ON)			//command��ö����command��ֵ��data���������飬��󳤶���7
{
	CString temp;
	int i=1;
	this->command = this->SendHeader;			//M
	this->command += delimiter;					//�ָ���,
	this->command += command;					//����
	this->command += delimiter;					//�ָ���,
	/*  �������飬������߸������ַ���  */
	for(i=1; i<=sizeof(data); i++)				
	{
		sprintf_s(this->data[i], strlen(this->data[i]), "%d", data[i]);		//ת��ΪASCII��
		this->command += this->data[i];			//���ݵ��ַ���
		this->command += delimiter;				//�ָ���
	}
	if(CheckSum_ON)
	{
		for (int i = 0; i < this->command.GetLength(); i++)
		{
			CheckSum -= this->command.GetAt(i);
		}
		this->command.Format(_T("%s"), CheckSum);
	}
	return this->command;	
}

CString ML_6000X::Pack(CString command, bool CheckSum_ON)			//command��ö����command��ֵ��data���������飬��󳤶���7
{
	CString temp;
	int i;
	this->command.Format(_T("%s"), this->SendHeader);			//M
	this->command += this->delimiter;									//,
	this->command += command;											//����
	//this->command += this->delimiter;									//,
	/*  �������飬������߸������ַ���  */
	
	if (CheckSum_ON)
	{
		/*  �������飬������߸������ַ���  */
		for (i = 0; i < this->command.GetLength(); i++)					//data[]
		{
			CheckSum -= this->command.GetAt(i);
		}
		temp.Format(_T("%x"), CheckSum);		//У���
		temp = temp.Right(2);
		this->command += temp;
		this->command += this->tail;					//\r\n
	}
	return this->command;
}

void ML_6000X::UnPack()								
{

}

CString ML_6000X::CompareSum(CString command, int *data)
{	
	return 0;
}

int ML_6000X::OCTtoHEX(int oct)
{
	return 0;
}
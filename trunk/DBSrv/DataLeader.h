#pragma once

#include "SServerStd.h"

class DataLeader : public SSingleton <DataLeader>
{
private:
	friend class SSingleton<DataLeader>;

public:
	//--------------------------------------------------------------
	int		SessionCount;
	//--------------------------------------------------------------
	char	LogSrvIP[15];
	int		LogSrvPortNum;

public:
	DataLeader(void);
	~DataLeader(void);

public:
	BOOL DataSetting();
	//--------------------------------------------------------------

};

#define GetDocument DataLeader::GetInstance()


#pragma once

#include "SServerStd.h"

class Document;
class SLogger;
class FieldSrvMgr;

class WorldMain
{
private:
	//======================================
	// singleTon
	//======================================
	Document*			m_document;
	SLogger*			m_logger;

public:
	WorldMain(void);
	~WorldMain(void);

	//�ʱ�ȭ & ����
	BOOL Init();
	void Release();
};

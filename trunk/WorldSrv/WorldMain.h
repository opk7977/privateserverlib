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

	//초기화 & 해제
	BOOL Init();
	void Release();
};

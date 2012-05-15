#pragma once

#include "SServerStd.h"

class Network;
class LoginDB;
class DataLeader;

class LoginMain
{
private:
	//======================================
	// singleTon객체들
	//======================================
	Network*		m_network;
	LoginDB*		m_dbMgr;
	DataLeader*		m_document;
	//======================================

public:
	LoginMain(void);
	~LoginMain(void);

	// 초기화
	BOOL Init();

	void Relase();
};

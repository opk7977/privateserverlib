#pragma once

#include "SServerStd.h"

class Network;
class LoginDB;
class DataLeader;

class LoginMain
{
private:
	//======================================
	// singleTon��ü��
	//======================================
	Network*		m_network;
	LoginDB*		m_dbMgr;
	DataLeader*		m_document;
	//======================================

public:
	LoginMain(void);
	~LoginMain(void);

	// �ʱ�ȭ
	BOOL Init();

	void Relase();
};

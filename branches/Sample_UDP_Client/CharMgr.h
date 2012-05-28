#pragma once

#include "SServerStd.h"
#include "SServerObj.h"

#include "NObject.h"

class NInput;
class NTime;

class CharMgr : public SSingleton <CharMgr>, public SServerObj
{
private:
	friend class SSingleton<CharMgr>;

private:
	// ��ü ĳ����
	ATL::CAtlMap< int, NObject*>			m_player;

	// ���� ��
	int										m_myIndex;
	NObject									m_heroObj;

	//======================================
	// singleTon ��ü��
	//======================================
	NInput*				m_pInput;
	NTime*				m_pTime;

private:
	CharMgr(void);
	~CharMgr(void);

public:
	void Init();
	void Release();
	void Frame();
	void Render();
};

#define GetCharMgr CharMgr::GetInstance()

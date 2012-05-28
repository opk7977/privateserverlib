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
	// 전체 캐릭터
	ATL::CAtlMap< int, NObject*>			m_player;

	// 나요 나
	int										m_myIndex;
	NObject									m_heroObj;

	//======================================
	// singleTon 객체들
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

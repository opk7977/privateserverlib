#pragma once

#include "SServerStd.h"
#include "SServerObj.h"

#include "NObject.h"

class NInput;
class NTime;
class PacketSender;

class GameChar : public NObject, public SServerObj
{
private:
	int m_SessionId;

public:
	void SetId( int id ) { m_SessionId = id; }
	int GetId() { return m_SessionId; }

};

class CharMgr : public SSingleton <CharMgr>, public SServerObj
{
private:
	friend class SSingleton<CharMgr>;

private:
	// 전체 캐릭터
	ATL::CAtlMap< int, NObject*>			m_player;

	// 나요 나
	int										m_myIndex;
	GameChar*								m_heroObj;

	//======================================
	// singleTon 객체들
	//======================================
	NInput*									m_pInput;
	NTime*									m_pTime;
	PacketSender*							m_sender;

private:
	CharMgr(void);
	~CharMgr(void);

public:
	void Init();
	void Release();
	void Frame();
	void Render();

	void AddMe( int _id );
	void AddChar( int _id );
	void DelChar( int _id );

	NObject* FindChar( int _id );
};

#define GetCharMgr CharMgr::GetInstance()

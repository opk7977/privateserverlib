#pragma once

#include "SServerStd.h"
#include "Character.h"

class CCharacterMgr : public SSingleton <CCharacterMgr>
{
private:
	friend class SSingleton<CCharacterMgr>;

private:
	//캐릭터 list(?) or map(?)
	std::map<DWORD, CCharacter*>	m_playerList;

	//hero(?)의 ID
	int						m_iHeroID;

	//캐릭터 개수
	int						m_iCount;
	//play하는 게임에 인원
	int						m_iPlayerCountIN;

	CRITICAL_SECTION		m_crtChar;

public:
	CCharacterMgr(void);
	~CCharacterMgr(void);
	
	//그리티컬 섹션
	void StartCritical();
	void EndCritical();

	//player인원
	void SetPlayerCount( int count );
	int GetPlayerCount();

	//현재 player인원
	int GetNowPlayerCount();

	//캐릭터 생성함수
	void CreateCharacter( int id, int x, int y, int z, int dir, int state );

	//캐릭터 삭제함수
	void DeleteCharacter( int id );

	//캐릭터 이동
	void MoveCharacter();

	//캐릭터들 모두 랜더
	void RenderAll( HDC* dc );

	//캐릭터를 ID값으로 찾아 return받기
	CCharacter* FindChar( int _index );

	//hero
	void SetHeroId( int id );
	int GetHeroId();
	CCharacter* GetHero();

	//모두 삭제 한다.
	void Release();
};

#define GetCharMgr CCharacterMgr::GetInstance()



#pragma once

#include "SServerStd.h"
#include "Character.h"

#define itrCHARACTER std::map<DWORD, CCharacter*>::iterator

class CCharacterMgr : public SSingleton <CCharacterMgr>
{
private:
	friend class SSingleton<CCharacterMgr>;

private:
	//캐릭터 list(?) or map(?)
	std::map<DWORD, CCharacter*>	m_playerList;

	//캐릭터 개수
	int						m_iCount;

	CRITICAL_SECTION		m_crtChar;

public:
	CCharacterMgr(void);
	~CCharacterMgr(void);

	//그리티컬 섹션
	void StartCritical();
	void EndCritical();

	//캐릭터 생성함수
	void CreateCharacter( int id, int x, int y, TCHAR* str );

	//캐릭터 삭제함수
	void DeleteCharacter( int id );

	//캐릭터 이동
	void MoveCharacter();

	//캐릭터들 모두 랜더
	void RenderAll( HDC* dc );

	//캐릭터를 ID값으로 찾아 return받기
	CCharacter* FindChar( int _index );

	//헤더를 보낸다
	itrCHARACTER GetHeader();
	
	//끝인지를 확인해 준다.
	//끝이면 TRUE
	BOOL IsEnd( itrCHARACTER pos );

	//접속해 있는 캐릭터의 개수(?)
	int GetCount();
};

#define GetCharMgr CCharacterMgr::GetInstance()



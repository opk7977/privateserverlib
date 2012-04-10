#ifndef __CHARACTER_MANAGER__
#define __CHARACTER_MANAGER__

#include "SServerStd.h"
#include "Character.h"

class CharacterMgr : public SSingleton <CharacterMgr>
{
private:
	friend class SSingleton<CharacterMgr>;

private:
	//캐릭터를 담고 있는 map( key 값고 캐릭터의 Id값은 동일하다 )
	std::map< DWORD, Character* >	m_mapCharacter;
	//캐릭터 인원(?)ㅋㅋ
	int								m_charCount;
private:
	CharacterMgr(void);
	~CharacterMgr(void);

public:
	//캐릭터 생성함수
	void CreateCharacter( int count );

	//캐릭터 움직임
	void MoveCharacter();

	//캐릭터들 모두 랜더
	void RenderAll( HDC dc );

	//캐릭터를 ID값으로 찾아 return받기
	Character* FindChar( int _index );


public:

};

#define GetCharMgr CharacterMgr::GetInstance()

#endif


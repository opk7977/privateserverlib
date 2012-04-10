#ifndef __CHARACTER_MANAGER__
#define __CHARACTER_MANAGER__

#include "SServerStd.h"
#include "Character.h"

class CharacterMgr : public SSingleton <CharacterMgr>
{
private:
	friend class SSingleton<CharacterMgr>;

private:
	//ĳ���͸� ��� �ִ� map( key ���� ĳ������ Id���� �����ϴ� )
	std::map< DWORD, Character* >	m_mapCharacter;
	//ĳ���� �ο�(?)����
	int								m_charCount;
private:
	CharacterMgr(void);
	~CharacterMgr(void);

public:
	//ĳ���� �����Լ�
	void CreateCharacter( int count );

	//ĳ���� ������
	void MoveCharacter();

	//ĳ���͵� ��� ����
	void RenderAll( HDC dc );

	//ĳ���͸� ID������ ã�� return�ޱ�
	Character* FindChar( int _index );


public:

};

#define GetCharMgr CharacterMgr::GetInstance()

#endif


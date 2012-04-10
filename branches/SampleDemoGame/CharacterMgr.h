#pragma once

#include "SServerStd.h"
#include "Character.h"

class CCharacterMgr : public SSingleton <CCharacterMgr>
{
private:
	friend class SSingleton<CCharacterMgr>;

private:
	//ĳ���� list(?) or map(?)
	std::map<DWORD, CCharacter*>	m_playerList;

	//hero(?)�� ID
	int						m_iHeroID;

	//ĳ���� ����
	int						m_iCount;

	CRITICAL_SECTION		m_crtChar;

public:
	CCharacterMgr(void);
	~CCharacterMgr(void);
	
	//�׸�Ƽ�� ����
	void StartCritical();
	void EndCritical();

	//ĳ���� �����Լ�
	void CreateCharacter( int id, int x, int y, TCHAR* str );

	//ĳ���� �����Լ�
	void DeleteCharacter( int id );

	//ĳ���� �̵�
	void MoveCharacter();

	//ĳ���͵� ��� ����
	void RenderAll( CDC* dc );

	//ĳ���͸� ID������ ã�� return�ޱ�
	CCharacter* FindChar( int _index );

	//hero
	void SetHeroId( int id );
	int GetHeroId();
	CCharacter* GetHero();

	//��� ���� �Ѵ�.
	void Release();
};

#define GetCharMgr CCharacterMgr::GetInstance()



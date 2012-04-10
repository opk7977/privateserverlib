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
	//play�ϴ� ���ӿ� �ο�
	int						m_iPlayerCountIN;

	CRITICAL_SECTION		m_crtChar;

public:
	CCharacterMgr(void);
	~CCharacterMgr(void);
	
	//�׸�Ƽ�� ����
	void StartCritical();
	void EndCritical();

	//player�ο�
	void SetPlayerCount( int count );
	int GetPlayerCount();

	//���� player�ο�
	int GetNowPlayerCount();

	//ĳ���� �����Լ�
	void CreateCharacter( int id, int x, int y, int z, int dir, int state );

	//ĳ���� �����Լ�
	void DeleteCharacter( int id );

	//ĳ���� �̵�
	void MoveCharacter();

	//ĳ���͵� ��� ����
	void RenderAll( HDC* dc );

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



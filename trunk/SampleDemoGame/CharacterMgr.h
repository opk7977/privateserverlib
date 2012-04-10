#pragma once

#include "SServerStd.h"
#include "Character.h"

#define itrCHARACTER std::map<DWORD, CCharacter*>::iterator

class CCharacterMgr : public SSingleton <CCharacterMgr>
{
private:
	friend class SSingleton<CCharacterMgr>;

private:
	//ĳ���� list(?) or map(?)
	std::map<DWORD, CCharacter*>	m_playerList;

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
	void RenderAll( HDC* dc );

	//ĳ���͸� ID������ ã�� return�ޱ�
	CCharacter* FindChar( int _index );

	//����� ������
	itrCHARACTER GetHeader();
	
	//�������� Ȯ���� �ش�.
	//���̸� TRUE
	BOOL IsEnd( itrCHARACTER pos );

	//������ �ִ� ĳ������ ����(?)
	int GetCount();
};

#define GetCharMgr CCharacterMgr::GetInstance()



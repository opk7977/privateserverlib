#pragma once

#include "SThread.h"
#include "STime.h"
#include "SList.h"

class GameSession;
class ItemObj;
class SPacket;
class CharObj;

class GameProc : public SThread
{
private:
	// �ڽ��� ID(���ȣ(key��))
	int						m_id;

// 	// ������ �ϰ� �ִ� player list
// 	std::list<GameSession*>	m_listPlayer;
// 	// ������ ���� �� �ο�
// 	int						m_inGamePlayerCount;

	SList<GameSession*>		m_listPlayer;

	// ������ ���� list
	SList<ItemObj*>			m_listItem;

	// ������ play�ϴ� �ο�(�κ񿡼� �޾ƿ� �ο�)
	int						m_playerCount;

	// �� �� ų��
	int						m_AttKillCount;
	int						m_DefKillCount;

	// ������ flag
	BOOL					m_nowIsPlaying;

	// �ð�
	STime					m_timer;

	// �ð�Count
	//

	// ������ �̺�Ʈ �ڵ�
	HANDLE					m_hStartGame;

public:
	GameProc(void);
	~GameProc(void);

	// ���� �������� �ʱ�ȭ
	void Init();
	void Init( int i );

	void Release();

	//////////////////////////////////////////////////////////////////////////
	void SendAsync();
	//////////////////////////////////////////////////////////////////////////

	int GetGameID() const { return m_id; }

	//======================================
	// ������ �����Լ�
	//======================================
	BOOL Run();
	//--------------------------------------
	// Run���� �ҷ��� ���� ���� �Լ�
	//--------------------------------------
	// return����	TRUE�� �������� ���� �����̰�
	//				FALSE�̸� player�� ��� �����ؼ� ������ ����� ����
	BOOL GameRun();
	//======================================

	//==============================================================
	//���ӽ���
	BOOL StartGame( int playercount );
	//���� �������� ó���ؾ� �ϴ� ����
	BOOL PreStartGame();
	//--------------------------------------------------------------
	//��������
	void EndGame();
	//���� ���ᰡ �ǰ� ó���ϴ� ����
	void EndLogic();
	//==============================================================

	//���� �÷��� �������� Ȯ��
	inline BOOL NowIsPlay() { return m_nowIsPlaying; }

	//�÷��̾� �߰�
	void AddPlayer( GameSession* player );
	//�÷��̾� ����/ �� ���� proc�� ����� ������ FALSE�� return
	BOOL DelPlayer( GameSession* player );
	//���ӳ��� ĳ���͸� �˻�
	CharObj* FindChar( int sessionID );

	//�濡 �ִ� ��� player���� ����
	//���� ���� �������� �ڽ��� session�� �Ű������� �ѱ��
	void SendAllPlayerInGame( SPacket& packet, GameSession* me = NULL );

	//�濡 �ִ� ��� player������ ��Ŷ�� ��´�
	//���� ���� �������� �ڽ��� session�� �Ű������� �ѱ��.
	void PackageAllPlayerInGame( SPacket& packet, GameSession* me = NULL  );
};

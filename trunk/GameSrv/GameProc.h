#pragma once

#include "SThread.h"
#include "STime.h"

class GameSession;
class SPacket;

class GameProc : public SThread
{
private:
	SServerObj				*m_critical;
private:
	// ������ �ϰ� �ִ� player list
	std::list<GameSession*>	m_listPlayer;

	// ������ ���� list
	//

	// ������ play�ϴ� �ο�
	int						m_playerCount;

	// ������ ���� �� �ο�
	int						m_inGamePlayerCount;

	// �� �� ų��
	int						m_AttKillCount;
	int						m_DefKillCount;

	// ������ flag
	BOOL					m_nowIsPlaying;

	// �ð�
	//

	// �ð�Count
	//

	// ������ �̺�Ʈ �ڵ�
	HANDLE					m_hStartGame;

public:
	GameProc(void);
	~GameProc(void);

	// ���� �������� �ʱ�ȭ
	void Init();
	void Release();

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
	void DelPlayer( GameSession* player );

	//�濡 �ִ� ��� player���� ����
	//���� ���� �������� �ڽ��� session�� �Ű������� �ѱ��
	void SendAllPlayerInGame( SPacket& packet, GameSession* me = NULL );

	//�濡 �ִ� ��� player������ ��Ŷ�� ��´�
	//���� ���� �������� �ڽ��� session�� �Ű������� �ѱ��.
	void PackageAllPlayerInGame( SPacket& packet, GameSession* me = NULL  );
};

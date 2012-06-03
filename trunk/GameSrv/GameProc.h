#pragma once

#include "SThread.h"
#include "SList.h"
#include "STime.h"

class GameSession;
class ItemObj;
class SPacket;
class CharObj;
class SLogger;

enum Team
{
	GAME_TEAM_ATT = 0,
	GAME_TEAM_DEF,
};

class GameProc : public SThread
{
private:
	//======================================
	// ������
	//======================================
	// �ڽ��� ID(���ȣ(key��))
	int						m_id;
	// �� ���� PORT��ȣ��
	int						m_port;
	//======================================

	// playerlist
	SList<GameSession*>		m_listPlayer;
	// ������ ���� list
	SList<ItemObj*>			m_listItem;

	//======================================
	// ������ flag
	BOOL					m_nowIsPlaying;

	//======================================
	// ������ play�ϴ� �ο�(�κ񿡼� �޾ƿ� �ο�)
	int						m_playerCount;
	// ������ ������ �غ� �Ǿ� �ִ� �ο���
	int						m_readyCount;

	//==============================================================
	//--------------------------------------
	// �̹� ��
	//--------------------------------------
	// �� �� ų��
	int						m_AttKillCount;
	int						m_DefKillCount;
	//--------------------------------------

	//--------------------------------------
	// ��ü�Ǽ�
	//--------------------------------------
	// �� �� ų��
	int						m_AttKillAllCount;
	int						m_DefKillAllCount;
	// ���ھ�
	int						m_AttWinCount;
	int						m_DefWinCount;
	int						m_TieCount;
	//==============================================================

	//======================================
	// �� ����
	int						m_gameStageMap;
	// ���� ���
	int						m_gameMode;
	//======================================
	//======================================
	// �ð�
	STime					m_timer;
	// �ð�Count
	int						m_playTime;				//���Ǵ� playTime
	int						m_nowPlayTimeCount;		//��������� time
	int						m_playCount;			//�� �Ǽ�

	//======================================
	// ������ �̺�Ʈ �ڵ�
	HANDLE					m_hStartGame;

	//======================================
	// ���� flag
	//======================================
	//play�ο��� ��� �غ� �� �Ǿ���
	HANDLE					m_hStartEvent;

	//======================================

	//======================================
	// SingleTon ��ü��
	//======================================
	SLogger*				m_logger;

	//======================================

public:
	GameProc(void);
	~GameProc(void);

	// ���� �������� �ʱ�ȭ
	void Init( int i );
	void Init();

	int GetGameID() const { return m_id; }
	int GetPort() const { return m_port; }

	//==============================================================
	// ����proc�������� �Լ�
	//==============================================================
	//m_playerCount
	//���� �ο�
	inline void SetPlayerCount( int count ) { m_playerCount = count; }
	inline int	GetPlayerCount() { return m_playerCount; }
	//���� ��
	inline void SetGameStage( int stage ) { m_gameStageMap = stage; }
	inline int	GetGameStage() { return m_gameStageMap; }
	//���� ���
	inline void SetGameMode( int mode ) { m_gameMode = mode; }
	inline int	GetGameMode() { return m_gameMode; }
	//���� �Ǵ� play�ð�
	inline void SetGamePlayTime( int timeCount ) { m_playTime = timeCount; }
	inline int	GetGamePlayTime() { return m_playTime; }
	//���� �Ǽ�
	inline void SetGamePlayCount( int count ) { m_playCount = count; }
	inline int	GetGamePlayCount() { return m_playCount; }
	//==============================================================

	//==============================================================
	//���� ���� �غ� �Ϸ�� ��ȣ�� ������ �ϳ��� �÷� �ش�.
	void AddReadyCount();
	//==============================================================

	//======================================
	// ������ �����Լ�
	//======================================
	BOOL Run();
	//--------------------------------------
	// Run���� �ҷ��� ���� ���� �Լ�
	//--------------------------------------
	// return����	TRUE�� �������� ���� �����̰�
	//				FALSE�̸� player�� ��� �����ؼ� ������ ����� ����
	void GameRun();
	//==============================================================
	//���ӽ���_���� ����
	BOOL StartGame();
	//--------------------------------------------------------------
	//���� �������� ó���ؾ� �ϴ� ����
	//BOOL PreStartGame();
	//���� ���ᰡ �ǰ� ó���ϴ� ����
	//��� ������ ����Ǿ����� return FALSE
	//�����ؾ� �ϴ� ������ �������� return TRUE
	BOOL ResetGame();
	//���� ������ ����
	void EndGame();
	//==============================================================

	//���� �÷��� �������� Ȯ��
	inline BOOL NowIsPlay() { return m_nowIsPlaying; }

	void AddKillCount( BOOL deathTeam );

	//���� ������ �ִ� ���ο����� �޴´�
	int GetConnectPlayerCount() { return m_listPlayer.GetItemCount(); }

	//�÷��̾� �߰�
	void AddPlayer( GameSession* player );
	//�÷��̾� ����/ �� ���� proc�� ����� ������ FALSE�� return
	BOOL DelPlayer( GameSession* player );

	void ClearItem();

	//���ӳ��� ĳ���͸� �˻�
	CharObj* FindChar( int sessionID );

	//SC_GAME_START_GAME
	BOOL SendStartPacket();

	//�濡 �ִ� ��� player���� ����
	//���� ���� �������� �ڽ��� session�� �Ű������� �ѱ��
	void SendAllPlayerInGame( SPacket& packet, GameSession* me = NULL );

	//�濡 �ִ� ��� player������ ��Ŷ�� ��´�
	//���� ���� �������� �ڽ��� session�� �Ű������� �ѱ��.
	void PackageAllPlayerInGame( SPacket& packet, GameSession* me = NULL  );
};

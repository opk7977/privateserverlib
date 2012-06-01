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
	int						m_readyCount;

	//==============================================================
	//--------------------------------------
	// �̹� ��
	//--------------------------------------
	// �� �� ų��
	int						m_AttKillCount;
	int						m_DefKillCount;
	// �¸� ��
	int						m_WinTeam;
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
	//==============================================================

	//���� ĳ������ ��
	//int						m_deathPlayerCount;

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
	//BOOL					m_isAllReady;
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

	//���� ��(?)�� ���� ������ �����ϰ� �ٽ� ����
	void ResultGame();

	void Release();

	//////////////////////////////////////////////////////////////////////////
	void SendAsync();
	//////////////////////////////////////////////////////////////////////////

	int GetGameID() const { return m_id; }
	int GetPort() const { return m_port; }

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

	//==============================================================
	//���ӽ���
	BOOL StartGame();
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

	void AddKillCount( BOOL deathTeam );

	//�÷��̾� �߰�
	void AddPlayer( GameSession* player );
	//�÷��̾� ����/ �� ���� proc�� ����� ������ FALSE�� return
	BOOL DelPlayer( GameSession* player );
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

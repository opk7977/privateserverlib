#pragma once

#include "SThread.h"
#include "SList.h"
#include "STime.h"

class GameSession;
class ItemObj;
class SPacket;
class CharObj;
class SLogger;
class DataLeader;
class DBSrvMgr;

#define WAIT_GAME_END_TIME	10

enum Team
{
	GAME_TEAM_ATT = 0,
	GAME_TEAM_DEF,
};

enum GameMode
{
	GAME_MODE_DEATH_MATCH = 0,
	GAME_MODE_BOOM_MISSION,
	GAME_MODE_HACKING_MISSION,

	GAME_MODE_COUNT,
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

	// �� ü�� ��� ������ clear�ǰ��ϴ� ������
	SList<GameSession*>		m_SendList;

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
	// ������ġ�� �ƴѰ�� ������ ���������� ���
	// -1:Ÿ�Ӿƿ�/ 0:0����/ 1:1����
	int						m_isWin;
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
	// ���� ����!
	HANDLE					m_hStartGame;
	//======================================
	// ���� flag
	//======================================
	//play�ο��� ��� �غ� �� �Ǿ���
	HANDLE					m_hStartEvent;
	//�κ񼭹��� ���� ������ ������ ��� �Ϸ� �Ǿ��ٴ� ��ȣ�� ����
	HANDLE					m_hReturnResult;
	//======================================

	//======================================
	// ���� ��忡 ����
	// ��� ���� �Լ��� ��� Func�迭
	//======================================
	void					(GameProc::*m_GameResult[GAME_MODE_COUNT])(int);
	//======================================

	//======================================
	// SingleTon ��ü��
	//======================================
	static SLogger*			m_logger;
	static DataLeader*		m_document;
	static DBSrvMgr*		m_dbMgr;
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
	//���� ���ᰡ �ǰ� ó���ϴ� ����
	//��� ������ ����Ǿ����� return FALSE
	//�����ؾ� �ϴ� ������ �������� return TRUE
	BOOL ResetGame();
	//���� ������ ����
	void EndGame();
	//������ ����(?)������ �ٽý��� Ȥ�� ���� ����
	void WaitTimeLogic( int waitTime = WAIT_GAME_END_TIME );
	//==============================================================

	//==============================================================
	// ���� ���� ���� �Լ���
	//--------------------------------------------------------------
	// GAME_MODE_DEATH_MATCH
	void GameEnd_DeathMatch( int winnerTeam );
	// GAME_MODE_BOOM_MISSION
	void GameEnd_BoomMission( int winnerTeam );
	// GAME_MODE_HACKING_MISSION
	void GameEnd_HackingMission( int winnerTeam );

	// ����� ������ DB������ ����
	BOOL SendToDBGameResult();
	// �κ񿡼� ��ȣ�� ����
	inline void RecvLobbyEndReadyOK() { SetEvent( m_hReturnResult ); }
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

	void CharacterRestart();
	void ClearItem();

	//���ӳ��� ĳ���͸� �˻�
	CharObj* FindChar( int sessionID );

	//��� ĳ������ �Ǹ� �÷� �ְ� �ǰ� �ö� �ֵ��� list�� �־� �ش�
	void PlayerHeal();
	//�ǰ� �ö� �ֵ鸸 ������.
	void SendPlayerHeal();

	//SC_GAME_START_GAME
	BOOL SendStartPacket();
	//SC_GAME_RESTART
	BOOL SendRestartPacket();
	//SC_GAME_GOTO_LOBBY
	BOOL SendGotoLobbyPacket();

	//�濡 �ִ� ��� player���� ����
	//���� ���� �������� �ڽ��� session�� �Ű������� �ѱ��
	void SendAllPlayerInGame( SPacket& packet, GameSession* me = NULL );
	//�濡 �ִ� �ڽ��� �� ����鿡�� ��Ŷ�� ����
	void SendPacketToMyTeam( int team, SPacket& packet, GameSession* me = NULL );

	//�濡 �ִ� ��� player������ ��Ŷ�� ��´�
	//���� ���� �������� �ڽ��� session�� �Ű������� �ѱ��.
	void PackageAllPlayerInGame( SPacket& packet, GameSession* me = NULL  );
};

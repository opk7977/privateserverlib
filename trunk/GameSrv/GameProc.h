#pragma once

#include "SThread.h"
#include "SList.h"
#include "STime.h"

class GameSession;
class MineItem;
class SPacket;
class CharObj;
class SLogger;
class DataLeader;
class DBSrvMgr;
class ItemMgr;
class MineItem;
class CharMgr;


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

enum SendTime
{
	SENDTIME_TEN	= 10,
	SENDTIME_FIVE	= 5,
};

enum LastTime
{
	LASTTIME_THREE	= 3,
};

class GameProc : public SThread
{
private:
	//======================================
	// ������
	//======================================
	// �ڽ��� ID(���ȣ(key��))
	int								m_id;
	// �� ���� PORT��ȣ��
	int								m_port;
	//======================================

	// playerlist
	SList<GameSession*>				m_listPlayer;
	// ������ ���� list
// 	SList<ItemObj*>					m_listItem;

	//======================================
	// ����ȭ�� ���� ��ü
	SServerObj*						m_mineCritical;
	// ����
	ATL::CAtlMap<int, MineItem*>	m_mapMine;
	// ��ġ�� ����
	//SList<MineItem*>				m_boomSoon;
	//======================================

	// �� ü�� ��� ������ clear�ǰ��ϴ� ������
	//SList<GameSession*>				m_SendList;

	//======================================
	// ������ flag
	BOOL							m_nowIsPlaying;

	//======================================
	// ������ play�ϴ� �ο�(�κ񿡼� �޾ƿ� �ο�)
	int								m_playerCount;
	// ������ ������ �غ� �Ǿ� �ִ� �ο���
	int								m_readyCount;

	//==============================================================
	//--------------------------------------
	// �̹� ��
	//--------------------------------------
	// �� �� ų��
	int								m_AttKillCount;
	int								m_DefKillCount;
	// ������ġ�� �ƴѰ�� ������ ���������� ���
	// -1:Ÿ�Ӿƿ�/ 0:0����/ 1:1����
	int								m_isWin;
	//--------------------------------------

	//--------------------------------------
	// ��ü�Ǽ�
	//--------------------------------------
	// �� �� ų��
	int								m_AttKillAllCount;
	int								m_DefKillAllCount;
	// ���ھ�
	int								m_AttWinCount;
	int								m_DefWinCount;
	int								m_TieCount;
	//==============================================================

	//======================================
	// �� ����
	int								m_gameStageMap;
	// ���� ���
	int								m_gameMode;
	//======================================
	//======================================
	// �ð�
	STime							m_timer;
	// �ð�Count
	int								m_playTime;				//���Ǵ� playTime
	int								m_nowPlayTimeCount;		//��������� time
	int								m_playCount;			//�� �Ǽ�

	//======================================
	// ������ �̺�Ʈ �ڵ�
	// ���� ����!
	HANDLE							m_hStartGame;
	//======================================
	// ���� flag
	//======================================
	//play�ο��� ��� �غ� �� �Ǿ���
	HANDLE							m_hStartEvent;
	//�κ񼭹��� ���� ������ ������ ��� �Ϸ� �Ǿ��ٴ� ��ȣ�� ����
	HANDLE							m_hReturnResult;
	//======================================

	//======================================
	// ���� ��忡 ����
	// ��� ���� �Լ��� ��� Func�迭
	//======================================
	void							(GameProc::*m_GameResult[GAME_MODE_COUNT])(int);
	//======================================

	//======================================
	// SingleTon ��ü��
	//======================================
	static SLogger*					m_logger;
	static DataLeader*				m_document;
	static DBSrvMgr*				m_dbMgr;
	static ItemMgr*					m_itemMgr;
	static CharMgr*					m_charMgr;
	//======================================


	//test��
	//�Ѱ��� �ð��� �帣�� �ȵ�
	BOOL							m_isSelectCountDown;
	//���� �ð��� �帣�� �ȵ�
	BOOL							m_isGameCountDown;

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
	//�׳� �ð��� ��ٸ��� �Լ� ������ �ð���ŭ ��ٸ�
	void WaitTimeLogic( float waitTime );
	//�ð��� ���� SendTime�������� ��Ŷ�� �����µ� LastCount�������� �ϳ��� ������.
	//SendTime�� 10�� 5/ LastCount�� 3������ �����ϴ�
	//void CountDownLogin( int waitTime, SendTime sendTime, LastTime lastTime );
	void CountDownLogic( int waitTime );
	//==============================================================

	//==============================================================
	// ���� �ʱ�ȭ
	//--------------------------------------------------------------
	void MineClear();
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

	//======================================
	// �ʱ�ȭ ����
	//======================================
	void CharacterRestart();
	//���� ������ ���� ���ڸ� �ٽ� ����� �� �ֵ��� ����� �ش�
	//����ĳ���� ����
	BOOL MineResetTarget( int sessionID );
	//��ü ����
	void MineReset();
	//======================================

	//���ӳ��� ĳ���͸� �˻�
	CharObj* FindChar( int sessionID );
	//���ڰ˻�
	MineItem* FindMine( int sessionID );

	//�Ǹ� �ø��� �ö� �ֵ鿡�� ���� hp�� �����ش�
	void CountUpCharHP( float elaps );
	//SC_GAME_REMAIN_HP
	BOOL SendHPCountUp( GameSession* session );

	//======================================
	// ����/ ��ĵ
	//======================================
	//����/ ��ĵ ��ġ ����
	void CountSkillPoints( float elaps );	

	//���Ű� ��ĵ ��ġ ����
	BOOL SendSkillPoint( GameSession* session );
	//��ųǮ�� �ֵ� Ȯ���ϰ� ����
	BOOL SendHideOff( GameSession* session );
	BOOL SendScanOff( GameSession* session );

	//======================================
	// ���� ����
	//======================================
	//���� ��ġ
	//BOOL SettingMine( int sessionId, float posX, float posY, float posZ, float dirX, float dirY, float dirZ );
	BOOL SettingMine( GameSession* session, float posX, float posY, float posZ, float dirX, float dirY, float dirZ );
	//--------------------------------------
	//���� ������ �ֵ� �ð� ���̱�
	void CountDownRunningMine();
	//�����ϴ� ���� �浹 üũ
	void ExplosionMineCrashCheck();
	//��ġ�� ���� ĳ���� �浹 üũ
	void MineCrashCheck();
	//======================================

	//======================================
	// ĳ���� ����
	//======================================
	void CountDownCharInvincible();
	//======================================
	// ���� Ǯ������ ��ο��� �˸���
	//======================================
	void SendEndInvincible( CharObj* uChar );
	//======================================

	//SC_GAME_GAME_READY
	BOOL SendReadyPacket();
	//SC_GAME_START_GAME
	BOOL SendStartPacket();
	//SC_GAME_TIME_REMAIN
	BOOL SendTimeRemain( int remainTime );
	//SC_GAME_NOTICE
	BOOL SendNotice( TCHAR* notice );
	//SC_GAME_RESTART
	BOOL SendRestartPacket();
	//ĳ���� ���¸� ���ᰡ �ƴ϶� �κ�� ���ư��� ���·� �ٲ� ��
	void SetGotoLobby();
	//SC_GAME_GOTO_LOBBY
	BOOL SendGotoLobbyPacket();

	//SC_GAME_RUN_MINE
	BOOL SendGameRunMine( MineItem* mine );
	//SC_GAME_EXPLOSION_MINE
	BOOL SendGameExplosionMine( MineItem* mine );
	//SC_GAME_CHARACTER_DAMEGED_BY_MINE
	//SC_GAME_YOU_DAMEGED_BY_MINE
	BOOL SendGameCharDamegedByMine( int mineSessionID, CharObj* damegedChar );
	//SC_GAME_CHARACTER_DIE_BY_MINE
	//SC_GAME_YOU_DIE_BY_MINE
	BOOL SendGameCharDieByMine( int masterID, CharObj* dieChar );

	//�濡 �ִ� ��� player���� ����
	//���� ���� �������� �ڽ��� session�� �Ű������� �ѱ��
	void SendAllPlayerInGame( SPacket& packet, GameSession* me = NULL );
	//�濡 �ִ� �ڽ��� �� ����鿡�� ��Ŷ�� ����
	void SendPacketToMyTeam( int team, SPacket& packet, GameSession* me = NULL );

	//�濡 �ִ� ��� player������ ��Ŷ�� ��´�
	//���� ���� �������� �ڽ��� session�� �Ű������� �ѱ��.
	void PackageAllPlayerInGame( SPacket& packet, GameSession* me = NULL  );


	//test
	void SetIsSelectGun() { m_isSelectCountDown = TRUE; }
	void SetIsStartGame() { m_isGameCountDown = TRUE; }
};

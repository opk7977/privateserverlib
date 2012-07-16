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
	// 지정값
	//======================================
	// 자신의 ID(방번호(key값))
	int								m_id;
	// 이 방의 PORT번호값
	int								m_port;
	//======================================

	// playerlist
	SList<GameSession*>				m_listPlayer;
	// 아이템 정보 list
// 	SList<ItemObj*>					m_listItem;

	//======================================
	// 동기화를 위한 객체
	SServerObj*						m_mineCritical;
	// 지뢰
	ATL::CAtlMap<int, MineItem*>	m_mapMine;
	// 설치된 지뢰
	//SList<MineItem*>				m_boomSoon;
	//======================================

	// 피 체운 대상 보내고 clear되고하는 패턴임
	//SList<GameSession*>				m_SendList;

	//======================================
	// 게임중 flag
	BOOL							m_nowIsPlaying;

	//======================================
	// 게임을 play하는 인원(로비에서 받아온 인원)
	int								m_playerCount;
	// 게임을 시작할 준비가 되어 있는 인원수
	int								m_readyCount;

	//==============================================================
	//--------------------------------------
	// 이번 판
	//--------------------------------------
	// 각 팀 킬수
	int								m_AttKillCount;
	int								m_DefKillCount;
	// 데스메치가 아닌경우 게임이 끝났을때의 결과
	// -1:타임아웃/ 0:0팀승/ 1:1팀승
	int								m_isWin;
	//--------------------------------------

	//--------------------------------------
	// 전체판수
	//--------------------------------------
	// 각 팀 킬수
	int								m_AttKillAllCount;
	int								m_DefKillAllCount;
	// 스코어
	int								m_AttWinCount;
	int								m_DefWinCount;
	int								m_TieCount;
	//==============================================================

	//======================================
	// 맵 정보
	int								m_gameStageMap;
	// 게임 모드
	int								m_gameMode;
	//======================================
	//======================================
	// 시간
	STime							m_timer;
	// 시간Count
	int								m_playTime;				//한판당 playTime
	int								m_nowPlayTimeCount;		//현재게임의 time
	int								m_playCount;			//총 판수

	//======================================
	// 쓰레드 이벤트 핸들
	// 게임 오픈!
	HANDLE							m_hStartGame;
	//======================================
	// 게임 flag
	//======================================
	//play인원이 모두 준비가 다 되었음
	HANDLE							m_hStartEvent;
	//로비서버로 부터 데이터 정산이 모두 완료 되었다는 신호를 받음
	HANDLE							m_hReturnResult;
	//======================================

	//======================================
	// 게임 모드에 따른
	// 결과 정산 함수를 담는 Func배열
	//======================================
	void							(GameProc::*m_GameResult[GAME_MODE_COUNT])(int);
	//======================================

	//======================================
	// SingleTon 객체들
	//======================================
	static SLogger*					m_logger;
	static DataLeader*				m_document;
	static DBSrvMgr*				m_dbMgr;
	static ItemMgr*					m_itemMgr;
	static CharMgr*					m_charMgr;
	//======================================


	//test용
	//총고르는 시간이 흐르면 안됨
	BOOL							m_isSelectCountDown;
	//게임 시간이 흐르면 안됨
	BOOL							m_isGameCountDown;

public:
	GameProc(void);
	~GameProc(void);

	// 게임 정보들을 초기화
	void Init( int i );
	void Init();

	int GetGameID() const { return m_id; }
	int GetPort() const { return m_port; }

	//==============================================================
	// 게임proc변수관련 함수
	//==============================================================
	//m_playerCount
	//게임 인원
	inline void SetPlayerCount( int count ) { m_playerCount = count; }
	inline int	GetPlayerCount() { return m_playerCount; }
	//게임 맵
	inline void SetGameStage( int stage ) { m_gameStageMap = stage; }
	inline int	GetGameStage() { return m_gameStageMap; }
	//게임 모드
	inline void SetGameMode( int mode ) { m_gameMode = mode; }
	inline int	GetGameMode() { return m_gameMode; }
	//게임 판당 play시간
	inline void SetGamePlayTime( int timeCount ) { m_playTime = timeCount; }
	inline int	GetGamePlayTime() { return m_playTime; }
	//게임 판수
	inline void SetGamePlayCount( int count ) { m_playCount = count; }
	inline int	GetGamePlayCount() { return m_playCount; }
	//==============================================================

	//==============================================================
	//게임 시작 준비가 완료된 신호를 받으면 하나씩 올려 준다.
	void AddReadyCount();
	//==============================================================

	//======================================
	// 쓰레드 동작함수
	//======================================
	BOOL Run();
	//--------------------------------------
	// Run에서 불려질 게임 실행 함수
	//--------------------------------------
	// return값이	TRUE면 정상적인 게임 종료이고
	//				FALSE이면 player가 모두 종료해서 게임이 종료된 경우다
	void GameRun();
	//==============================================================
	//게임시작_변수 셋팅
	BOOL StartGame();
	//--------------------------------------------------------------
	//게임 종료가 되고 처리하는 로직
	//모든 게임이 종료되었으면 return FALSE
	//진행해야 하는 게임이 남았으면 return TRUE
	BOOL ResetGame();
	//게임 완전히 종료
	void EndGame();
	//그냥 시간을 기다리는 함수 지정된 시간만큼 기다림
	void WaitTimeLogic( float waitTime );
	//시간을 세서 SendTime단위마다 패킷을 보내는데 LastCount설정부터 하나씩 보낸다.
	//SendTime은 10과 5/ LastCount는 3정도가 적당하다
	//void CountDownLogin( int waitTime, SendTime sendTime, LastTime lastTime );
	void CountDownLogic( int waitTime );
	//==============================================================

	//==============================================================
	// 지뢰 초기화
	//--------------------------------------------------------------
	void MineClear();
	//==============================================================

	//==============================================================
	// 게임 종료 정산 함수들
	//--------------------------------------------------------------
	// GAME_MODE_DEATH_MATCH
	void GameEnd_DeathMatch( int winnerTeam );
	// GAME_MODE_BOOM_MISSION
	void GameEnd_BoomMission( int winnerTeam );
	// GAME_MODE_HACKING_MISSION
	void GameEnd_HackingMission( int winnerTeam );

	// 정산된 내용을 DB서버로 전송
	BOOL SendToDBGameResult();
	// 로비에서 신호를 받음
	inline void RecvLobbyEndReadyOK() { SetEvent( m_hReturnResult ); }
	//==============================================================

	//지금 플레이 중인지를 확인
	inline BOOL NowIsPlay() { return m_nowIsPlaying; }

	void AddKillCount( BOOL deathTeam );

	//지금 접속해 있는 총인원수를 받는다
	int GetConnectPlayerCount() { return m_listPlayer.GetItemCount(); }

	//플레이어 추가
	void AddPlayer( GameSession* player );
	//플레이어 제거/ 현 게임 proc에 사람이 없으면 FALSE를 return
	BOOL DelPlayer( GameSession* player );

	//======================================
	// 초기화 로직
	//======================================
	void CharacterRestart();
	//다음 게임을 위해 지뢰를 다시 사용할 수 있도록 만들어 준다
	//지정캐릭터 지뢰
	BOOL MineResetTarget( int sessionID );
	//전체 지뢰
	void MineReset();
	//======================================

	//게임내의 캐릭터를 검색
	CharObj* FindChar( int sessionID );
	//지뢰검색
	MineItem* FindMine( int sessionID );

	//피를 올리고 올라간 애들에게 남은 hp를 보내준다
	void CountUpCharHP( float elaps );
	//SC_GAME_REMAIN_HP
	BOOL SendHPCountUp( GameSession* session );

	//======================================
	// 은신/ 스캔
	//======================================
	//은신/ 스캔 수치 증감
	void CountSkillPoints( float elaps );	

	//은신과 스캔 수치 전송
	BOOL SendSkillPoint( GameSession* session );
	//스킬풀린 애들 확인하고 전송
	BOOL SendHideOff( GameSession* session );
	BOOL SendScanOff( GameSession* session );

	//======================================
	// 지뢰 로직
	//======================================
	//지뢰 설치
	//BOOL SettingMine( int sessionId, float posX, float posY, float posZ, float dirX, float dirY, float dirZ );
	BOOL SettingMine( GameSession* session, float posX, float posY, float posZ, float dirX, float dirY, float dirZ );
	//--------------------------------------
	//터질 예정인 애들 시간 줄이기
	void CountDownRunningMine();
	//폭발하는 지뢰 충돌 체크
	void ExplosionMineCrashCheck();
	//설치된 지뢰 캐릭터 충돌 체크
	void MineCrashCheck();
	//======================================

	//======================================
	// 캐릭터 무적
	//======================================
	void CountDownCharInvincible();
	//======================================
	// 무적 풀렸으면 모두에게 알린다
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
	//캐릭터 상태를 종료가 아니라 로비로 돌아가는 상태로 바꿔 줌
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

	//방에 있는 모든 player에게 전송
	//나를 빼고 보내려면 자신의 session을 매개변수로 넘긴다
	void SendAllPlayerInGame( SPacket& packet, GameSession* me = NULL );
	//방에 있는 자신의 팀 사람들에게 패킷을 전송
	void SendPacketToMyTeam( int team, SPacket& packet, GameSession* me = NULL );

	//방에 있는 모든 player정보를 패킷에 담는다
	//나를 빼고 담으려면 자신의 session을 매개변수로 넘긴다.
	void PackageAllPlayerInGame( SPacket& packet, GameSession* me = NULL  );


	//test
	void SetIsSelectGun() { m_isSelectCountDown = TRUE; }
	void SetIsStartGame() { m_isGameCountDown = TRUE; }
};

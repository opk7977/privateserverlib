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
	// 자신의 ID(방번호(key값))
	int						m_id;
	// playerlist
	SList<GameSession*>		m_listPlayer;
	// 아이템 정보 list
	SList<ItemObj*>			m_listItem;

	//======================================
	// 게임중 flag
	BOOL					m_nowIsPlaying;

	//======================================
	// 게임을 play하는 인원(로비에서 받아온 인원)
	int						m_playerCount;

	//======================================
	// 각 팀 킬수
	int						m_AttKillCount;
	int						m_DefKillCount;
	//--------------------------------------
	// 스코어
	int						m_AttWinCount;
	int						m_DefWinCount;
	//======================================
	//======================================
	// 맵 정보
	int						m_gameStageMap;
	// 게임 모드
	int						m_gameMode;
	//======================================
	//======================================
	// 시간
	STime					m_timer;
	// 시간Count
	int						m_playTime;				//한판당 playTime
	int						m_nowPlayTimeCount;		//현재게임의 time
	int						m_playCount;			//총 판수

	//======================================
	// 쓰레드 이벤트 핸들
	HANDLE					m_hStartGame;

public:
	GameProc(void);
	~GameProc(void);

	// 게임 정보들을 초기화
	void Init();
	void Init( int i );

	void Release();

	//////////////////////////////////////////////////////////////////////////
	void SendAsync();
	//////////////////////////////////////////////////////////////////////////

	int GetGameID() const { return m_id; }

	//======================================
	// 쓰레드 동작함수
	//======================================
	BOOL Run();
	//--------------------------------------
	// Run에서 불려질 게임 실행 함수
	//--------------------------------------
	// return값이	TRUE면 정상적인 게임 종료이고
	//				FALSE이면 player가 모두 종료해서 게임이 종료된 경우다
	BOOL GameRun();
	//======================================
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
	//게임시작
	BOOL StartGame();
	//게임 시작전에 처리해야 하는 로직
	BOOL PreStartGame();
	//--------------------------------------------------------------
	//게임종료
	void EndGame();
	//게임 종료가 되고 처리하는 로직
	void EndLogic();
	//==============================================================

	//지금 플레이 중인지를 확인
	inline BOOL NowIsPlay() { return m_nowIsPlaying; }

	//플레이어 추가
	void AddPlayer( GameSession* player );
	//플레이어 제거/ 현 게임 proc에 사람이 없으면 FALSE를 return
	BOOL DelPlayer( GameSession* player );
	//게임내의 캐릭터를 검색
	CharObj* FindChar( int sessionID );

	//방에 있는 모든 player에게 전송
	//나를 빼고 보내려면 자신의 session을 매개변수로 넘긴다
	void SendAllPlayerInGame( SPacket& packet, GameSession* me = NULL );

	//방에 있는 모든 player정보를 패킷에 담는다
	//나를 빼고 담으려면 자신의 session을 매개변수로 넘긴다.
	void PackageAllPlayerInGame( SPacket& packet, GameSession* me = NULL  );
};

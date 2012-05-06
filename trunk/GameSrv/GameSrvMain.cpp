#include "GameSrvMain.h"

#include "DataLeader.h"

#include "Network.h"
#include "SrvNet.h"

#include "GameMgr.h"

//test////////////////////////////////////////////////////////////////////
#include "GameProc.h"
#include "CharMgr.h"
//////////////////////////////////////////////////////////////////////////

GameSrvMain::GameSrvMain(void)
{
}

GameSrvMain::~GameSrvMain(void)
{
	Release();
}

BOOL GameSrvMain::Init()
{
	//서버 초기화
	if( !GetNetwork.Init( GetData.SessionCount ) )
		return FALSE;

	//서버 셋팅
	if( !GetNetwork.SrvSetting( GetData.GameSrvProtNum ) )
		return FALSE;

	//로비서버와 접속
// 	if( !GetSrvNet.ConnectToSrv( GetData.LobbySrvIP, GetData.LobbySrvPortNum ) )
// 		return FALSE;

	//캐릭터 공간 셋팅
	GetCharMgr.Init();

	//게임proc셋팅
	GetGameMgr.CreateGameProc( GetData.RoomCount );


	//test////////////////////////////////////////////////////////////////////
	//1번 방을 열어 놓는다////////////////////////////////////////////////////
	GameProc* tmpGame = GetGameMgr.FindGame( 1 );
	tmpGame->StartGame( 8 );
	//첫번째 캐릭터 수동 입력/////////////////////////////////////////////////
	CharObj* tmpChar = GetCharMgr.GetCharSpace();
	tmpChar->SetIndexId( 1 );
	tmpChar->SetID( _T("admin") );
	tmpChar->SetTeam( SRV_CHAR_TEAM_ATT );
	tmpChar->SetState( SRV_CHAR_STATE_STAND );
	tmpChar->SetPosition( 10.f, 0.f, 10.f );
	tmpChar->SetDirection( 0.f, 0.f, 1.f );
	//두번째 캐릭터 수동 입력/////////////////////////////////////////////////
	tmpChar = GetCharMgr.GetCharSpace();
	tmpChar->SetIndexId( 2 );
	tmpChar->SetID( _T("seg") );
	tmpChar->SetTeam( SRV_CHAR_TEAM_DEF );
	tmpChar->SetState( SRV_CHAR_STATE_STAND );
	tmpChar->SetPosition( 20.f, 0.f, 20.f );
	tmpChar->SetDirection( 0.f, 0.f, 1.f );
	//세번째 캐릭터 수동 입력/////////////////////////////////////////////////
	tmpChar = GetCharMgr.GetCharSpace();
	tmpChar->SetIndexId( 3 );
	tmpChar->SetID( _T("areah") );
	tmpChar->SetTeam( SRV_CHAR_TEAM_ATT );
	tmpChar->SetState( SRV_CHAR_STATE_STAND );
	tmpChar->SetPosition( 30.f, 0.f, 30.f );
	tmpChar->SetDirection( 0.f, 0.f, 1.f );
	//네번째 캐릭터 수동 입력/////////////////////////////////////////////////
	tmpChar = GetCharMgr.GetCharSpace();
	tmpChar->SetIndexId( 4 );
	tmpChar->SetID( _T("aaa") );
	tmpChar->SetTeam( SRV_CHAR_TEAM_DEF );
	tmpChar->SetState( SRV_CHAR_STATE_STAND );
	tmpChar->SetPosition( 40.f, 0.f, 40.f );
	tmpChar->SetDirection( 0.f, 0.f, 1.f );
	//다섯번째 캐릭터 수동 입력///////////////////////////////////////////////
	tmpChar = GetCharMgr.GetCharSpace();
	tmpChar->SetIndexId( 5 );
	tmpChar->SetID( _T("zzz") );
	tmpChar->SetTeam( SRV_CHAR_TEAM_ATT );
	tmpChar->SetState( SRV_CHAR_STATE_STAND );
	tmpChar->SetPosition( 50.f, 0.f, 50.f );
	tmpChar->SetDirection( 0.f, 0.f, 1.f );
	//여섯번째 캐릭터 수동 입력///////////////////////////////////////////////
	tmpChar = GetCharMgr.GetCharSpace();
	tmpChar->SetIndexId( 6 );
	tmpChar->SetID( _T("qqq") );
	tmpChar->SetTeam( SRV_CHAR_TEAM_DEF );
	tmpChar->SetState( SRV_CHAR_STATE_STAND );
	tmpChar->SetPosition( 60.f, 0.f, 60.f );
	tmpChar->SetDirection( 0.f, 0.f, 1.f );
	//일곱번째 캐릭터 수동 입력///////////////////////////////////////////////
	tmpChar = GetCharMgr.GetCharSpace();
	tmpChar->SetIndexId( 7 );
	tmpChar->SetID( _T("asdf") );
	tmpChar->SetTeam( SRV_CHAR_TEAM_DEF );
	tmpChar->SetState( SRV_CHAR_STATE_STAND );
	tmpChar->SetPosition( 70.f, 0.f, 70.f );
	tmpChar->SetDirection( 0.f, 0.f, 1.f );
	//////////////////////////////////////////////////////////////////////////
	
	return TRUE;
}

void GameSrvMain::Release()
{
}

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
	//���� �ʱ�ȭ
	if( !GetNetwork.Init( GetData.SessionCount ) )
		return FALSE;

	//���� ����
	if( !GetNetwork.SrvSetting( GetData.GameSrvProtNum ) )
		return FALSE;

	//�κ񼭹��� ����
// 	if( !GetSrvNet.ConnectToSrv( GetData.LobbySrvIP, GetData.LobbySrvPortNum ) )
// 		return FALSE;

	//ĳ���� ���� ����
	GetCharMgr.Init();

	//����proc����
	GetGameMgr.CreateGameProc( GetData.RoomCount );


	//test////////////////////////////////////////////////////////////////////
	//1�� ���� ���� ���´�////////////////////////////////////////////////////
	GameProc* tmpGame = GetGameMgr.FindGame( 1 );
	tmpGame->StartGame( 8 );
	//ù��° ĳ���� ���� �Է�/////////////////////////////////////////////////
	CharObj* tmpChar = GetCharMgr.GetCharSpace();
	tmpChar->SetIndexId( 1 );
	tmpChar->SetID( _T("admin") );
	tmpChar->SetTeam( SRV_CHAR_TEAM_ATT );
	tmpChar->SetState( SRV_CHAR_STATE_STAND );
	tmpChar->SetPosition( 10.f, 0.f, 10.f );
	tmpChar->SetDirection( 0.f, 0.f, 1.f );
	//�ι�° ĳ���� ���� �Է�/////////////////////////////////////////////////
	tmpChar = GetCharMgr.GetCharSpace();
	tmpChar->SetIndexId( 2 );
	tmpChar->SetID( _T("seg") );
	tmpChar->SetTeam( SRV_CHAR_TEAM_DEF );
	tmpChar->SetState( SRV_CHAR_STATE_STAND );
	tmpChar->SetPosition( 20.f, 0.f, 20.f );
	tmpChar->SetDirection( 0.f, 0.f, 1.f );
	//����° ĳ���� ���� �Է�/////////////////////////////////////////////////
	tmpChar = GetCharMgr.GetCharSpace();
	tmpChar->SetIndexId( 3 );
	tmpChar->SetID( _T("areah") );
	tmpChar->SetTeam( SRV_CHAR_TEAM_ATT );
	tmpChar->SetState( SRV_CHAR_STATE_STAND );
	tmpChar->SetPosition( 30.f, 0.f, 30.f );
	tmpChar->SetDirection( 0.f, 0.f, 1.f );
	//�׹�° ĳ���� ���� �Է�/////////////////////////////////////////////////
	tmpChar = GetCharMgr.GetCharSpace();
	tmpChar->SetIndexId( 4 );
	tmpChar->SetID( _T("aaa") );
	tmpChar->SetTeam( SRV_CHAR_TEAM_DEF );
	tmpChar->SetState( SRV_CHAR_STATE_STAND );
	tmpChar->SetPosition( 40.f, 0.f, 40.f );
	tmpChar->SetDirection( 0.f, 0.f, 1.f );
	//�ټ���° ĳ���� ���� �Է�///////////////////////////////////////////////
	tmpChar = GetCharMgr.GetCharSpace();
	tmpChar->SetIndexId( 5 );
	tmpChar->SetID( _T("zzz") );
	tmpChar->SetTeam( SRV_CHAR_TEAM_ATT );
	tmpChar->SetState( SRV_CHAR_STATE_STAND );
	tmpChar->SetPosition( 50.f, 0.f, 50.f );
	tmpChar->SetDirection( 0.f, 0.f, 1.f );
	//������° ĳ���� ���� �Է�///////////////////////////////////////////////
	tmpChar = GetCharMgr.GetCharSpace();
	tmpChar->SetIndexId( 6 );
	tmpChar->SetID( _T("qqq") );
	tmpChar->SetTeam( SRV_CHAR_TEAM_DEF );
	tmpChar->SetState( SRV_CHAR_STATE_STAND );
	tmpChar->SetPosition( 60.f, 0.f, 60.f );
	tmpChar->SetDirection( 0.f, 0.f, 1.f );
	//�ϰ���° ĳ���� ���� �Է�///////////////////////////////////////////////
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

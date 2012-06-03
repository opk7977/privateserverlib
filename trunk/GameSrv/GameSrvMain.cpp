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
	//ĳ���� ���� ����
	GetCharMgr.Init();

	//ITEM ���� ����

	//����proc����
	GetGameMgr.CreateGameProc( GetDocument.RoomCount );

	//���� �ʱ�ȭ
	if( !GetNetwork.Init( GetDocument.SessionCount ) )
		return FALSE;

	//���� ����
	if( !GetNetwork.SrvSetting( GetDocument.GameSrvProtNum ) )
		return FALSE;

	//�κ񼭹��� ����
	if( !GetSrvNet.ConnectToSrv( GetDocument.LobbySrvIP, GetDocument.LobbySrvPortNum ) )
		return FALSE;

	//test////////////////////////////////////////////////////////////////////
	//1�� ���� ���� ���´�////////////////////////////////////////////////////
// 	GameProc* tmpGame = GetGameMgr.FindGame( 1 );
// 	tmpGame->StartGame( 8 );
// 	//ù��° ĳ���� ���� �Է�/////////////////////////////////////////////////
// 	CharObj* tmpChar = GetCharMgr.GetCharSpace();
// 	tmpChar->SetSessionID( 1 );
// 	tmpChar->SetID( _T("admin") );
// 	tmpChar->SetTeam( SRV_CHAR_TEAM_ATT );
// 	tmpChar->SetState( SRV_CHAR_STATE_STAND );
// 	tmpChar->SetPosition( 10.f, 0.f, 10.f );
// 	tmpChar->SetDirection( 0.f, 0.f, 1.f );
// 	tmpChar->SetDirInt( 0 );
// 	//�ι�° ĳ���� ���� �Է�/////////////////////////////////////////////////
// 	tmpChar = GetCharMgr.GetCharSpace();
// 	tmpChar->SetSessionID( 2 );
// 	tmpChar->SetID( _T("seg") );
// 	tmpChar->SetTeam( SRV_CHAR_TEAM_DEF );
// 	tmpChar->SetState( SRV_CHAR_STATE_STAND );
// 	tmpChar->SetPosition( 20.f, 0.f, 20.f );
// 	tmpChar->SetDirection( 0.f, 0.f, 1.f );
// 	tmpChar->SetDirInt( 0 );
// 	//����° ĳ���� ���� �Է�/////////////////////////////////////////////////
// 	tmpChar = GetCharMgr.GetCharSpace();
// 	tmpChar->SetSessionID( 3 );
// 	tmpChar->SetID( _T("areah") );
// 	tmpChar->SetTeam( SRV_CHAR_TEAM_ATT );
// 	tmpChar->SetState( SRV_CHAR_STATE_STAND );
// 	tmpChar->SetPosition( 30.f, 0.f, 30.f );
// 	tmpChar->SetDirection( 0.f, 0.f, 1.f );
// 	tmpChar->SetDirInt( 0 );
// 	//�׹�° ĳ���� ���� �Է�/////////////////////////////////////////////////
// 	tmpChar = GetCharMgr.GetCharSpace();
// 	tmpChar->SetSessionID( 4 );
// 	tmpChar->SetID( _T("aaa") );
// 	tmpChar->SetTeam( SRV_CHAR_TEAM_DEF );
// 	tmpChar->SetState( SRV_CHAR_STATE_STAND );
// 	tmpChar->SetPosition( 40.f, 0.f, 40.f );
// 	tmpChar->SetDirection( 0.f, 0.f, 1.f );
// 	tmpChar->SetDirInt( 0 );
// 	//�ټ���° ĳ���� ���� �Է�///////////////////////////////////////////////
// 	tmpChar = GetCharMgr.GetCharSpace();
// 	tmpChar->SetSessionID( 5 );
// 	tmpChar->SetID( _T("zzz") );
// 	tmpChar->SetTeam( SRV_CHAR_TEAM_ATT );
// 	tmpChar->SetState( SRV_CHAR_STATE_STAND );
// 	tmpChar->SetPosition( 50.f, 0.f, 50.f );
// 	tmpChar->SetDirection( 0.f, 0.f, 1.f );
// 	tmpChar->SetDirInt( 0 );
// 	//������° ĳ���� ���� �Է�///////////////////////////////////////////////
// 	tmpChar = GetCharMgr.GetCharSpace();
// 	tmpChar->SetSessionID( 6 );
// 	tmpChar->SetID( _T("qqq") );
// 	tmpChar->SetTeam( SRV_CHAR_TEAM_DEF );
// 	tmpChar->SetState( SRV_CHAR_STATE_STAND );
// 	tmpChar->SetPosition( 60.f, 0.f, 60.f );
// 	tmpChar->SetDirection( 0.f, 0.f, 1.f );
// 	tmpChar->SetDirInt( 0 );
// 	//�ϰ���° ĳ���� ���� �Է�///////////////////////////////////////////////
// 	tmpChar = GetCharMgr.GetCharSpace();
// 	tmpChar->SetSessionID( 7 );
// 	tmpChar->SetID( _T("asdf") );
// 	tmpChar->SetTeam( SRV_CHAR_TEAM_DEF );
// 	tmpChar->SetState( SRV_CHAR_STATE_STAND );
// 	tmpChar->SetPosition( 70.f, 0.f, 70.f );
// 	tmpChar->SetDirection( 0.f, 0.f, 1.f );
// 	tmpChar->SetDirInt( 0 );
// 	//�ϰ���° ĳ���� ���� �Է�///////////////////////////////////////////////
// 	tmpChar = GetCharMgr.GetCharSpace();
// 	tmpChar->SetSessionID( 8 );
// 	tmpChar->SetID( _T("www") );
// 	tmpChar->SetTeam( SRV_CHAR_TEAM_DEF );
// 	tmpChar->SetState( SRV_CHAR_STATE_STAND );
// 	tmpChar->SetPosition( 80.f, 0.f, 80.f );
// 	tmpChar->SetDirection( 0.f, 0.f, 1.f );
// 	tmpChar->SetDirInt( 0 );
// 	//�ϰ���° ĳ���� ���� �Է�///////////////////////////////////////////////
// 	tmpChar = GetCharMgr.GetCharSpace();
// 	tmpChar->SetSessionID( 9 );
// 	tmpChar->SetID( _T("ddd") );
// 	tmpChar->SetTeam( SRV_CHAR_TEAM_DEF );
// 	tmpChar->SetState( SRV_CHAR_STATE_STAND );
// 	tmpChar->SetPosition( 90.f, 0.f, 90.f );
// 	tmpChar->SetDirection( 0.f, 0.f, 1.f );
// 	tmpChar->SetDirInt( 0 );
// 	//�ϰ���° ĳ���� ���� �Է�///////////////////////////////////////////////
// 	tmpChar = GetCharMgr.GetCharSpace();
// 	tmpChar->SetSessionID( 10 );
// 	tmpChar->SetID( _T("ccc") );
// 	tmpChar->SetTeam( SRV_CHAR_TEAM_DEF );
// 	tmpChar->SetState( SRV_CHAR_STATE_STAND );
// 	tmpChar->SetPosition( 100.f, 0.f, 100.f );
// 	tmpChar->SetDirection( 0.f, 0.f, 1.f );
// 	tmpChar->SetDirInt( 0 );
	//////////////////////////////////////////////////////////////////////////
	
	return TRUE;
}

void GameSrvMain::Release()
{
	//���������� ����
	GetSrvNet.DisConnect();
}

#include "StdAfx.h"
#include "LogDataBase.h"

LogDataBase::LogDataBase(void)
{
}

LogDataBase::~LogDataBase(void)
{
}

BOOL LogDataBase::Init( TCHAR* filename )
{
	return m_query.ConnectMdb( filename );
}

void LogDataBase::Release()
{
	//큐에 패킷이 있다면 모두 넣어 주고 꺼야 한다

	//DB와 연결 해제
	m_query.DisConnect();
}

//======================================

BOOL LogDataBase::CreateServerTbl()
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("CREATE TABLE tblServerID( ID INT, ServerName VARCHAR(20) unique, PRIMARY KEY(ID) );") );

	if( !m_query.Exec( strQuery ) )
	{
		return FALSE;
	}

	m_query.Clear();

	//tbl을 만들었으면 기본항목을 넣어 준다///////////////////////////////////////
	ZeroMemory( strQuery, 255 );
	wsprintf( (TCHAR*)strQuery, _T("INSERT INTO tblServerID (ID, ServerName) VALUES (8800, 'SegLoginSrv');") );

	if( !m_query.Exec( strQuery ) )
		return FALSE;
	m_query.Clear();

	//////////////////////////////////////////////////////////////////////////
	ZeroMemory( strQuery, 255 );
	wsprintf( (TCHAR*)strQuery, _T("INSERT INTO tblServerID (ID, ServerName) VALUES (8900, 'SegLobbySrv');") );

	if( !m_query.Exec( strQuery ) )
		return FALSE;
	m_query.Clear();

	//////////////////////////////////////////////////////////////////////////
	ZeroMemory( strQuery, 255 );
	wsprintf( (TCHAR*)strQuery, _T("INSERT INTO tblServerID (ID, ServerName) VALUES (7900, 'SegGameSrv');") );

	if( !m_query.Exec( strQuery ) )
		return FALSE;
	m_query.Clear();

	//////////////////////////////////////////////////////////////////////////
	ZeroMemory( strQuery, 255 );
	wsprintf( (TCHAR*)strQuery, _T("INSERT INTO tblServerID (ID, ServerName) VALUES (9000, 'SegDBSrv');") );

	if( !m_query.Exec( strQuery ) )
		return FALSE;
	m_query.Clear();


	return TRUE;
}

BOOL LogDataBase::CreateTableListTbl()
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("CREATE TABLE tblTableNameList( tblName VARCHAR(20), Visible INT );") );

	if( !m_query.Exec( strQuery ) )
	{
		return FALSE;
	}

	m_query.Clear();
	return TRUE;
}

BOOL LogDataBase::CreateLogLvTbl()
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("CREATE TABLE tblLogLv( Lv INT, LvString VARCHAR(25), PRIMARY KEY(Lv, LvString) );") );

	if( !m_query.Exec( strQuery ) )
		return FALSE;
	m_query.Clear();

	//tbl을 만들었으면 항목을 넣어 준다///////////////////////////////////////
	ZeroMemory( strQuery, 255 );
	wsprintf( (TCHAR*)strQuery, _T("INSERT INTO tblLogLv (Lv, LvString) VALUES (0, 'LOG_LEVEL_ALL');") );

	if( !m_query.Exec( strQuery ) )
		return FALSE;
	m_query.Clear();

	//////////////////////////////////////////////////////////////////////////
	ZeroMemory( strQuery, 255 );
	wsprintf( (TCHAR*)strQuery, _T("INSERT INTO tblLogLv (Lv, LvString) VALUES (1, 'LOG_LEVEL_INFORMATION');") );

	if( !m_query.Exec( strQuery ) )
		return FALSE;
	m_query.Clear();

	//////////////////////////////////////////////////////////////////////////
	ZeroMemory( strQuery, 255 );
	wsprintf( (TCHAR*)strQuery, _T("INSERT INTO tblLogLv (Lv, LvString) VALUES (2, 'LOG_LEVEL_WORRNING');") );

	if( !m_query.Exec( strQuery ) )
		return FALSE;
	m_query.Clear();

	//////////////////////////////////////////////////////////////////////////
	ZeroMemory( strQuery, 255 );
	wsprintf( (TCHAR*)strQuery, _T("INSERT INTO tblLogLv (Lv, LvString) VALUES (3, 'LOG_LEVEL_ERROR');") );

	if( !m_query.Exec( strQuery ) )
		return FALSE;
	m_query.Clear();

	return TRUE;
}

//======================================

BOOL LogDataBase::CreateLogTable( TCHAR* tableName )
{
	SQLWCHAR	strQuery[255];

	//table을 하나 만들고
	wsprintf( (TCHAR*)strQuery, _T("CREATE TABLE %s( SrvID INT, LogLv INT, Log TEXT );"), tableName );

	if( !m_query.Exec( strQuery ) )
	{
		return FALSE;
	}

	m_query.Clear();

	ZeroMemory( strQuery, 255 );
	//tblList에 추가해 준다
	wsprintf( (TCHAR*)strQuery, _T("INSERT INTO tblTableNameList (tblName, Visible) VALUES ('%s', 1);"), tableName );

	if( !m_query.Exec( strQuery ) )
	{
		return FALSE;
	}

	m_query.Clear();
	return TRUE;
}

BOOL LogDataBase::InsertServerID( TCHAR* srvName, int ID )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("INSERT INTO tblServerID (ID,ServerName) VALUES ('%d','%s');"), ID, srvName );

	if( !m_query.Exec( strQuery ) )
	{
		return FALSE;
	}

	m_query.Clear();
	return TRUE;
}

void LogDataBase::SettingTableListCombobox( CComboBox* combo )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("SELECT tblName FROM tblTableNameList WHERE Visible=1;") );

	if( !m_query.Exec( strQuery ) )
	{
		MessageBox( NULL, _T("정상적으로 쿼리 되지 못했습니다\n전적으로 개발자 잘못이니 신고하세요ㅠ"), _T("죄송"), MB_OK );
		return;
	}

	//담기 전에 combobox초기화
	combo->ResetContent();

	TCHAR	tableName[25]={0};
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		ZeroMemory( tableName, 50 );
		m_query.GetStr( _T("tblName"), tableName );
		combo->AddString( tableName );
	}

	m_query.Clear();
}

void LogDataBase::SettingServerListCombobox( CComboBox* combo )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("SELECT ServerName FROM tblServerID;") );

	if( !m_query.Exec( strQuery ) )
	{
		MessageBox( NULL, _T("정상적으로 쿼리 되지 못했습니다\n전적으로 개발자 잘못이니 신고하세요ㅠ"), _T("죄송"), MB_OK );
		return;
	}

	//담기 전에 combobox초기화
	combo->ResetContent();

	TCHAR	SrvName[25]={0};
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		ZeroMemory( SrvName, 50 );
		m_query.GetStr( _T("ServerName"), SrvName );
		combo->AddString( SrvName );
	}

	m_query.Clear();
}

void LogDataBase::SettingLogLevelCombobox( CComboBox* combo )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("SELECT LvString FROM tblLogLv;") );

	if( !m_query.Exec( strQuery ) )
	{
		MessageBox( NULL, _T("정상적으로 쿼리 되지 못했습니다\n전적으로 개발자 잘못이니 신고하세요ㅠ"), _T("죄송"), MB_OK );
		return;
	}

	TCHAR	LvName[25]={0};
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		ZeroMemory( LvName, 50 );
		m_query.GetStr( _T("LvString"), LvName );
		combo->AddString( LvName );
	}

	m_query.Clear();
}

BOOL LogDataBase::DisabledTable( TCHAR* tableName )
{
	//--------------------------------------
	// 진짜 table을 없애는건 아니고
	// table list의 활성상태를 꺼주는것
	//--------------------------------------
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("UPDATE tblTableNameList SET Visible=0 WHERE tblName='%s';"), tableName );
	if( !m_query.Exec( strQuery ) )
	{
		MessageBox( NULL, _T("DELETE] 정상적으로 쿼리 되지 못했습니다\n전적으로 개발자 잘못이니 신고하세요ㅠ"), _T("죄송"), MB_OK );
		return FALSE;
	}

	m_query.Clear();

	return TRUE;
}

void LogDataBase::InsertLog( int serverId, int loglv, TCHAR* log )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("INSERT INTO %s (SrvID,LogLv,Log) VALUES ('%d','%d','%s');"), m_nowTable, serverId, loglv, log );

	if( !m_query.Exec( strQuery ) )
		return;

	m_query.Clear();
}

BOOL LogDataBase::SetLogList( TCHAR* table, TCHAR* Server, TCHAR* LogLevel, CListBox* listbox )
{
	SQLWCHAR	strQuery[255]={0,};
	//먼저 serverID를 받아 온다
	wsprintf( (TCHAR*)strQuery, _T("SELECT ID FROM tblServerID WHERE ServerName='%s';"), Server );

	if( !m_query.Exec( strQuery ) )
		return FALSE;

	int SrvId;
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		SrvId = m_query.GetInt( _T("ID") );
	}
	m_query.Clear();

	//다음은 로그레벨 받아 오기
	ZeroMemory( strQuery, 255 );
	wsprintf( (TCHAR*)strQuery, _T("SELECT Lv FROM tblLogLv WHERE LvString='%s';"), LogLevel);

	if( !m_query.Exec( strQuery ) )
		return FALSE;

	int LogLv;
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		LogLv = m_query.GetInt( _T("Lv") );
	}
	m_query.Clear();
	
	//이제 로그 받아 오기
	ZeroMemory( strQuery, 255 );

	if( LogLv == 0 )
	{
		//모든 Level의 log항목을 얻어 오는것
		wsprintf( (TCHAR*)strQuery
			, _T("SELECT Log FROM %s WHERE SrvID=%d;")
			, table, SrvId );
	}
	else
	{
		wsprintf( (TCHAR*)strQuery
			, _T("SELECT Log FROM %s WHERE LogLv=%d AND SrvID=%d;")
			, table, LogLv, SrvId );
	}

	if( !m_query.Exec( strQuery ) )
		return FALSE;

	//담기 전에 listbox초기화
	listbox->ResetContent();

	//담아 오기
	TCHAR log[512]={0,};
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		m_query.GetStr( _T("Log"), log );
		listbox->AddString( log );
		ZeroMemory( log, 512 );
	}

	//종료
	m_query.Clear();

	return TRUE;
}

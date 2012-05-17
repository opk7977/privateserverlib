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
	wsprintf( (TCHAR*)strQuery, _T("CREATE TABLE tblServerID( ID INT PRIMARY KEY, ServerName VARCHAR(20) );") );

	if( !m_query.Exec( strQuery ) )
	{
		return FALSE;
	}

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

	TCHAR	tableName[25]={0};
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		ZeroMemory( tableName, 25 );
		m_query.GetStr( _T("tblName"), tableName );
		combo->AddString( tableName );
	}

	m_query.Clear();
}

void LogDataBase::SettingServerListCombobox( CComboBox* combo )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("SELECT ID, ServerName FROM tblServerID;") );

	if( !m_query.Exec( strQuery ) )
	{
		MessageBox( NULL, _T("정상적으로 쿼리 되지 못했습니다\n전적으로 개발자 잘못이니 신고하세요ㅠ"), _T("죄송"), MB_OK );
		return;
	}

	int		SrvId;
	TCHAR	SrvName[25]={0};
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		SrvId = m_query.GetInt( _T("ID") );
		m_query.GetStr( _T("ServerName"), SrvName );

		TCHAR tmp[50]={0,};
		wsprintf( tmp, _T("%d %s"), SrvId, SrvName );
		combo->AddString( tmp );
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
	SQLWCHAR	strQuery[512];
	wsprintf( (TCHAR*)strQuery, _T("INSERT INTO tblServerID (SrvID,LogLv,Log) VALUES ('%d','%d','%s');"), serverId, loglv, log );

	if( !m_query.Exec( strQuery ) )
	{
		return FALSE;
	}

	m_query.Clear();
	return TRUE;
}

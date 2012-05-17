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
	//ť�� ��Ŷ�� �ִٸ� ��� �־� �ְ� ���� �Ѵ�

	//DB�� ���� ����
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

	//table�� �ϳ� �����
	wsprintf( (TCHAR*)strQuery, _T("CREATE TABLE %s( SrvID INT, LogLv INT, Log TEXT );"), tableName );

	if( !m_query.Exec( strQuery ) )
	{
		return FALSE;
	}

	m_query.Clear();

	ZeroMemory( strQuery, 255 );
	//tblList�� �߰��� �ش�
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
		MessageBox( NULL, _T("���������� ���� ���� ���߽��ϴ�\n�������� ������ �߸��̴� �Ű��ϼ����"), _T("�˼�"), MB_OK );
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
		MessageBox( NULL, _T("���������� ���� ���� ���߽��ϴ�\n�������� ������ �߸��̴� �Ű��ϼ����"), _T("�˼�"), MB_OK );
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
	// ��¥ table�� ���ִ°� �ƴϰ�
	// table list�� Ȱ�����¸� ���ִ°�
	//--------------------------------------
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("UPDATE tblTableNameList SET Visible=0 WHERE tblName='%s';"), tableName );
	if( !m_query.Exec( strQuery ) )
	{
		MessageBox( NULL, _T("DELETE] ���������� ���� ���� ���߽��ϴ�\n�������� ������ �߸��̴� �Ű��ϼ����"), _T("�˼�"), MB_OK );
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

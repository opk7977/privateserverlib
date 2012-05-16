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

BOOL LogDataBase::CreateServerTbl()
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("create table tblServerID (ID int, ServerName varchar2(20));") );

	if( !m_query.Exec( strQuery ) )
	{
		return FALSE;
	}

	m_query.Clear();
	return TRUE;
}

BOOL LogDataBase::CreateTableListTbl()
{
	return TRUE;
}

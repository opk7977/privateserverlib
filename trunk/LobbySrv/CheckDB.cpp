#include "CheckDB.h"
#include "SLogger.h"

CheckDB::CheckDB(void)
{
}

CheckDB::~CheckDB(void)
{
}

BOOL CheckDB::Init( TCHAR* filename )
{
	return m_query.ConnectMdb( filename );
}

void CheckDB::Relase()
{
	m_query.DisConnect();
}

void CheckDB::GetData( int in_sessionId, TCHAR* out_ID )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select U_ID from tblUser where ID=%d"), in_sessionId );

	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("CheckDB::GetData()\nSQLExecDirect Failed\n") );
		return;
	}

	while( m_query.Fetch() != SQL_NO_DATA )
	{
		m_query.GetStr( _T("U_ID"), out_ID );
	}
	m_query.Clear();
}


BOOL CheckDB::UpdateLogin( int sessionId, BOOL isLogin /*= TRUE */ )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("update tblUser set IS_LOGIN=%d where ID=%d"), isLogin, sessionId );
	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[LoginDB::UpdateLogin()] SQLExecDirect Failed(UpdateLogin())\n") );
		return FALSE;
	}

	return TRUE;
}

int CheckDB::IsLogin( int sessionID )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T( "select IS_LOGIN from tblUser where ID=%d"), sessionID );
	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[LoginDB::TryLogin()] SQLExecDirect Failed(AskLogin())\n") );
		return -10;
	}

	BOOL isLoginResult = FALSE;
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		isLoginResult = m_query.GetInt( _T("IS_LOGIN") );
	}
	m_query.Clear();


	return isLoginResult;
}
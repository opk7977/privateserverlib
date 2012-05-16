#include "CheckDB.h"
#include "SLogger.h"
#include "LobbyProtocol.h"

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

void CheckDB::Release()
{
	m_query.DisConnect();
}

void CheckDB::GetData( int in_sessionId, TCHAR* out_ID )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select U_ID from tblUser where ID=%d"), in_sessionId );

	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("CheckDB::GetData()\n%d¹ø session Äõ¸® ½ÇÆÐ\n%s\n\n"),
						in_sessionId,
						(TCHAR*)strQuery );
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
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::UpdateLogin()\n%d¹ø session Äõ¸® ½ÇÆÐ\n%s\n\n"),
						sessionId,
						(TCHAR*)strQuery );
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
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::TryLogin()\n%d¹ø session Äõ¸® ½ÇÆÐ\n%s\n\n"),
						sessionID,
						(TCHAR*)strQuery );
		return SERVER_ERROR;
	}

	int isLoginResult = -1;
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		isLoginResult = m_query.GetInt( _T("IS_LOGIN") );
	}
	m_query.Clear();


	return isLoginResult;
}
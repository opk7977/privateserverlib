#include "LoginDB.h"
#include "SLogger.h"

#include "LoginProtocol.h"

LoginDB::LoginDB(void)
{
	m_logger = &GetLogger;
}

LoginDB::~LoginDB(void)
{
}

BOOL LoginDB::Init( TCHAR* filename )
{
	return m_query.ConnectMdb( filename );
}

void LoginDB::Release()
{
	//꺼지기 전에 DB의 모든 계정의 로그인 정보를 초기화 해준다.
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("update tblUser set IS_LOGIN=0"), 0 );
	if( !m_query.Exec( strQuery ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::Release()\nSQLExecDirect Failed\n%s\n\n"),
						(TCHAR*)strQuery );
	}

	//종료
	m_query.DisConnect();
}

int LoginDB::CheckId( TCHAR* tstrID )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select ID from tblUser where U_ID='%s'"), tstrID );
 
	if( !m_query.Exec( strQuery ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::CheckId()\nSQLExecDirect Failed\n%s\n\n"),
						(TCHAR*)strQuery );
		return SERVER_ERROR;
	}

	int	id = -1;
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		id = m_query.GetInt( _T("ID") );
	}
	m_query.Clear();

	if( id > 0 )
	{	
		//id 값이 양수이면 현재 받아온 id가 있다->id가 있다
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::CheckId()\nID %s은(는) 중복됩니다.\n\n"),
						tstrID );
		return OVERLAPPED_ID;
	}

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginDB::CheckId()\nID %s은(는) 사용할 수 있습니다.\n\n"),
					tstrID );
	//id가 존재하지 않으면 사용할 수 있는 id다
	return (int)TRUE;
}

int LoginDB::AddAccount( TCHAR* tstrID, TCHAR* tstrPW, TCHAR* tstrMAILE )
{
	SQLWCHAR	strQuery[512];
	wsprintf( (TCHAR*)strQuery, _T("insert into tblUser (U_ID,U_PW,E_MAIL) VALUES ('%s','%s','%s')"), tstrID, tstrPW, tstrMAILE );

	if( !m_query.Exec(strQuery) )
	{
		//쿼리 실패
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::AddAccount()\nSQLExecDirect Failed\n%s\n\n"),
						_T("[ID %s] 계정생성에 실패 했습니다."),
						(TCHAR*)strQuery,
						tstrID );
		return SERVER_ERROR;
	}

	//쿼리 성공
	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginDB::AddAccount()\n[ID %s] 계정 생성에 성공하였습니다.\n\n"),
					tstrID );

	m_query.Clear();
	return (int)TRUE;
}

int LoginDB::TryLogin( TCHAR* tstrID, TCHAR* tstrPW )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select ID, U_ID,U_PW from tblUser where U_ID='%s'"), tstrID );

	if( !m_query.Exec( strQuery ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::TryLogin()\nSQLExecDirect Failed(AskLogin())\n%s\n\n"),
						(TCHAR*)strQuery );
		return SERVER_ERROR;
	}

	int			id = 0;
	TCHAR		name[50]={0};
	TCHAR		pass[50]={0};
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		id = m_query.GetInt( _T("ID") );
		m_query.GetStr( _T("U_ID"), name );
		m_query.GetStr( _T("U_PW"), pass );
	}
	m_query.Clear();

	if( id <= 0 )
	{	
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::TryLogin()\n로그인 실패_ ID %s가 존재하지 않습니다.\n\n"),
						tstrID );
		return NONEXISTENT_ID;
	}

	//id는 이제 확인 됨
	if( _tcscmp( tstrPW, pass ) != 0 )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::TryLogin()\n로그인 실패_ ID %s의 비번이 틀립니다.\n\n"),
						tstrID );
		return WRONG_PW;		//비번이 틀림
	}
	else
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::TryLogin()\nID %s 로그인 성공.\n\n"),
						tstrID );
		return id;
	}
}

BOOL LoginDB::UpdateLogin( int sessionId, BOOL isLogin /*= TRUE */ )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("update tblUser set IS_LOGIN=%d where ID=%d"), isLogin, sessionId );
	if( !m_query.Exec( strQuery ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::UpdateLogin()\nSQLExecDirect Failed(UpdateLogin())\n%s\n\n"),
						(TCHAR*)strQuery );
		return FALSE;
	}

	return TRUE;
}

int LoginDB::IsLogin( int sessionID )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T( "select IS_LOGIN from tblUser where ID=%d"), sessionID );
	if( !m_query.Exec( strQuery ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::TryLogin()\nSQLExecDirect Failed(AskLogin())\n%s\n\n"),
						(TCHAR*)strQuery);
		return SERVER_ERROR;
	}

	BOOL isLoginResult = FALSE;
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		isLoginResult = m_query.GetInt( _T("IS_LOGIN") );
	}
	m_query.Clear();

	return isLoginResult;
}

#include "LoginDB.h"
#include "SLogger.h"

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
	m_query.Exec( strQuery );

	m_query.DisConnect();
}

int LoginDB::CheckId( TCHAR* tstrID )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select ID from tblUser where U_ID='%s'"), tstrID );
 
	if( !m_query.Exec( strQuery ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[LoginDB::CheckId()] SQLExecDirect Failed\n") );
		return -10;
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
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[LoginDB::CheckId()] 중복되는 ID가 존재합니다.\n") );
		return -1;
	}

	//id가 존재하지 않으면 사용할 수 있는 id다
	return 1;
}

int LoginDB::AddAccount( TCHAR* tstrID, TCHAR* tstrPW, TCHAR* tstrMAILE )
{
	SQLWCHAR	strQuery[512];
	wsprintf( (TCHAR*)strQuery, _T("insert into tblUser (U_ID,U_PW,E_MAIL) VALUES ('%s','%s','%s')"), tstrID, tstrPW, tstrMAILE );

	if( !m_query.Exec(strQuery) )
	{
		//쿼리 실패
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[LoginDB::AddAccount()] SQLExecDirect Failed\n") );
		return -1;
	}

	//쿼리 성공
	m_query.Clear();
	return 1;
}

int LoginDB::TryLogin( TCHAR* tstrID, TCHAR* tstrPW )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select ID, U_ID,U_PW from tblUser where U_ID='%s'"), tstrID );

	if( !m_query.Exec( strQuery ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[LoginDB::TryLogin()] SQLExecDirect Failed(AskLogin())\n") );
		return -10;
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
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[LoginDB::TryLogin()] 로그인 실패_ ID가 존재하지 않습니다.\n\n") );
		return -1;
	}

	//id는 이제 확인 됨
	if( _tcscmp( tstrPW, pass ) != 0 )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[LoginDB::TryLogin()] 로그인 실패_ 비번이 틀립니다.\n\n") );
		return 0;		//비번이 틀림
	}
	else
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[LoginDB::TryLogin()] 로그인 성공.\n\n") );
		return id;
	}
}

BOOL LoginDB::UpdateLogin( int sessionId, BOOL isLogin /*= TRUE */ )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("update tblUser set IS_LOGIN=%d where ID=%d"), isLogin, sessionId );
	if( !m_query.Exec( strQuery ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[LoginDB::UpdateLogin()] SQLExecDirect Failed(UpdateLogin())\n\n") );
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
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[LoginDB::TryLogin()] SQLExecDirect Failed(AskLogin())\n\n") );
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

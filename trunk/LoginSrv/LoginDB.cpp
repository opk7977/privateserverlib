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
	//������ ���� DB�� ��� ������ �α��� ������ �ʱ�ȭ ���ش�.
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("update tblUser set IS_LOGIN=0"), 0 );
	if( !m_query.Exec( strQuery ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::Release()\nSQLExecDirect Failed\n%s\n\n"),
						(TCHAR*)strQuery );
	}

	//����
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
		//id ���� ����̸� ���� �޾ƿ� id�� �ִ�->id�� �ִ�
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::CheckId()\nID %s��(��) �ߺ��˴ϴ�.\n\n"),
						tstrID );
		return OVERLAPPED_ID;
	}

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginDB::CheckId()\nID %s��(��) ����� �� �ֽ��ϴ�.\n\n"),
					tstrID );
	//id�� �������� ������ ����� �� �ִ� id��
	return (int)TRUE;
}

int LoginDB::AddAccount( TCHAR* tstrID, TCHAR* tstrPW, TCHAR* tstrMAILE )
{
	SQLWCHAR	strQuery[512];
	wsprintf( (TCHAR*)strQuery, _T("insert into tblUser (U_ID,U_PW,E_MAIL) VALUES ('%s','%s','%s')"), tstrID, tstrPW, tstrMAILE );

	if( !m_query.Exec(strQuery) )
	{
		//���� ����
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::AddAccount()\nSQLExecDirect Failed\n%s\n\n"),
						_T("[ID %s] ���������� ���� �߽��ϴ�."),
						(TCHAR*)strQuery,
						tstrID );
		return SERVER_ERROR;
	}

	//���� ����
	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginDB::AddAccount()\n[ID %s] ���� ������ �����Ͽ����ϴ�.\n\n"),
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
						_T("LoginDB::TryLogin()\n�α��� ����_ ID %s�� �������� �ʽ��ϴ�.\n\n"),
						tstrID );
		return NONEXISTENT_ID;
	}

	//id�� ���� Ȯ�� ��
	if( _tcscmp( tstrPW, pass ) != 0 )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::TryLogin()\n�α��� ����_ ID %s�� ����� Ʋ���ϴ�.\n\n"),
						tstrID );
		return WRONG_PW;		//����� Ʋ��
	}
	else
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LoginDB::TryLogin()\nID %s �α��� ����.\n\n"),
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

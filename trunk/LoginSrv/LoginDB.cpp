#include "LoginDB.h"
#include "SLogger.h"

LoginDB::LoginDB(void)
{
}

LoginDB::~LoginDB(void)
{
}

BOOL LoginDB::Init( TCHAR* filename )
{
	return m_query.ConnectMdb( filename );
}

void LoginDB::Relase()
{
	m_query.DisConnect();
}

int LoginDB::CheckId( TCHAR* tstrID )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select ID from tblUser where U_ID='%s'"), tstrID );
 
	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[LoginDB::CheckId()] SQLExecDirect Failed\n") );
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
		//id ���� ����̸� ���� �޾ƿ� id�� �ִ�->id�� �ִ�
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[LoginDB::CheckId()] �ߺ��Ǵ� ID�� �����մϴ�.\n") );
		return -1;
	}

	//id�� �������� ������
	return 1;
}

int LoginDB::AddAccount( TCHAR* tstrID, TCHAR* tstrPW, TCHAR* tstrMAILE )
{
	SQLWCHAR	strQuery[512];
	wsprintf( (TCHAR*)strQuery, _T("insert into tblUser (U_ID,U_PW,E_MAIL) VALUES ('%s','%s','%s')"), tstrID, tstrPW, tstrMAILE );

	if( !m_query.Exec(strQuery) )
	{
		//���� ����
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[LoginDB::AddAccount()] SQLExecDirect Failed\n") );
		return -1;
	}

	//���� ����
	m_query.Clear();
	return 1;
}

int LoginDB::TryLogin( TCHAR* tstrID, TCHAR* tstrPW )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select ID, U_ID,U_PW from tblUser where U_ID='%s'"), tstrID );

	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[LoginDB::TryLogin()] SQLExecDirect Failed(AskLogin())\n") );
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
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[LoginDB::TryLogin()] �α��� ����_ ID�� �������� �ʽ��ϴ�.\n") );
		return -1;
	}

	//id�� ���� Ȯ�� ��
	if( _tcscmp( tstrPW, pass ) != 0 )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[LoginDB::TryLogin()] �α��� ����_ ����� Ʋ���ϴ�.\n") );
		return 0;		//����� Ʋ��
	}
	else
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[LoginDB::TryLogin()] �α��� ����.\n") );
		return id;
	}
}
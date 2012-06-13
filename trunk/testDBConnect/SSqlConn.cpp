#include "SSqlConn.h"


SSqlConn::SSqlConn(void)
{
}

SSqlConn::~SSqlConn(void)
{
}

BOOL SSqlConn::Init( TCHAR* srvName, TCHAR* _id, TCHAR* _pw )
{
	if( !m_query.ConnectSrv( srvName, _id, _pw ) )
		return FALSE;

	return TRUE;
}

// BOOL SSqlConn::PrintAllRanksInfo()
// {
// // 	if( SQLExecDirect( hStmt, (SQLWCHAR*)_T("{call testAll};"), SQL_NTS ) != SQL_SUCCESS )
// // 		return FALSE;
// 
// 	if( !m_query.Exec( _T("{call testAll};") ) )
// 		return FALSE;
// 
// 	while( m_query.Fetch() != SQL_NO_DATA )
// 	{
// 		_tprintf( _T("%d\t%d\t%d\n"), m_query.GetInt( _T("RANK_ID") ), m_query.GetInt( _T("RANK_NAME") ), m_query.GetInt( _T("RANK_NEED_POINT") ) );
// 	}
// 
// 	m_query.Clear();
// 
// 	return TRUE;
// }

void SSqlConn::Release()
{
	m_query.DisConnect();
}

int SSqlConn::CheckID( TCHAR* _id )
{
	// return 받을 결과값
	int result = 0;
	SQLINTEGER iJnk = SQL_NTS;
	SQLRETURN sqlret;

	sqlret = m_query.BindParamaterInt( 1, result, iJnk, FALSE );
	sqlret = m_query.BindParamaterStr( 2, _id, iJnk, TRUE );
	
	if( !m_query.Exec( _T("{?=call CheckID(?)};") ) )
	{
		return -10;
	}

	printf("%d\n", result );

	return result;
}

int SSqlConn::CreateAccount( TCHAR* _id, TCHAR* _pw, TCHAR* _mail )
{
	SQLINTEGER iJnk = SQL_NTS;
	SQLRETURN sqlret;

	sqlret = m_query.BindParamaterStr( 1, _id, iJnk, TRUE );
	sqlret = m_query.BindParamaterStr( 2, _pw, iJnk, TRUE );
	sqlret = m_query.BindParamaterStr( 3, _mail, iJnk, TRUE );

	if( !m_query.Exec( _T("{call InsertAccountItem(?,?,?)};") ) )
	{
		return -10;
	}

	return 1;
}

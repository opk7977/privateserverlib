#include "SQuery.h"

#ifdef _DEBUG
	#include "SLogger.h"
#endif

SQuery::SQuery(void)
{
}

SQuery::~SQuery(void)
{
	//DisConnect();
}

BOOL SQuery::ConnectMdb( TCHAR* conStr )
{
	SQLRETURN	retcode;

	//환경핸들 할당 받기//////////////////////////////////////////////////////
	retcode = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] 환경핸들 할당 실패\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 환경핸들 할당 성공\n") );
#endif
	//////////////////////////////////////////////////////////////////////////

	//환경핸들 버전을 3.0으로 설정////////////////////////////////////////////
	retcode = SQLSetEnvAttr( m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] 환경핸들 버젼 설정 실패\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 환경핸들 버젼 설정 성공\n") );
#endif
	//////////////////////////////////////////////////////////////////////////

	//연결핸들 할당하고 환경핸들에 연결하기///////////////////////////////////
	retcode = SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hDbc );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] 연결핸들 할당 실패\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 연결핸들 할당 성공\n") );
#endif
	//////////////////////////////////////////////////////////////////////////

	//mdb에 연결//////////////////////////////////////////////////////////////
	//경로를 문자열로 완성하기 위한 변수
	TCHAR		Dir[255];

	//현재 문서의 경로를 얻어 온다.
	GetCurrentDirectory( 255, Dir );
	//경로와 mdb문서의 주소를 합쳐 완성된 경로를 만든다.
	wsprintf( (TCHAR*)ConnectDirMdb, _T("DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s\\%s;"), Dir, conStr );
	//로그로 경로 출력
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] %s\n"), (TCHAR*)ConnectDirMdb );
#endif

	retcode = SQLDriverConnect( m_hDbc, NULL, ConnectDirMdb, sizeof(ConnectDirMdb), NULL, 0, NULL, SQL_DRIVER_NOPROMPT );
	//연결에 문제가 있다면 return하고 log를 남긴다.
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] mdb연결 실패\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] mdb연결 성공\n") );
#endif

	//명령 핸들을 할당하자
	retcode = SQLAllocHandle( SQL_HANDLE_STMT, m_hDbc, &m_hStmt );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] 명령핸들 할당 실패\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 명령핸들 할당 성공\n") );
#endif

	//끗
	return TRUE; 
}

BOOL SQuery::ConnectSrv( TCHAR* conStr, TCHAR* srvID, TCHAR* srvPW )
{
	SQLRETURN	retcode;

	//환경핸들 할당 받기//////////////////////////////////////////////////////
	retcode = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] 환경핸들 할당 실패\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 환경핸들 할당 성공\n") );
#endif
	//////////////////////////////////////////////////////////////////////////

	//환경핸들 버전을 3.0으로 설정////////////////////////////////////////////
	retcode = SQLSetEnvAttr( m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] 환경핸들 버젼 설정 실패\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 환경핸들 버젼 설정 성공\n") );
#endif
	//////////////////////////////////////////////////////////////////////////

	//연결핸들 할당하고 환경핸들에 연결하기///////////////////////////////////
	retcode = SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hDbc );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] 연결핸들 할당 실패\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 연결핸들 할당 성공\n") );
#endif
	//////////////////////////////////////////////////////////////////////////

	//Srv에 연결//////////////////////////////////////////////////////////////
	retcode = SQLConnect( m_hDbc, conStr, SQL_NTS, srvID, SQL_NTS, srvPW, SQL_NTS );
	//연결에 문제가 있다면 return하고 log를 남긴다.
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] mdb연결 실패\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] mdb연결 성공\n") );
#endif

	//명령 핸들을 할당하자
	retcode = SQLAllocHandle( SQL_HANDLE_STMT, m_hDbc, &m_hStmt );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] 명령핸들 할당 실패\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 명령핸들 할당 성공\n") );
#endif

	//끗
	return TRUE; 
}

void SQuery::DisConnect()
{
	//정리는 할당 했던 반대 순서로 해 줘야 한다.
	//명령핸들
	if( m_hStmt )
	{
		SQLFreeHandle( SQL_HANDLE_STMT, m_hStmt );
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 명령 핸들 할당 해제\n") );
#endif
	}

	//연결핸들
	if( m_hDbc )
	{
		//db와의 연결부터 끊어 준다
		SQLDisconnect( m_hDbc );
		SQLFreeHandle( SQL_HANDLE_DBC, m_hDbc );
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 연결 핸들 할당 해제\n") );
#endif
	}

	//환결핸들
	if( m_hEnv )
	{
		SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 환경 핸들 할당 해제\n") );
#endif
	}
}

SQLRETURN SQuery::BindParamaterStr( int index, TCHAR* strParam, SQLINTEGER &jnk, BOOL isIn /*= TRUE */ )
{
	SQLSMALLINT inOut;
	SQLINTEGER iJnk = SQL_NTS;

	if( isIn )
		inOut = SQL_PARAM_INPUT;
	else
		inOut = SQL_PARAM_OUTPUT;

	return SQLBindParameter( m_hStmt, index, inOut, SQL_C_WCHAR, SQL_WCHAR, _tcslen(strParam), 0, strParam, 0, &jnk );
}

SQLRETURN SQuery::BindParamaterInt( int index, int &iParam, SQLINTEGER &jnk, BOOL isIn /*= TRUE */ )
{
	SQLSMALLINT inOut;

	if( isIn )
		inOut = SQL_PARAM_INPUT;
	else
		inOut = SQL_PARAM_OUTPUT;

	return SQLBindParameter( m_hStmt, index, inOut, SQL_C_LONG, SQL_INTEGER, 0, 0, &iParam, 0, &jnk );
}

BOOL SQuery::Exec( TCHAR* szSQL )
{
	//명령핸들이 연결되어 있지 않다면 return ( 방어 코드 )
	if( !m_hStmt )
		return FALSE;

	SQLRETURN	ret;
	ret = SQLExecDirect( m_hStmt, (SQLWCHAR*)szSQL, SQL_NTS );
	//실패한 경우는 false를 return해 주고log에 남긴다
	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] 쿼리 실패\n%s\n\n"), szSQL );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 쿼리 성공\n")
												 _T("\t%s\n"), szSQL );
#endif

	//레코드의 개수를 구한다.
	SQLRowCount( m_hStmt, &m_iRowCount );
	//컬럼의 개수를 구한다.
	SQLNumResultCols( m_hStmt, &m_iColCount );
	//컬럼의 개수가 최대치를 넘어 가면 안된다
	if( m_iColCount > MAX_COL )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 컬럼의 개수가 최대치를 넘어 갑니다.\n") );
#endif
		return FALSE;
	}

	//컬럼의 개수가 0인경우 - bind할 필요가 없는 경우의 쿼리( delete or insert )
	if( m_iColCount == 0 )
	{
		//쿼리문의 연결을 끊어 주고
		Clear();
		return TRUE;
	}

	//bind해 줘야 한다.
	//col배열은 0부터 컬럼 번호는 1부터!!
	for( int i=0; i<m_iColCount; ++i )
	{
		SQLBindCol( m_hStmt, i+1, SQL_C_TCHAR, m_tstrBindCol[i], 255, &m_iCol[i] );
		//컬럼의 스키마들을 받아 온다( 속성의 이름들 )
		SQLDescribeCol( m_hStmt, i+1, m_tchColName[i], 40, NULL, NULL, NULL, NULL, NULL );
	}

	return TRUE;
}

SQLRETURN SQuery::Fetch()
{
	return SQLFetch( m_hStmt );
}

void SQuery::Clear()
{
	SQLCloseCursor( m_hStmt );
	SQLFreeStmt( m_hStmt, SQL_UNBIND );

	m_iRowCount = 0;
}

int SQuery::GetInt( int nCol )
{
	if( nCol > m_iColCount )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 넘겨 받은 int가 컬럼 count보다 큽니다.\n") );
#endif
		return -1;
	}
	if( m_iCol[nCol-1] == SQL_NULL_DATA )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 데이터가 없습니다\n") );
#endif
		return -1;
	}
	else
	{
		int aaa = _ttoi( m_tstrBindCol[nCol-1] );
		return aaa;
	}
}

int SQuery::GetInt( TCHAR* sCol )
{
	//받아온 컬럼의 이름으로 index를 검색한다
	int n = FindCollasName( sCol );
	if( n < 0 )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 입력한 Col Name에 해당하는 컬럼이 없습니다\n") );
#endif
		return -1;
	}
	else
		return GetInt( n );
}

void SQuery::GetStr( int nCol, TCHAR* buf )
{
	//ZeroMemory( buf, 255 );

	if( nCol > m_iColCount )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 넘겨 받은 int가 컬럼 count보다 큽니다.\n") );
#endif
		return;
	}
	if( m_iCol[nCol-1] == SQL_NULL_DATA )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 데이터가 없습니다\n") );
#endif
		lstrcpy( buf, _T("NULL") );
	}
	else
	{
		CopyMemory( buf, m_tstrBindCol[nCol-1], wcslen(m_tstrBindCol[nCol-1])*sizeof(TCHAR) );
	}
}

void SQuery::GetStr( TCHAR* sCol, TCHAR* buf )
{
	int n = FindCollasName( sCol );
	if( n < 0 )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] 입력한 Col Name에 해당하는 컬럼이 없습니다\n") );
#endif
		//ZeroMemory( buf, 255 );
	}
	else
		GetStr( n, buf );
}

int SQuery::FindCollasName( TCHAR* colName )
{
	for( int i=0; i<m_iColCount; ++i )
	{
		//결과가 0이라면 같은 문자열이니까
		if( wcscmp( colName, (LPCTSTR)m_tchColName[i] ) == 0 )
			return i+1;
	}
	//일치하는 컬럼이 없으면 -1을 return한다.
	return -1;
}

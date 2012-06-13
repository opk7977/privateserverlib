#include "SSqlConn.h"
#include <locale.h>


// SQLHENV			hEnv;
// SQLHDBC			hDbc;
// SQLHSTMT		hStmt;
// 
// BOOL DBConnect();
// void Release();
// BOOL DBExcuteSQL();


void main()
{
// 	_tsetlocale(LC_ALL, _T("Korean"));
// 
// 	if( DBConnect() == FALSE )
// 	{
// 		printf("실패!\n");
// 		exit(1);
// 	}
// 
// 	if( DBExcuteSQL() == FALSE )
// 	{
// 		printf("실패2\n");
// 		exit(1);
// 	}
// 	Release();
	SSqlConn qu;

	if( !qu.Init( _T("Unknown"), _T("sa"), _T("1234") ) )
		return;

	TCHAR tmp[10] = _T("areah1");
	int result = qu.CheckID( tmp );
	if( result == -10 )
		printf("서버에러\n");
	else if( result > 0 )
		printf("사용할 수 없는 id\n");
	else
		printf("사용할 수 있는 id\n");

	result = qu.CreateAccount( _T("Edward"), _T("Edward"), _T("Edward") );
	if( result == -10 )
		printf("만들기 실패!\n");
	else
		printf("만들기 성공!\n");


	qu.Release();
	
}

// BOOL DBConnect()
// {
// 	SQLRETURN sqlret;
// 
// 	sqlret = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv );
// 	if( sqlret != SQL_SUCCESS && sqlret != SQL_SUCCESS_WITH_INFO )
// 		return FALSE;
// 
// 	sqlret = SQLSetEnvAttr( hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER );
// 	if( sqlret != SQL_SUCCESS && sqlret != SQL_SUCCESS_WITH_INFO )
// 		return FALSE;
// 
// 	sqlret = SQLAllocHandle( SQL_HANDLE_DBC, hEnv, &hDbc );
// 	if( sqlret != SQL_SUCCESS && sqlret != SQL_SUCCESS_WITH_INFO )
// 		return FALSE;
// 
// 	//sqlret = SQLConnect( hDbc, _T("test"), SQL_NTS, _T("sa"), SQL_NTS, _T("1234"), SQL_NTS );
// 	sqlret = SQLConnect( hDbc, _T("Unknown"), SQL_NTS, _T("sa"), SQL_NTS, _T("1234"), SQL_NTS );
// 	if( sqlret != SQL_SUCCESS && sqlret != SQL_SUCCESS_WITH_INFO )
// 		return FALSE;
// 
// 	sqlret = SQLAllocHandle( SQL_HANDLE_STMT, hDbc, &hStmt );
// 	if( sqlret != SQL_SUCCESS && sqlret != SQL_SUCCESS_WITH_INFO )
// 		return FALSE;
// 
// 	return TRUE;
// }
// 
// void Release()
// {
// 	if( hStmt )	SQLFreeHandle( SQL_HANDLE_STMT, hStmt );
// 	if( hDbc )	SQLDisconnect( hDbc );
// 	if( hDbc )	SQLFreeHandle( SQL_HANDLE_DBC, hDbc );
// 	if( hEnv )	SQLFreeHandle( SQL_HANDLE_ENV, hEnv );
// }
// 
// BOOL DBExcuteSQL()
// {
// // 	SQLWCHAR	MemName[5], Jumin[14], Addr[30];
// // 	SQLINTEGER	iName, iJumin, iAddr;
// // 
// // 	SQLBindCol( hStmt, 1, SQL_WCHAR, MemName, sizeof( MemName ), &iName );
// // 	SQLBindCol( hStmt, 2, SQL_WCHAR, Jumin, sizeof( Jumin ), &iJumin );
// // 	SQLBindCol( hStmt, 3, SQL_WCHAR, Addr, sizeof( Addr ), &iAddr );
// // 
// // 	if( SQLExecDirect( hStmt, (SQLWCHAR*)_T("select * from memberTbl"), SQL_NTS ) != SQL_SUCCESS )
// // 		return FALSE;
// // 
// // 	printf( "memberName\tJumin\t\tAddr\n" );
// // 	while( SQLFetch( hStmt ) != SQL_NO_DATA )
// // 		_tprintf( _T("%s\t\t%s\t%s\n"), MemName, Jumin, Addr );
// // 
// // 	if( hStmt ) SQLCloseCursor( hStmt );
// 
// 
// // 	SQLWCHAR	ID[4];
// // 	SQLINTEGER	iID;
// // 
// // 	SQLBindCol( hStmt, 1, SQL_WCHAR, ID, sizeof( ID ), &iID );
// // 
// // 	if( SQLExecDirect( hStmt, (SQLWCHAR*)_T("select ID from AccountTbl"), SQL_NTS ) != SQL_SUCCESS )
// // 		return FALSE;
// // 
// // 	while( SQLFetch( hStmt ) != SQL_NO_DATA )
// // 		_tprintf( _T("%d"), _ttoi(ID) );
// // 
// // 	if( hStmt ) SQLCloseCursor( hStmt );
// 
// // 	SQLRETURN retcode;
// // 
// // 	SQLWCHAR	id[10] = _T("areah"), tmp[10];
// // 	SQLINTEGER	iId=0;
// // 	SQLINTEGER	iJnk=SQL_NTS;
// // 	retcode = SQLBindParameter( hStmt, 1, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_C_TCHAR, 10, 0, &id, 0, &iJnk );
// // 	retcode = SQLBindCol( hStmt, 1, SQL_WCHAR, tmp, sizeof( tmp ), &iJnk );
// // 
// // 	if( SQLExecDirect( hStmt, (SQLWCHAR*)_T("select ID from AccountTbl where USER_ID=?;"), SQL_NTS ) != SQL_SUCCESS )
// // 		return FALSE;
// // 
// // 	while( SQLFetch( hStmt ) != SQL_NO_DATA )
// // 	{
// // 		_tprintf( _T("%d"), _ttoi(tmp) );
// // 	}
// // 
// // 	if( hStmt ) SQLCloseCursor( hStmt );
// 
// 
// 	SQLRETURN retcode;
// 
// 	SQLINTEGER	rankID = 2, iJnk=SQL_NTS, result[3];
// 	SQLTCHAR	resultName[10]={0,};
// 	{
// 	int			rankID1= 1;
// 
// 	retcode = SQLBindParameter( hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &rankID1, 0, &iJnk );
// 	}
// 	retcode = SQLBindCol( hStmt, 1, SQL_INTEGER, &result[0], sizeof( result[0] ), &iJnk );
// 	retcode = SQLBindCol( hStmt, 2, SQL_C_WCHAR, &resultName, sizeof( resultName ), &iJnk );
// 	retcode = SQLBindCol( hStmt, 3, SQL_INTEGER, &result[1], sizeof( result[1] ), &iJnk );
// 	retcode = SQLBindCol( hStmt, 4, SQL_INTEGER, &result[2], sizeof( result[2] ), &iJnk );
// 
// 	retcode = SQLExecDirect( hStmt, (SQLWCHAR*)_T("{call test(?)};"), SQL_NTS );
// 	if( retcode != SQL_SUCCESS )
// 		return FALSE;
// 
// 	while( SQLFetch( hStmt ) != SQL_NO_DATA )
// 	{
// 		_tprintf( _T("%d\t%s\t%d\t%d\n"), result[0], resultName, result[1], result[2] );
// 	}
// 
// // 	if( hStmt ) SQLCloseCursor( hStmt );
// // 
// // 	SQLRETURN retcode;
// // 
// // 	SQLINTEGER	iJnk=SQL_NTS, result[4];
// // 
// // 	retcode = SQLBindCol( hStmt, 1, SQL_INTEGER, &result[0], sizeof( result[0] ), &iJnk );
// // 	retcode = SQLBindCol( hStmt, 2, SQL_INTEGER, &result[1], sizeof( result[1] ), &iJnk );
// // 	retcode = SQLBindCol( hStmt, 3, SQL_INTEGER, &result[2], sizeof( result[2] ), &iJnk );
// // 	retcode = SQLBindCol( hStmt, 4, SQL_INTEGER, &result[3], sizeof( result[3] ), &iJnk );
// // 
// // 	if( SQLExecDirect( hStmt, (SQLWCHAR*)_T("{call testAll};"), SQL_NTS ) != SQL_SUCCESS )
// // 		return FALSE;
// // 
// // 	while( SQLFetch( hStmt ) != SQL_NO_DATA )
// // 	{
// // 		_tprintf( _T("%d\t%d\t%d\t%d\n"), result[0], result[1], result[2], result[3] );
// // 	}
// // 
// // 	if( hStmt ) SQLCloseCursor( hStmt );
// 
// // 	int result = 0;
// // 	SQLINTEGER iJnk = SQL_NTS;
// // 	SQLRETURN sqlret;
// // 	TCHAR _id[10] = _T("areah");
// // 
// // 	sqlret = SQLBindParameter( hStmt, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &result, 0, &iJnk );
// // 	sqlret = SQLBindParameter( hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, _tcslen(_id), 0, &_id, 0, &iJnk );
// // 
// // 	sqlret = SQLExecDirect( hStmt, (SQLWCHAR*)_T("{?=call CheckID(?)};"), SQL_NTS );
// // 	if( sqlret != SQL_SUCCESS )
// // 		return FALSE;
// // 
// // 	if( hStmt ) SQLCloseCursor( hStmt );
// // 
// // 	printf("%d\n", result );
// 
// 	return TRUE;
// 
// }


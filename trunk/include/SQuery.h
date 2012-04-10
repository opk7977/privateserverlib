#ifndef __DATABASE_QUERY__

#include "SServerobj.h"

#include <sql.h>
#include <sqlext.h>

class SQuery : public SServerObj
{
public:
	enum
	{
		MAX_COL	= 100,
	};

private:
	//ODBC변수들
	SQLHENV					m_hEnv;		//환경핸들
	SQLHDBC					m_hDbc;		//연결핸들

	SQLHSTMT				m_hStmt;	//명령핸들

	//mdb주소를 담고 있는 문자열 변수
	SQLWCHAR				ConnectDirMdb[255];
	
	//DB에서 Binding될 문자열 공간
	TCHAR					m_tstrBindCol[MAX_COL][255];

	SQLINTEGER				m_iRowCount;                //검색된(?) 레코드 개수
	SQLSMALLINT				m_iColCount;				//컬럼 개수
	SQLWCHAR				m_tchColName[MAX_COL][50];	//컬럼의 이름들(스키마)를 담는다.
	SQLINTEGER				m_iCol[MAX_COL];			//컬럼의 길이, 상태

public:
	SQuery(void);
	virtual ~SQuery(void);
	
	//mdb 연결( mdb에 직접 연결 )
	BOOL ConnectMdb( TCHAR* conStr );
	
	//연결 끊기
	void DisConnect();

	//쿼리분을 받아 실행
	BOOL Exec( TCHAR* szSQL );

	//한행 가져오기
	SQLRETURN Fetch();
	//명령 쿼리 연결 끊기
	//다음 쿼리를 위해 쿼리가 끝나면 선언해 줘야 한다.
	void Clear();

	//정수형 컬럼 읽기
	int GetInt( int nCol );					//내부적으로 사용
	int GetInt( TCHAR* sCol );

	//문자열형 컬럼 읽기
	void GetStr( int nCol, TCHAR* buf );	//내부적으로 사용
	void GetStr( TCHAR* sCol, TCHAR* buf );

	//컬럼의 이름으로 검색해서 해당 index를 찾아 준다.
	int FindCollasName( TCHAR* colName );
};

#endif
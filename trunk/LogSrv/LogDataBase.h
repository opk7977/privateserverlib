#pragma once

#include "SQuery.h"

#pragma comment( lib, "odbc32.lib" )

//class CComboBox;

class LogDataBase : public SSingleton <LogDataBase>
{
private:
	friend class SSingleton<LogDataBase>;

private:
	SQuery		m_query;

	TCHAR		m_nowTable[20];

private:
	LogDataBase(void);
	~LogDataBase(void);

public:
	//파일 이름에 확장자 포함!
	BOOL Init( TCHAR* filename );
	void Release();

	inline void SetTable( TCHAR* tblName ) { CopyMemory( m_nowTable, tblName, 20 ); }

	//======================================
	// DB함수들
	//======================================
//private:
	//--------------------------------------
	// DB를 다시 만들게 되면 써야 하는 함수
	//--------------------------------------
	BOOL CreateServerTbl();
	BOOL CreateTableListTbl();

public:
	// log table을 하나 만든다
	BOOL CreateLogTable( TCHAR* tableName );

	// server ID를 추가한다.
	BOOL InsertServerID( TCHAR* srvName, int ID );

	// table의 list를 모두 콤보박스로 넣어 준다
	void SettingTableListCombobox( CComboBox* combo );
	// server를 모두 콤보박스로 넣어 준다
	void SettingServerListCombobox( CComboBox* combo );

	// 테이블을 제거한다
	BOOL DisabledTable( TCHAR* tableName );


	//Log를 추가한다.
	void InsertLog( int serverId, int loglv, TCHAR* log );
};

#define GetDB LogDataBase::GetInstance()


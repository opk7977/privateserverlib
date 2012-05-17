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
	//���� �̸��� Ȯ���� ����!
	BOOL Init( TCHAR* filename );
	void Release();

	inline void SetTable( TCHAR* tblName ) { CopyMemory( m_nowTable, tblName, 20 ); }

	//======================================
	// DB�Լ���
	//======================================
//private:
	//--------------------------------------
	// DB�� �ٽ� ����� �Ǹ� ��� �ϴ� �Լ�
	//--------------------------------------
	BOOL CreateServerTbl();
	BOOL CreateTableListTbl();

public:
	// log table�� �ϳ� �����
	BOOL CreateLogTable( TCHAR* tableName );

	// server ID�� �߰��Ѵ�.
	BOOL InsertServerID( TCHAR* srvName, int ID );

	// table�� list�� ��� �޺��ڽ��� �־� �ش�
	void SettingTableListCombobox( CComboBox* combo );
	// server�� ��� �޺��ڽ��� �־� �ش�
	void SettingServerListCombobox( CComboBox* combo );

	// ���̺��� �����Ѵ�
	BOOL DisabledTable( TCHAR* tableName );


	//Log�� �߰��Ѵ�.
	void InsertLog( int serverId, int loglv, TCHAR* log );
};

#define GetDB LogDataBase::GetInstance()


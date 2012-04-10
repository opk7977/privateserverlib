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
	//ODBC������
	SQLHENV					m_hEnv;		//ȯ���ڵ�
	SQLHDBC					m_hDbc;		//�����ڵ�

	SQLHSTMT				m_hStmt;	//����ڵ�

	//mdb�ּҸ� ��� �ִ� ���ڿ� ����
	SQLWCHAR				ConnectDirMdb[255];
	
	//DB���� Binding�� ���ڿ� ����
	TCHAR					m_tstrBindCol[MAX_COL][255];

	SQLINTEGER				m_iRowCount;                //�˻���(?) ���ڵ� ����
	SQLSMALLINT				m_iColCount;				//�÷� ����
	SQLWCHAR				m_tchColName[MAX_COL][50];	//�÷��� �̸���(��Ű��)�� ��´�.
	SQLINTEGER				m_iCol[MAX_COL];			//�÷��� ����, ����

public:
	SQuery(void);
	virtual ~SQuery(void);
	
	//mdb ����( mdb�� ���� ���� )
	BOOL ConnectMdb( TCHAR* conStr );
	
	//���� ����
	void DisConnect();

	//�������� �޾� ����
	BOOL Exec( TCHAR* szSQL );

	//���� ��������
	SQLRETURN Fetch();
	//��� ���� ���� ����
	//���� ������ ���� ������ ������ ������ ��� �Ѵ�.
	void Clear();

	//������ �÷� �б�
	int GetInt( int nCol );					//���������� ���
	int GetInt( TCHAR* sCol );

	//���ڿ��� �÷� �б�
	void GetStr( int nCol, TCHAR* buf );	//���������� ���
	void GetStr( TCHAR* sCol, TCHAR* buf );

	//�÷��� �̸����� �˻��ؼ� �ش� index�� ã�� �ش�.
	int FindCollasName( TCHAR* colName );
};

#endif
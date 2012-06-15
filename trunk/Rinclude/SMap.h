#ifndef __SESSION_MAP__
#define __SESSION_MAP__

#include "SServerObj.h"

class SMap : public SServerObj
{
private:
	ATL::CAtlMap<int, SServerObj*>		m_Map;
	int									m_iSize;

public:
	SMap(void);
	virtual ~SMap(void);

	void Release();
	//////////////////////////////////////////////////////////////////////////
	//�������� ���Ǵ� �Լ�
	//srv���� �߰��� ����� ���� ���� ���� �Լ�����..
	//���� �Է�
	void PushIndex( int index, SServerObj* obj );
	//�ش� index�� ����
	void PopIndex( int index );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//srv���� �ַ� ����� �Լ��� ����
	//�ش� index�� SServerObj�� �Ѱ� �ش�
	SServerObj* PeekObj( int index );
	//////////////////////////////////////////////////////////////////////////

	int GetSize();

};


#endif


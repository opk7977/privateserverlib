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
	//생성때만 사용되는 함수
	//srv에서 중간에 사용할 일은 거의 없는 함수지만..
	//정보 입력
	void PushIndex( int index, SServerObj* obj );
	//해당 index를 제거
	void PopIndex( int index );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//srv에서 주로 사용할 함수는 이쪽
	//해당 index의 SServerObj를 넘겨 준다
	SServerObj* PeekObj( int index );
	//////////////////////////////////////////////////////////////////////////

	int GetSize();

};


#endif


#ifndef __INDEX_QEUEUE__
#define __INDEX_QEUEUE__


#include "SServerobj.h"


class SIndexQeueue : public SServerObj
{
	//기본 사이즈는 100으로 설정된다
	//수정하려면 SServerStd에서 수정할 수 있음
	std::vector<int>		m_vecIndex;

	int						m_iReadPos;
	int						m_iWritePos;

	int						m_iMaxCount;

public:
	SIndexQeueue(void);
	virtual ~SIndexQeueue(void);

	//초기화 함수
	//startindex부터 시작
	BOOL	IsCreate( int size, int startindex );
	void	Release();

	//vector가 꽉 차여 있으면 true를 return
	BOOL	IsPull();
	//vector가 비어 있으면 true를 return
	BOOL	IsEmpty();
	//사용할 수 있는 index값을 빼온다.
	//사용 가능한 index값이 없다면 -1을 return
	int		GetIndex();
	//현재의 index값을 return
	int		PeekIndex() const { return m_iReadPos; }
	//넘겨 받은 index를 다시 사용할 수 있도록 뒤에 넣어 준다.
	BOOL	PutIndex( int _index );
};


#endif


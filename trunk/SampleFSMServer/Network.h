#ifndef __NETWORK__
#define __NETWORK__

#include "SAcceptor.h"

class Network
{
private:
	SAcceptor	m_accepter;

public:
	Network(void);
	~Network(void);

	//서버 셋팅 accepter와 iocp등 초기화 하고
	BOOL	Init( int count );

	//서버 연결
	BOOL	SettingSrv( WORD port );
	
	//서버 종료
	void	Release();

	//////////////////////////////////////////////////////////////////////////

};

#endif


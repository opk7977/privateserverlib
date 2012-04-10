#include "Sample.h"

#include "SThreadMgr.h"
#include "SegIOCP.h"
#include "SSessionMgr.h"
#include "SampleSession.h"

#define Srvport		8900

Sample::Sample(void)
{
}

Sample::~Sample(void)
{
}

BOOL Sample::Init()
{
	//소켓 초기화
	if( !m_acceptor.Init() )
		return FALSE;

	//세션 공간 10확보
	GetSessionMgr.Create( SRUNTIME_CLASS(SampleSession), 2 );

	//IOCP 생성
	if( !GetIOCP.Init() )
		return FALSE;

	return TRUE;
}

BOOL Sample::Setting()
{
	if( !m_acceptor.SetAcceptor( Srvport ) )
		return FALSE;

	return TRUE;
}

void Sample::Run()
{
	GetThreadMgr.IsEndAllThread();
}

void Sample::Release()
{
	m_acceptor.Release();
}

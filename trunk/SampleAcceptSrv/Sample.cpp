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
	//���� �ʱ�ȭ
	if( !m_acceptor.Init() )
		return FALSE;

	//���� ���� 10Ȯ��
	GetSessionMgr.Create( SRUNTIME_CLASS(SampleSession), 2 );

	//IOCP ����
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

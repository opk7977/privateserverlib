#include "SampleSession.h"

//static ������ �����ϱ� ���� ������ ��� �Ѵ�.
SIMPLEMENT_DYNAMIC(SampleSession)
SIMPLEMENT_DYNCREATE(SampleSession)

SampleSession::SampleSession(void)
{
}

SampleSession::~SampleSession(void)
{
}

void SampleSession::OnCreate()
{
	SSession::OnCreate();
}
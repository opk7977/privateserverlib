#include "SampleSession.h"

//static 선언을 정의하기 위해 선언해 줘야 한다.
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
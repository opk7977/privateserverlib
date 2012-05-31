#include "SObject.h"

//함수 포인터를 이용하여 간접적으로 함수를 호출.
SObject* SRTClass::CreateObject() { return (*m_pfunCreateObject)(); }
// = SIMPLEMENT_DYNCREATE(SObject)

//구조체에 정보를 채워 넣는다.
SRTClass SObject::classSObject = { "SObject", sizeof(SObject), NULL };
//원래는 이런식으로 사용하는 것이지만 지금은 기본 셋팅(?)상태.
//아무것도 없는 녀석을 기준으로 만들어 놓은 거기때문에 함수포인터를 NULL을 넘긴다.
//SIMPLEMENT_DYNAMIC(SObject)
// = {
// SRTClass SObject::classSObject = { "SObject", sizeof(SObject), SObject::CreateObject() };
//	 }

//==============================================================

SObject::SObject()
{
}

SObject::~SObject()
{
}

//==============================================================

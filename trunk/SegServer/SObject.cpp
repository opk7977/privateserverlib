#include "SObject.h"

//�Լ� �����͸� �̿��Ͽ� ���������� �Լ��� ȣ��.
SObject* SRTClass::CreateObject() { return (*m_pfunCreateObject)(); }
// = SIMPLEMENT_DYNCREATE(SObject)

//����ü�� ������ ä�� �ִ´�.
SRTClass SObject::classSObject = { "SObject", sizeof(SObject), NULL };
//������ �̷������� ����ϴ� �������� ������ �⺻ ����(?)����.
//�ƹ��͵� ���� �༮�� �������� ����� ���� �ű⶧���� �Լ������͸� NULL�� �ѱ��.
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

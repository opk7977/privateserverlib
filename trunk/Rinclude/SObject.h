#ifndef __BASE_OBJECT__
#define __BASE_OBJECT__

#include <stdio.h>
#include "SServerStd.h"


class SObject;

//Run-Time Identification( Ȥ�� Information )
struct SRTClass
{
	//class name
	char			m_chClassName[30];
	//size of class
	int				m_iClassSize;
	//Func Pointer( CreateObject() );
	SObject*		(*m_pfunCreateObject)();
	//CreateObject Func
	SObject*		CreateObject();
};

//==============================================================================
// ���� ���� 1. #_stringȭ ������
//------------------------------------------------------------------------------
// #define STRING(s) #s			-> s�� ���� string ���� ����� �ش�
//
// ex) cout<<STRING(zzz);		-> zzz�� ���ڿ��� �ƴѵ� ���� ���� zzz�� ���
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// ���� ���� 2. ##_��ū ���� ������
//------------------------------------------------------------------------------
// #define TOKEN(i,j) i##j		-> i�� j�� ������ �ش�.
//
// ex) cout<<TOKEN(kgca,game);	-> kgcagame �� ���
//
// ex) #define STRING(i) lpsz##i
//	   cout<<STRING(Classname);	-> lpszClassname�� ���
//==============================================================================


//=====================================================================================================================
// ��ũ�� �Լ� ����( ���Ǹ� ���� ����ϴ� �κ��̴�, �̸��� ����! )
//---------------------------------------------------------------------------------------------------------------------
//�Ѱ� ���� Ŭ������ �ּҸ� ��ȯ(?)
#define SRUNTIME_CLASS(class_name) (&class_name::class##class_name)
//---------------------------------------------------------------------------------------------------------------------
//static class( �Է¹��� class�� �̸����� staticŬ������ ������ش� )
#define SDECLARE_DYNAMIC(class_name) static SRTClass class##class_name;
//static func( �Է¹��� class�� CreateObject �Լ��� static���� ���� )
#define SDECLARE_DYNCREATE(class_name) static SObject* CreateObject();
//---------------------------------------------------------------------------------------------------------------------
//���� Ŭ���� ������ũ��( Ŭ���� �̸�, Ŭ���� ũ��, Ŭ���������Լ� ������ )
#define SIMPLEMENT_DYNAMIC(class_name) SRTClass class_name::class##class_name\
	= { (#class_name), sizeof(class_name), class_name::CreateObject };
//CreateObject�Լ� ������ũ��
#define SIMPLEMENT_DYNCREATE(class_name) SObject* class_name::CreateObject() { return new class_name; }
//=====================================================================================================================


class SObject
{
	//�ٸ� Object class���� ��ӹ��� �ֻ��� Ŭ����
	//���� ������ �ƴ����� �����ڰ� protected�̹Ƿ� ��ü�� ������ ���� ����.
protected:
	SObject() {}

public:
	virtual SRTClass* GetRTClass() const { return NULL; }
	//staticŬ������ ����� �ִ� ��ũ��
	SDECLARE_DYNAMIC(SObject)		// = static SRTClass classSObject;

	//virtual �Լ��� �ݵ�� �ϳ��� �־�� �Ѵ�
	virtual ~SObject(void) {}
};

#endif


#ifndef __BASE_OBJECT__
#define __BASE_OBJECT__

#include <stdio.h>
#include "SServerStd.h"


class SObject;

//Run-Time Identification( 혹은 Information )
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
// 기초 지식 1. #_string화 연산자
//------------------------------------------------------------------------------
// #define STRING(s) #s			-> s가 뭐던 string 으로 만들어 준다
//
// ex) cout<<STRING(zzz);		-> zzz가 문자열도 아닌데 오류 없이 zzz를 출력
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 기초 지식 2. ##_토큰 연결 연산자
//------------------------------------------------------------------------------
// #define TOKEN(i,j) i##j		-> i와 j를 연결해 준다.
//
// ex) cout<<TOKEN(kgca,game);	-> kgcagame 이 출력
//
// ex) #define STRING(i) lpsz##i
//	   cout<<STRING(Classname);	-> lpszClassname이 출력
//==============================================================================


//=====================================================================================================================
// 매크로 함수 정의( 편의를 위해 사용하는 부분이다, 겁먹지 말자! )
//---------------------------------------------------------------------------------------------------------------------
//넘겨 받은 클래스의 주소를 반환(?)
#define SRUNTIME_CLASS(class_name) (&class_name::class##class_name)
//---------------------------------------------------------------------------------------------------------------------
//static class( 입력받은 class의 이름으로 static클래스를 만들어준다 )
#define SDECLARE_DYNAMIC(class_name) static SRTClass class##class_name;
//static func( 입력받은 class에 CreateObject 함수를 static으로 만듬 )
#define SDECLARE_DYNCREATE(class_name) static SObject* CreateObject();
//---------------------------------------------------------------------------------------------------------------------
//동적 클래스 생성매크로( 클래스 이름, 클래스 크기, 클래스생성함수 포인터 )
#define SIMPLEMENT_DYNAMIC(class_name) SRTClass class_name::class##class_name\
	= { (#class_name), sizeof(class_name), class_name::CreateObject };
//CreateObject함수 생성매크로
#define SIMPLEMENT_DYNCREATE(class_name) SObject* class_name::CreateObject() { return new class_name; }
//=====================================================================================================================


class SObject
{
	//다른 Object class들이 상속받을 최상위 클래스
	//순수 가상은 아니지만 생성자가 protected이므로 객체로 생성할 수는 없다.
protected:
	SObject() {}

public:
	virtual SRTClass* GetRTClass() const { return NULL; }
	//static클래스를 나들어 주는 매크로
	SDECLARE_DYNAMIC(SObject)		// = static SRTClass classSObject;

	//virtual 함수가 반드시 하나는 있어야 한다
	virtual ~SObject(void) {}
};

#endif


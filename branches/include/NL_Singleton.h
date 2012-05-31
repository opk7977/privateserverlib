#pragma once

#include "NL_Base.h"

//=====================================
//  Singleton Object
//=====================================
class NSingletonObject
{
public:
	virtual void ReleaseInstance() = 0;
};

void AddSingletonObject(NSingletonObject* obj);
void ReleaseSingletonObject();

//=========================================================================
//	Name   : NSingleton<>
//	Content: 단일체 클래스 템플릿
//=========================================================================
template < typename _Ty >
class NSingleton : public NSingletonObject
{
	//==================================
	//	Ctor & Dtor
	//==================================
	NSingleton(const NSingleton& rhs){}
	NSingleton& operator = (const NSingleton& rhs){}

	static _Ty* __instance;

public:
	NSingleton()
	{
		AddSingletonObject(this);
	}
	virtual ~NSingleton(){}

	//==================================
	//	External Function
	//==================================
	NL_INLINE 
	static _Ty* GetInstance()
	{
		if( !__instance )
			__instance = new _Ty();
		return __instance;
	}

	template < typename _Derived >
	NL_INLINE
	static _Derived* GetInstance_T()
	{
		if( !__instance )
			__instance = new _Derived();
		return (_Derived*)__instance;
	}

	NL_INLINE
	/*static*/ void ReleaseInstance()
	{
		SAFE_DELETE_AFTER_RELEASE( __instance )
	}
};

template < typename _Ty >
_Ty* NSingleton< _Ty >::__instance = NULL; 

#define DECL_FRIEND( _Type )	friend class _Type;
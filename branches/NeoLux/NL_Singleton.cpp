#include "NL_Singleton.h"

//=====================================
//  NSingleton List
//=====================================
class __SINGLETON_LIST
{
	static std::list<NSingletonObject*>*	__g__singletonList;
public:
	static std::list<NSingletonObject*>& Get() 
	{
		if( !__g__singletonList )
			__g__singletonList = new std::list<NSingletonObject*>;
		return *__g__singletonList;	
	}
	static void Release()
	{
		SAFE_DELETE( __g__singletonList )
	}
};
std::list<NSingletonObject*>*	__SINGLETON_LIST::__g__singletonList = 0;

void AddSingletonObject(NSingletonObject* obj)
{
	__SINGLETON_LIST::Get().push_back(obj);
}

void ReleaseSingletonObject()
{
	for(std::list<NSingletonObject*>::iterator itor = __SINGLETON_LIST::Get().begin();
		itor != __SINGLETON_LIST::Get().end(); ++itor)
		(*itor)->ReleaseInstance();
	__SINGLETON_LIST::Get().clear();
	__SINGLETON_LIST::Release();
}
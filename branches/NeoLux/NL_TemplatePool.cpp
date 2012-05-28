#include "NL_TemplatePool.h"

//=====================================
//  Pool Object
//=====================================
class __POOL_OBJECT
{
	static std::list<NPoolObject*>* __g__poolList;
public:
	static std::list<NPoolObject*>& Get() 
	{
		if( !__g__poolList )
			__g__poolList = new std::list<NPoolObject*>;
		return *__g__poolList;	
	}
	static void Release()
	{
		SAFE_DELETE( __g__poolList )
	}
};
std::list<NPoolObject*>*	__POOL_OBJECT::__g__poolList = 0;

void AddPoolObject(NPoolObject* obj)
{
	__POOL_OBJECT::Get().push_back(obj);
}

bool ResetPoolObject()
{
	bool ret = true;
	for(std::list<NPoolObject*>::iterator itor = __POOL_OBJECT::Get().begin();
		itor != __POOL_OBJECT::Get().end(); ++itor)
	{
		if( !(*itor)->Reset() )
			ret = false;
	}

	return ret;
}

void RemovePoolObject()
{
	for(std::list<NPoolObject*>::iterator itor = __POOL_OBJECT::Get().begin();
		itor != __POOL_OBJECT::Get().end(); ++itor)
	{
		(*itor)->Remove();
	}
}

void ReleasePoolObject()
{
	__POOL_OBJECT::Get().clear();
	__POOL_OBJECT::Release();
}
#pragma once

//===================================
//	Method Macro
//===================================
#define NL_TEMPLATE		template <typename _Type>
#define NL_INLINE		__forceinline
#define NL_METHOD		NL_TEMPLATE NL_INLINE

//=========================================================================
//	Safe Method for General Purpose.
//=========================================================================
#define SAFE_ZERO( _ptr, _Type )\
{\
	if(_ptr)\
		memset( _ptr, 0, sizeof(_Type) );\
}

#define SAFE_RESET( _ptr )\
{\
	if( _ptr )\
		_ptr->Reset();\
}

#define SAFE_NEW( _ptr, _Type )\
{\
	if(!_ptr)\
		_ptr = new _Type();\
}

#define SAFE_DELETE( _ptr )\
{\
	if(_ptr)\
	{\
		delete _ptr;\
		_ptr = NULL;\
	}\
}

#define SAFE_RELEASE( _ptr )\
{\
	if(_ptr)\
	{\
		_ptr->Release();\
		_ptr = NULL;\
	}\
}

#define SAFE_DELETE_AFTER_RELEASE( _ptr )\
{\
	if( _ptr )\
	{\
		_ptr->Release();\
		delete _ptr;\
		_ptr = NULL;\
	}\
}

#define SAFE_ARRAY_NEW( _ptr, _Type, _cnt )\
{\
	if( !_ptr )\
		_ptr = new _Type[ _cnt ];\
}

#define SAFE_ARRAY_DELETE( _ptr )\
{\
	if( _ptr )\
	{\
		delete [] _ptr;\
		_ptr = NULL;\
	}\
}

#define SAFE_ARRAY_DELETE_FOR( _ptr, _cnt )\
{\
	if( _ptr )\
	{\
		for( int i = 0; i < _cnt; ++i )\
		{\
			delete _ptr[i];\
			_ptr[i] = NULL;\
		}\
	}\
}

#define SAFE_ARRAY_RELEASE( _ptr, _cnt )\
{\
	if( _ptr )\
	{\
		for( int i = 0; i < _cnt; ++i )\
		{\
			_ptr[i]->Release();\
			_ptr[i] = NULL;\
		}\
	}\
}

#define SAFE_ARRAY_DELETE_AFTER_RELEASE( _pptr, _cnt )\
{\
	if( _ptr )\
	{\
		for( int i = 0; i < _cnt; ++i )\
		{\
			_ptr[i]->Release();\
			delete _ptr[i];\
			_ptr[i] = NULL;\
		}\
	}\
}

//=========================================================================
//	Safe method for STL vector<>
//=========================================================================
NL_METHOD
void SAFE_STL_VECTOR_WIPE(_Type& _vec)
{
	if(!_vec.empty())
	{
		_Type().swap(_vec);
	}
}

NL_METHOD
void SAFE_STL_VECTOR_DELETE(_Type& _vec)
{
	if(!_vec.empty())
	{
		for(size_t i = 0; i < _vec.size(); ++i)
			delete _vec[i];
		_Type().swap(_vec);
	}
}

NL_METHOD
void SAFE_STL_VECTOR_RELEASE(_Type& _vec)
{
	if( !_vec.empty() )
	{
		for(size_t i = 0; i < _vec.size(); ++i)
			_vec[i]->Release();
		_Type().swap(_vec);
	}
}

NL_METHOD
void SAFE_STL_VECTOR_DELETE_AFTER_RELEASE(_Type& _vec)
{
	if( !_vec.empty() )
	{
		for(size_t i = 0; i < _vec.size(); ++i)
		{
			_vec[i]->Release();
			delete _vec[i];
		}
		_Type().swap(_vec);
	}
}

//=========================================================================
//	Safe method for STL list<> & set<>
//=========================================================================
NL_METHOD
void SAFE_STL_LIST_DELETE(_Type& _container)
{
	if(!_container.empty())
	{
		for(_Type::iterator itor = _container.begin();
			itor != _container.end(); ++itor)
			delete (*itor);
		_container.clear();
	}
}

NL_METHOD
void SAFE_STL_LIST_RELEASE(_Type& _container)
{
	if(!_container.empty())
	{
		for(_Type::iterator itor = _container.begin();
			itor != _container.end(); ++itor)
			(*itor)->Release();
		_container.clear();
	}
}

NL_METHOD
void SAFE_STL_LIST_DELETE_AFTER_RELEASE(_Type& _container)
{
	if(!_container.empty())
	{
		for(_Type::iterator itor = _container.begin();
			itor != _container.end(); ++itor)
		{
			(*itor)->Release();
			delete (*itor);
		}
		_container.clear();
	}
}

//=========================================================================
//	Safe method for STL map<>
//=========================================================================
NL_METHOD
void SAFE_STL_MAP_DELETE(_Type& _container)
{
	if(!_container.empty())
	{
		for(_Type::iterator itor = _container.begin();
			itor != _container.end(); ++itor)
			delete (*itor).second;
		_container.clear();
	}
}

NL_METHOD
void SAFE_STL_MAP_RELEASE(_Type& _container)
{
	if(!_container.empty())
	{
		for(_Type::iterator itor = _container.begin();
			itor != _container.end(); ++itor)
			(*itor).second->Release();
		_container.clear();
	}
}

NL_METHOD
void SAFE_STL_MAP_DELETE_AFTER_RELEASE(_Type& _container)
{
	if(!_container.empty())
	{
		for(_Type::iterator itor = _container.begin();
			itor != _container.end(); ++itor)
		{
			(*itor).second->Release();
			delete (*itor).second;
		}
		_container.clear();
	}
}

//=========================================================================
//	Safe method for Atl CAtlMap<>
//=========================================================================
NL_METHOD
void SAFE_ATL_MAP_DELETE(_Type& _container)
{
	if( !_container.IsEmpty() )
	{
		_Type::CPair* _curPair = NULL;
		POSITION pos = _container.GetStartPosition();
		while(pos)
		{
			_curPair = _container.GetNext(pos);
			delete _curPair->m_value;
		}
		_container.RemoveAll();
	}
}

NL_METHOD
void SAFE_ATL_MAP_RELEASE(_Type& _container)
{
	if( !_container.IsEmpty() )
	{
		_Type::CPair* _curPair = NULL;
		POSITION pos = _container.GetStartPosition();
		while(pos)
		{
			_curPair = _container.GetNext(pos);
			_curPair->m_value->Release();
		}
		_container.RemoveAll();
	}
}

NL_METHOD
void SAFE_ATL_MAP_DELETE_AFTER_RELEASE(_Type& _container)
{
	if( !_container.IsEmpty() )
	{
		_Type::CPair* _curPair = NULL;
		POSITION pos = _container.GetStartPosition();
		while(pos)
		{
			_curPair = _container.GetNext(pos);
			_curPair->m_value->Release();
			delete _curPair->m_value;
		}
		_container.RemoveAll();
	}
}
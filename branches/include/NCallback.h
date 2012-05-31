#pragma once

//==================================
//	함수 테이블
//==================================
struct FuncTable
{
	void (*Do)(void*, void*);
};

template < typename _FuncBinder >
struct TableBase
{
	__forceinline 
	static FuncTable* GetTable()
	{
		static FuncTable funcTable = { &_FuncBinder::Do };
		return &funcTable;
	}
};

//==================================
//	전역 함수 바인더
//==================================
template < typename _FuncType >
struct Global_Func_Binder : public TableBase< Global_Func_Binder< _FuncType > >
{
	union FnConverter	{	void* voidPtr; _FuncType funcPtr;	};

	__forceinline
	static void Init(void** ppDest, _FuncType pSrc)
	{
		FnConverter fcv; 
		fcv.funcPtr = pSrc;
		*ppDest = fcv.voidPtr;
	}

	__forceinline
	static void Do(void*, void* funcPtr)
	{
		static FnConverter fcv;
		fcv.voidPtr = funcPtr;
		(*fcv.funcPtr)();
	}
};

//==================================
//	멤버 함수 바인더
//==================================
template < typename _ClsType, typename _ClsFuncType >
struct Member_Func_Binder : public TableBase< Member_Func_Binder< _ClsType, _ClsFuncType > >
{
	union FnConverter	{	void* voidPtr; _ClsFuncType funcPtr;	};

	__forceinline
	static void Init(void** ppDest, _ClsFuncType pSrc)
	{
		FnConverter fcv; 
		fcv.funcPtr = pSrc;
		*ppDest = fcv.voidPtr;
	}

	__forceinline
	static void Do(void* objPtr, void* funcPtr)
	{
		if( !objPtr )
			return;

		static FnConverter fcv;
		fcv.voidPtr = funcPtr;
		(static_cast<_ClsType*>(objPtr)->*fcv.funcPtr)();
	}
};

//=========================================================================
//	Name   : NCallBack
//	Content: 콜백 함수 클래스
//=========================================================================
class NCallback
{
	void*		m_objPtr;
	void*		m_funcPtr;
	FuncTable*	m_tablePtr;

public:
	NCallback(void):
		m_objPtr(0),
		m_funcPtr(0),
		m_tablePtr(0)
	{
		
	}

	NCallback( const NCallback& rhs ):
		m_objPtr( rhs.m_objPtr ),
		m_funcPtr( rhs.m_funcPtr ),
		m_tablePtr( rhs.m_tablePtr )
	{

	}

	NCallback( void (*globalFuncPtr)() )
	{
		this->Assign( globalFuncPtr );
	}

	template < typename _ClsType, typename _CallableClsType >
	NCallback( void (_CallableClsType::*funcPtr)(), _ClsType* objPtr )
	{
		this->Bind( funcPtr, objPtr );
	}

	template < typename _ClsType, typename _CallableClsType >
	NCallback( void (_CallableClsType::*funcPtr)() const, _ClsType* objPtr )
	{
		this->Bind( funcPtr, objPtr );
	}

	NCallback& operator = (const NCallback& rhs)
	{
		if( this != &rhs )
		{
			m_objPtr = rhs.m_objPtr;
			m_funcPtr = rhs.m_funcPtr;
			m_tablePtr = rhs.m_tablePtr;
		}
		return *this;
	}

	NCallback& operator = ( void (*globalFuncPtr)() )
	{
		this->Assign( globalFuncPtr );
		return *this;
	}

	__forceinline
	void operator () () const
	{
		m_tablePtr->Do( m_objPtr, m_funcPtr );
	}

	template < typename _ClsType, typename _CallableClsType >
	void Bind( void (_CallableClsType::*funcPtr)(), _ClsType* objPtr )
	{
		m_objPtr = const_cast<void *>( static_cast<const void *>( const_cast<const _ClsType *>( objPtr ) ) );
		Member_Func_Binder< _ClsType, void (_CallableClsType::*)() >::Init( &m_funcPtr, funcPtr );
		m_tablePtr = Member_Func_Binder< _ClsType, void (_CallableClsType::*)() >::GetTable();
	}

	template < typename _ClsType, typename _CallableClsType >
	void Bind( void (_CallableClsType::*funcPtr)() const, _ClsType* objPtr )
	{
		m_objPtr = const_cast<void *>( static_cast<const void *>( const_cast<const _ClsType *>( objPtr ) ) );
		Member_Func_Binder< _ClsType, void (_CallableClsType::*)() >::Init( &m_funcPtr, funcPtr );
		m_tablePtr = Member_Func_Binder< _ClsType, void (_CallableClsType::*)() >::GetTable();
	}

	void Assign( void (*funcPtr)() )
	{
		m_objPtr = 0;
		Global_Func_Binder< void (*)() >::Init( &m_funcPtr, funcPtr );
		m_tablePtr = Global_Func_Binder< void (*)() >::GetTable();
	}

	bool IsEmpty() const
	{
		return !m_tablePtr;
	}

	void Reset()
	{
		m_objPtr = 0;
		m_funcPtr = 0;
		m_tablePtr = 0;
	}
};
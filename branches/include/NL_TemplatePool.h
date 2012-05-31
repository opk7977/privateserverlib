#pragma once

#include "NL_Base.h"
#include "NL_Singleton.h"
#include "NL_Resource.h"

//=====================================
//  NPoolObject
//=====================================
class NPoolObject
{
public:
	virtual bool Reset() = 0;
	virtual void Remove() = 0;
};

void AddPoolObject(NPoolObject* obj);
bool ResetPoolObject();
void RemovePoolObject();
void ReleasePoolObject();

//=========================================================================
//	Name   : NTemplatePool
//	Content: 템플릿 매니져 클래스
//=========================================================================
template <typename _Type>
class NTemplatePool : public NSingleton< NTemplatePool< _Type > >, public NPoolObject
{
	DECL_FRIEND( NSingleton< NTemplatePool< _Type > > )
	
	//==================================
	//	Protected Data
	//==================================
protected:
	CString										m_dir;
	std::map< int, _Type* >						m_templateMap;
	typename std::map< int, _Type* >::iterator	m_itor;
	int											m_curIndex;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NTemplatePool(): m_curIndex(0)
	{
		AddPoolObject( this );
	}
	virtual ~NTemplatePool(){}

	//==================================
	//	External Function
	//==================================
	bool	IsLoaded() const
	{
		return !m_templateMap.empty();
	}
	void	SetDir(const TCHAR* dir)
	{
		m_dir = dir;
	}

	const TCHAR* GetDir() const
	{
		return m_dir.GetString();
	}

	int		AddByPath(const TCHAR* fileName)
	{
		int chk = FindByPath( fileName );
		if( chk >= 0 )
			return chk;

		_Type* pNew = new _Type();
		pNew->Init();
		if( !pNew->Load(fileName) )
		{
			delete pNew;
			return -1;
		}

		m_templateMap[ m_curIndex ] = pNew;

		return m_curIndex++;
	}

	template < typename _Derived >
	int		AddByPath_T(const TCHAR* fileName)
	{
		int chk = FindByPath( fileName );
		if( chk >= 0 )
			return chk;

		_Type* pNew = new _Derived();
		pNew->Init();
		if( !pNew->Load(fileName) )
		{
			delete pNew;
			return -1;
		}

		m_templateMap[ m_curIndex ] = pNew;

		return m_curIndex++;
	}

	int		AddByName(const TCHAR* fileName)
	{
		CString fullPath =  m_dir;
		fullPath		 += fileName;
		
		return AddByPath( fullPath );
	}

	template < typename _Derived >
	int		AddByName_T(const TCHAR* fileName)
	{
		CString fullPath =  m_dir;
		fullPath		 += fileName;

		return AddByPath_T< _Derived >( fullPath );
	}

	int		AddByPtr(_Type* ptr)
	{
		int chk = FindByPtr( ptr );
		if( chk >= 0 )
			return chk;

		m_templateMap[ m_curIndex ] = ptr;

		return m_curIndex++;
	}
	int		AddByPtrWithIndex(_Type* ptr, int index)
	{
		int chk = FindByPtr( ptr );
		if( chk >= 0 )
			return chk;

		m_templateMap[ index ] = ptr;
		return index;
	}

	int		FindByPath(const TCHAR* fileName)
	{
		if( m_templateMap.empty() )
			return -1;

		int chk = -1;
		
		for(m_itor = m_templateMap.begin();
			m_itor != m_templateMap.end();
			++m_itor)
		{
			if( !_tcsicmp( fileName, m_itor->second->GetFilePath() ) )
			{
				chk = m_itor->first;
				break;
			}
		}

		return chk;
	}

	int		FindByName(const TCHAR* fileName)
	{
		if( m_templateMap.empty() )
			return -1;

		CString fullPath =  m_dir;
		fullPath		 += fileName;

		return FindByPath(fullPath);
	}

	int		FindByPtr(_Type* ptr)
	{
		if( m_templateMap.empty() )
			return -1;

		int chk = -1;

		for(m_itor = m_templateMap.begin();
			m_itor != m_templateMap.end();
			++m_itor)
		{
			if( ptr == m_itor->second )
			{
				chk = m_itor->first;
				break;
			}
		}

		return chk;
	}

	void	RemoveByIndex(int index)
	{
		m_itor = m_templateMap.find(index);
		if(m_itor == m_templateMap.end())
			return;

		if( m_itor->second->SubRef() == 0 )
		{
			SAFE_DELETE_AFTER_RELEASE( m_itor->second );
			m_templateMap.erase( m_itor );
		}
	}

	void	RemoveByPtr(_Type* ptr)
	{
		for(m_itor = m_templateMap.begin();
			m_itor != m_templateMap.end();
			++m_itor)
		{
			if( ptr == m_itor->second )
			{
				if( m_itor->second->SubRef() == 0 )
				{
					SAFE_DELETE_AFTER_RELEASE( m_itor->second );
					m_templateMap.erase( m_itor );
				}
				break;
			}
		}
	}

	void	RemoveByPath( const TCHAR* filePath )
	{
		for(m_itor = m_templateMap.begin();
			m_itor != m_templateMap.end();
			++m_itor)
		{
			if( !_tcsicmp( filePath, m_itor->second->GetFilePath() ) )
			{
				if( m_itor->second->SubRef() == 0 )
				{
					SAFE_DELETE_AFTER_RELEASE( m_itor->second );
					m_templateMap.erase( m_itor );
				}
				break;
			}
		}
	}

	void	RemoveByName( const TCHAR* fileName )
	{
		if( m_templateMap.empty() )
			return;

		CString fullPath =  m_dir;
		fullPath		 += fileName;

		RemoveByPath( fullPath.GetString() );
	}

	NL_INLINE
	_Type*	GetByIndex(int index)
	{
		m_itor = m_templateMap.find( index );
		if( m_itor == m_templateMap.end() )
			return NULL;

		m_itor->second->AddRef();
		return m_itor->second;
	}

	NL_INLINE
	_Type*	GetByName(const TCHAR* fileName)
	{
		int chk = AddByName(fileName);
		if( chk < 0 )
			return NULL;

		m_templateMap[ chk ]->AddRef();
		return m_templateMap[ chk ];
	}

	template < typename _Derived >
	NL_INLINE
	_Derived*	GetByName_T(const TCHAR* fileName)
	{
		int chk = AddByName_T< _Derived >(fileName);
		if( chk < 0 )
			return NULL;

		m_templateMap[ chk ]->AddRef();
		return (_Derived*)m_templateMap[ chk ];
	}

	NL_INLINE
	_Type*  GetByPath(const TCHAR* fileName)
	{
		int chk = AddByPath( fileName );
		if( chk < 0 )
			return NULL;

		m_templateMap[ chk ]->AddRef();
		return m_templateMap[ chk ];
	}

	template < typename _Derived >
	NL_INLINE
	_Derived*  GetByPath_T(const TCHAR* fileName)
	{
		int chk = AddByPath_T< _Derived >( fileName );
		if( chk < 0 )
			return NULL;

		m_templateMap[ chk ]->AddRef();
		return (_Derived*)m_templateMap[ chk ];
	}

	void	Remove()
	{
		for(m_itor = m_templateMap.begin();
			m_itor != m_templateMap.end();
			++m_itor)
		{
			m_itor->second->Remove();
		}
	}

	bool	Reset()
	{
		bool success = true;
		for(m_itor = m_templateMap.begin();
			m_itor != m_templateMap.end();
			++m_itor)
		{
			success = m_itor->second->Reset();
		}

		return success;
	}

	virtual void	Release()
	{
		SAFE_STL_MAP_DELETE_AFTER_RELEASE( m_templateMap );
		m_curIndex = 0;
	}
};
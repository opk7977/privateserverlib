#pragma once

#include "NL_Base.h"

//=========================================================================
//	Name   : NResource
//	Content: 리소스 추상 클래스
//=========================================================================
class NResource
{
protected:
	//==================================
	//	Protected Data
	//==================================
	int			m_refCount;
	CString		m_filePath;
	CString		m_fileName;
	CString		m_fileExt;
	CString		m_fileFullName;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NResource():m_refCount(0) {}
	virtual ~NResource(){}

	//==================================
	//	External Function
	//==================================
	virtual void Init()							= 0;
	virtual bool Load(const TCHAR* fileName)	= 0;
	virtual void Remove()						= 0;
	virtual bool Reset()						= 0;
	virtual void Release()						= 0;
	virtual bool IsLoaded() const				= 0;

	NL_INLINE
	const TCHAR* GetFilePath() const
	{
		return m_filePath.GetString();
	}
	NL_INLINE
	const TCHAR* GetFileName() const
	{
		return m_fileName.GetString();
	}
	NL_INLINE
	const TCHAR* GetFileExt() const
	{
		return m_fileExt.GetString();
	}
	NL_INLINE
	const TCHAR* GetFileFullName() const
	{
		return m_fileFullName.GetString();
	}
	NL_INLINE
	int			 GetFileFullNameSize() const
	{
		return m_fileFullName.GetLength();
	}

	void SplitPath(const TCHAR* fileName)
	{
		m_filePath = fileName;

		TCHAR buf[ MAX_PATH ];
		TCHAR name[ MAX_PATH ];
		TCHAR ext[ MAX_PATH ];

		_tsplitpath_s( fileName,
			buf, MAX_PATH, 
			buf, MAX_PATH, 
			name, MAX_PATH,
			ext, MAX_PATH );

		m_fileName = name;
		m_fileExt  = ext;
		m_fileFullName = m_fileName + m_fileExt;
	}

	NL_INLINE
	void AddRef()
	{
		++m_refCount;
	}

	NL_INLINE
	int SubRef()
	{
		return m_refCount = max( m_refCount - 1, 0 );
	}
};
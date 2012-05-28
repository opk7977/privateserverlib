#pragma once

#include "NL_Font.h"
#include "NL_Singleton.h"

//=========================================================================
//	Name   : NFontPool
//	Content: 폰트 자원 관리 클래스
//=========================================================================
class ND3d;
class NFontPool : public NSingleton< NFontPool >
{
	DECL_FRIEND( NSingleton< NFontPool > )

	//==================================
	//	Private Data
	//==================================
	typedef std::map< CString, NFont* >	FONTMAP;
	typedef FONTMAP::iterator			FONTITOR;
	
	FONTMAP*	m_fontMap;
	ND3d*		m_pDevice;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NFontPool();
	~NFontPool();

	//==================================
	//	External Function
	//==================================
	void	Init();
	NFont*	GetFont( const TCHAR* fontName, int Height, bool bBold );
	bool	RemoveFont( const TCHAR* fontName, int Height, bool bBold );
	void	Reset();
	void	Remove();
	void	Release();
	
private:
	//==================================
	//	Internal Function
	//==================================
	NFont* _CreateNFont( const CString& index, const TCHAR* fontName, int Height, bool bBold );
};

#define	Mgr_NFont	NFontPool::GetInstance()
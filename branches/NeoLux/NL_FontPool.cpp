#include "NL_FontPool.h"
#include "NL_Device.h"

//=========================================================================
//	Name   : NFontPool
//	Content: 폰트 자원 관리 클래스
//=========================================================================
NFontPool::NFontPool(): m_pDevice(0), m_fontMap(0)
{

}

NFontPool::~NFontPool()
{	

}

void NFontPool::Init()
{
	m_pDevice = I_ND3d;
	SAFE_NEW( m_fontMap, FONTMAP )
}

NFont* NFontPool::GetFont( const TCHAR* fontName, int Height, bool bBold )
{
	NFont* result = NULL;
	static CString tmp;
	tmp.Format( _T("%s%d%d"), fontName, Height, bBold ? FW_BOLD : FW_NORMAL );

	FONTITOR target = m_fontMap->find( tmp );
	if(target == m_fontMap->end())	
		result = _CreateNFont( tmp, fontName, Height, bBold );
	else
		result = target->second;
	
	return result;
}

bool NFontPool::RemoveFont( const TCHAR* fontName, int Height, bool bBold )
{
	static CString tmp;
	tmp.Format( _T("%s%d%d"), fontName, Height, bBold ? FW_BOLD : FW_NORMAL );

	FONTITOR target = m_fontMap->find( tmp );
	if( target == m_fontMap->end() )
		return false;

	SAFE_DELETE_AFTER_RELEASE( target->second );
	m_fontMap->erase( target );

	return true;
}

void NFontPool::Reset()
{
	for(FONTITOR itor = m_fontMap->begin();
		itor != m_fontMap->end(); ++itor)
		itor->second->Reset();
}

void NFontPool::Remove()
{
	for(FONTITOR itor = m_fontMap->begin();
		itor != m_fontMap->end(); ++itor)
		itor->second->Release();
}

void NFontPool::Release()
{
	SAFE_STL_MAP_DELETE_AFTER_RELEASE( *m_fontMap );
	SAFE_DELETE( m_fontMap );
}

NFont* NFontPool::_CreateNFont( const CString& index, const TCHAR* fontName, int Height, bool bBold )
{
	NFont* fontNew = new NFont();
	if( !fontNew->Init(m_pDevice, Height, bBold ? FW_BOLD : FW_NORMAL, fontName) )
	{
		delete fontNew;
		return NULL;
	}

	m_fontMap->insert( std::make_pair( index, fontNew ) );
	FONTITOR target = m_fontMap->find( index );
	
	return target->second;
}
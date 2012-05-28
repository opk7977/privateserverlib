#include "NL_Texture.h"
#include "NL_Device.h"

void NTexture::Init()
{
	// 
}

bool NTexture::Load(const TCHAR* filePath)
{
	LPDIRECT3DTEXTURE9 tmpTex;
	if( FAILED( I_ND3d->CreateTextureFromFile(filePath, &tmpTex ) ) )
		return false;

	Release();
	m_texture = tmpTex;

	SplitPath( filePath );

	return true;
}

void NTexture::Remove()
{
	SAFE_RELEASE( m_texture )
}

bool NTexture::Reset()
{
	CString tmpStr = m_filePath;
	return Load( tmpStr.GetString() );
}

void NTexture::Release()
{
	SAFE_RELEASE( m_texture )
}
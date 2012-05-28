#include "NL_Shader.h"
#include "NL_Device.h"

void NShader::Init()
{
	
}

bool NShader::Load(const TCHAR* fileName)
{
	Release();

	LPD3DXEFFECT tmpEff;
	if( FAILED( I_ND3d->CreateEffectFromFile(fileName, &tmpEff) ) )
		return false;
	
	m_pEffect = tmpEff;

	SplitPath( fileName );

	return true;
}

void NShader::Remove()
{
	SAFE_RELEASE( m_pEffect )
}

bool NShader::Reset()
{
	CString tmpStr = m_filePath;
	return Load( tmpStr.GetString() );
}

void NShader::Release()
{
	SAFE_RELEASE( m_pEffect )
}
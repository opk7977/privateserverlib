#include "NL_Sky.h"
#include "NL_Device.h"
#include "NL_Camera.h"
#include "NL_Shader.h"

NSky::NSky(void) :
	m_pSphere(0),
	m_envMap(0),
	m_pSkyFx(0)
{

}

NSky::~NSky(void)
{
	Release();
}

bool NSky::Init( float skyRadius /*= 10000.0f*/ )
{
	Release();

	m_pDevice = I_ND3d;
	m_pCamera = I_NCamera;

	m_skyRadius = skyRadius;
	
	// ����Ʈ ����
	m_pShader = Mgr_NShader->GetByName( _T("SkyObj.fx") );
	Reset();
	
	return true;
}

void NSky::Render()
{
	static NMATRIX __WVP;

	__WVP.Identity();
	__WVP.Translation(m_pCamera->GetCurCamera()->GetPosition());
	__WVP *= m_pCamera->GetCurCamera()->GetViewProj();
	
	// ��� ����
	m_pSkyFx->SetMatrix(m_hWVP, __WVP);

	m_pSkyFx->Begin(NULL, 0);
	m_pSkyFx->BeginPass(0);

	// �׸���
	m_pSphere->DrawSubset(0);
	
	m_pSkyFx->EndPass();
	m_pSkyFx->End();
}

void NSky::Remove()
{
	SAFE_RELEASE( m_pSphere )
	SAFE_RELEASE( m_envMap  )
}

bool NSky::Reset()
{
	// ��豸 �޽� ����
	if( FAILED( D3DXCreateSphere(m_pDevice->GetDevice(), m_skyRadius, 30, 30, &m_pSphere, NULL) ) )
	{
		MessageBox(NULL, _T("Failed to Create Sky mesh."), _T("NSky Error"), MB_OK);
		return false;
	}

	// ȯ��� �ؽ��� �ε�
	if( !m_envMapName.IsEmpty() )
	{
		if( FAILED( D3DXCreateCubeTextureFromFile(m_pDevice->GetDevice(), m_envMapName.GetString(), &m_envMap ) ) )
		{
			MessageBox(NULL, _T("Failed to Load Sky envmap."), _T("NSky Error"), MB_OK);
			return false;
		}
	}

	// �ڵ� ���ϱ�
	m_pSkyFx	= m_pShader->GetShader();
	m_hTech		= m_pSkyFx->GetTechniqueByName("SkyTech");
	m_hWVP		= m_pSkyFx->GetParameterByName(NULL, "matWVP");
	m_hEnvMap	= m_pSkyFx->GetParameterByName(NULL, "envMap");

	// ��ũ�а� �ؽ��� �̸� ����
	m_pSkyFx->SetTechnique(m_hTech);
	m_pSkyFx->SetTexture(m_hEnvMap, m_envMap);

	return true;
}

void NSky::Release()
{
	SAFE_RELEASE( m_pSphere )
	SAFE_RELEASE( m_envMap )
}

bool NSky::SetCubeTexture(const TCHAR* name)
{
	LPDIRECT3DCUBETEXTURE9 tmpTex;

	// ȯ��� �ؽ��� �ε�
	if( FAILED( D3DXCreateCubeTextureFromFile(m_pDevice->GetDevice(), name, &tmpTex ) ) )
	{
		MessageBox(NULL, _T("Failed to Load Sky envmap."), _T("NSky Error"), MB_OK);
		return false;
	}
	SAFE_RELEASE( m_envMap )
	m_envMapName = name;
	m_envMap = tmpTex;
	m_pSkyFx->SetTexture( m_hEnvMap, m_envMap );

	return true;
}
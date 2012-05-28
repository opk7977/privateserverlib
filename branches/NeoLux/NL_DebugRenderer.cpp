#include "NL_DebugRenderer.h"
#include "NL_Device.h"
#include "NL_Camera.h"
#include "NL_Shader.h"
#include "NL_Debug.h"
#include "NL_BoundingBox.h"

NDebugRenderer::NDebugRenderer(): 
	m_pEff(0),
	m_pLine(0)
{
	
}

NDebugRenderer::~NDebugRenderer()
{
	
}

void NDebugRenderer::Init()
{
	m_pDev = I_ND3d;
	m_pCam = I_NCamera;
}

bool NDebugRenderer::Load(const TCHAR* fileName)
{
	m_pShader = Mgr_NShader->GetByName( fileName );
	if(!m_pShader)
		return false;

	SplitPath(fileName);

	Remove();
	Reset();
	return true;
}

void NDebugRenderer::PreRender()
{
	m_pDev->FVF( P3C_VERTEX::FVF );
	m_pEff->SetMatrix( m_hVP, m_pCam->GetCurCamera()->GetViewProj() );

	m_pEff->SetTechnique( m_hTech );
	m_pEff->Begin( NULL, 0 );
	m_pEff->BeginPass( 0 );
}

void NDebugRenderer::PostRender()
{
	m_pEff->EndPass();
	m_pEff->End();
}

void NDebugRenderer::RenderLine(NDebug* pDebug)
{
	PreRender();

	m_pDev->DrawUP( pDebug->m_lineBuffer, sizeof( P3C_VERTEX ), 1, D3DPT_LINELIST );

	PostRender();
}

void NDebugRenderer::RenderLineMulti(NDebug* pDebug)
{
	PreRender();

	m_pDev->DrawUP( pDebug->m_lineMultiBuffer, sizeof( P3C_VERTEX ), 
		pDebug->m_lineMultiCount, D3DPT_LINESTRIP );

	PostRender();
}

void NDebugRenderer::RenderLineList(NDebug* pDebug)
{
	PreRender();

	m_pDev->DrawUP( pDebug->m_lineMultiBuffer, sizeof( P3C_VERTEX ), 
		pDebug->m_lineMultiCount, D3DPT_LINELIST );

	PostRender();
}

void NDebugRenderer::RenderTriangle(NDebug* pDebug)
{
	PreRender();

	m_pDev->DrawUP( pDebug->m_triangleBuffer, sizeof( P3C_VERTEX ), 1 );

	PostRender();
}

void NDebugRenderer::RenderAxis(NDebug* pDebug)
{
	PreRender();

	m_pDev->DrawUP( pDebug->m_3AxisBuffer, sizeof( P3C_VERTEX ), 3, D3DPT_LINELIST );

	PostRender();
}

void NDebugRenderer::RenderBox(NDebug* pDebug)
{
	PreRender();

	m_pDev->DrawIndexedUP( 
		pDebug->m_boxIndex,
		pDebug->m_boxBuffer,
		sizeof( P3C_VERTEX ),
		8, 12, 0, D3DFMT_INDEX32,
		D3DPT_LINELIST );

	PostRender();
}

void NDebugRenderer::RenderSphere(NDebug* pDebug)
{
	PreRender();

	for(int i = 0; i < 3; ++i)
		m_pDev->DrawUP( pDebug->m_circleBuffer[i],
			sizeof( P3C_VERTEX ), CIRCLE_DETAIL, D3DPT_LINESTRIP );

	PostRender();
}

void NDebugRenderer::RenderBoundingBoxWorld(NDebug* pDebug, NMATRIX& matWorld)
{
	m_pDev->FVF( P3C_VERTEX::FVF );
	m_pEff->SetMatrix( m_hW, matWorld );
	m_pEff->SetMatrix( m_hVP, m_pCam->GetCurCamera()->GetViewProj() );
	
	m_pEff->SetTechnique( m_hTech );
	m_pEff->Begin( NULL, 0 );
	m_pEff->BeginPass( 1 );

	m_pDev->DrawIndexedUP( 
		pDebug->m_boxIndex,
		pDebug->m_boxBuffer,
		sizeof( P3C_VERTEX ),
		8, 12, 0, D3DFMT_INDEX32,
		D3DPT_LINELIST );

	PostRender();
}

void NDebugRenderer::RenderCapsule( NDebug* pDebug )
{
	PreRender();

	for(int i = 0; i < 2; ++i)
		m_pDev->DrawUP( pDebug->m_circleBuffer[i],
			sizeof( P3C_VERTEX ), CIRCLE_DETAIL, D3DPT_LINESTRIP );

	for(int i = 0; i < 4; ++i)
		m_pDev->DrawUP( pDebug->m_hemiSphereBuffer[i],
			sizeof( P3C_VERTEX ), CIRCLE_DETAIL/2, D3DPT_LINESTRIP );

	PostRender();
}

void NDebugRenderer::RenderCrossLine(NDebug* pDebug)
{
	m_pLine->SetWidth( 3.0f );
	m_pLine->Begin();

	m_pLine->Draw( pDebug->m_crossVert[0], 2, 0xffffff00 );
	m_pLine->Draw( pDebug->m_crossVert[2], 2, 0xffff44ff );

	m_pLine->End();
}

void NDebugRenderer::Remove()
{
	SAFE_RELEASE( m_pLine )
}

bool NDebugRenderer::Reset()
{
	m_pDev->CreateLine( &m_pLine );
	
	m_pEff  = m_pShader->GetShader();
	m_hTech = m_pEff->GetTechniqueByName("DebugRender");
	m_hW	= m_pEff->GetParameterByName(0, "matW");
	m_hVP	= m_pEff->GetParameterByName(0, "matVP");

	return true;
}

void NDebugRenderer::Release()
{
	SAFE_RELEASE( m_pLine )
}
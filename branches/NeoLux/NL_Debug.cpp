#include "NL_Debug.h"
#include "NL_Device.h"
#include "NL_Camera.h"
#include "NL_Time.h"
#include "NL_Input.h"
#include "NL_FontPool.h"
#include "NL_Renderer.h"
#include "NL_DebugRenderer.h"
#include "NL_Sphere.h"
#include "NL_BoundingBox.h"
#include "NL_OBB.h"
#include "NL_Grid.h"

void NDebug::Init(NFont* pFont)
{
	m_bSolid    = true;
	m_pFont		= pFont;
	m_pTime		= I_NTime;
	m_pInput	= I_NInput;
	m_pDev		= I_ND3d;
	m_pGrid		= I_NGrid;
	
	TCHAR* fileName = _T("debug.fx");
	m_pRenderer = Mgr_NRenderer->GetByName_T< NDebugRenderer >( fileName );
	
	m_bRenderFPS = true;
	m_bRenderBasisAxis	= false;
	m_bRenderCursorLine = false;
	m_bRenderGrid = false;

	m_3AxisBuffer[0].Set(0.0f, 0.0f, 0.0f, 0xffff0000);
	m_3AxisBuffer[1].Set(1000.0f, 0.0f, 0.0f, 0xffff0000);
	m_3AxisBuffer[2].Set(0.0f, 0.0f, 0.0f, 0xff00ff00);
	m_3AxisBuffer[3].Set(0.0f, 1000.0f, 0.0f, 0xff00ff00);
	m_3AxisBuffer[4].Set(0.0f, 0.0f, 0.0f, 0xff0000ff);
	m_3AxisBuffer[5].Set(0.0f, 0.0f, 1000.0f, 0xff0000ff);

	float invMax = 1.0f / CIRCLE_DETAIL;
	for(int i = 0; i <= CIRCLE_DETAIL; ++i)
	{
		m_sinTable[i] = sinf( NL_PI_X_2 * i * invMax );
		m_cosTable[i] = cosf( NL_PI_X_2 * i * invMax );
		
		for(int j = 0; j < 3; ++j)
			m_circleBuffer[j][i].vc = 0xff44ff44;
	}

	// 인덱스
	// 앞면
	m_boxIndex[0][0] = 0;
	m_boxIndex[0][1] = 1;
	m_boxIndex[0][2] = 1;
	m_boxIndex[0][3] = 3;
	m_boxIndex[0][4] = 3;
	m_boxIndex[0][5] = 2;
	m_boxIndex[0][6] = 2;
	m_boxIndex[0][7] = 0;
	
	// 옆면
	m_boxIndex[1][0] = 0;
	m_boxIndex[1][1] = 4;
	m_boxIndex[1][2] = 6;
	m_boxIndex[1][3] = 1;
	m_boxIndex[1][4] = 3;
	m_boxIndex[1][5] = 7;
	m_boxIndex[1][6] = 5;
	m_boxIndex[1][7] = 2;
	
	// 뒷면
	m_boxIndex[2][0] = 6;
	m_boxIndex[2][1] = 4;
	m_boxIndex[2][2] = 4;
	m_boxIndex[2][3] = 5;
	m_boxIndex[2][4] = 5;
	m_boxIndex[2][5] = 7;
	m_boxIndex[2][6] = 7;
	m_boxIndex[2][7] = 6;

	for(int i = 0; i < 8; ++i)
		m_boxBuffer[i].vc   = 0xffffff44;
}

void NDebug::Frame()
{
	if( m_pInput->IsKeydownFirst(VK_F1) )
	{
		m_bRenderFPS = !m_bRenderFPS;
	}
	else if( m_pInput->IsKeydownFirst(VK_F2) )
	{
		m_pDev->RenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		m_bSolid = true;
	}
	else if( m_pInput->IsKeydownFirst(VK_F3) )
	{
		m_pDev->RenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		m_bSolid = false;
	}
	else if( m_pInput->IsKeydownFirst(VK_F4) )
	{
		m_pDev->RenderState( D3DRS_FILLMODE, D3DFILL_POINT );
		m_bSolid = false;
	}
	else if( m_pInput->IsKeydownFirst(VK_F5) )
	{
		m_bRenderBasisAxis	= !m_bRenderBasisAxis;
	}
	else if( m_pInput->IsKeydownFirst(VK_F6) )
	{
		m_bRenderCursorLine	= !m_bRenderCursorLine;
	}
	else if( m_pInput->IsKeydownFirst(VK_F8) )
	{
		m_bRenderGrid = !m_bRenderGrid;
	}
}

void NDebug::Render()
{
	if( m_bRenderFPS )
	{
		DrawFPS();
	}

	if( m_bRenderBasisAxis )
	{
		DrawAxis();
	}

	if( m_bRenderCursorLine )
	{
		DrawCrossLine( m_pInput->GetCursorPos() );
	}

	if( m_bRenderGrid )
	{
		m_pGrid->Render();
	}
}

void NDebug::DrawFPS()
{
	m_pFont->Draw( 10, 10, NL_LIGHTORANGE, _T("FPS: %d"), m_pTime->GetFPS() );
	m_pFont->Draw( 10, 24, NL_LIGHTORANGE, _T("SPF: %f"), m_pTime->GetElapsedTime() );
}

void NDebug::DrawLine(	const NVECTOR3& p0,
						const NVECTOR3& p1,
						D3DCOLOR		vc )
{
	m_lineBuffer[0].vp = p0;
	m_lineBuffer[1].vp = p1;
	m_lineBuffer[0].vc = vc;
	m_lineBuffer[1].vc = vc;

	m_pRenderer->RenderLine( this );
}

void NDebug::DrawLineMulti(P3C_VERTEX*	pData, 
						   int			count)
{
	m_lineMultiBuffer = pData;
	m_lineMultiCount = count;

	m_pRenderer->RenderLineMulti( this );
}

void NDebug::DrawLineList(	P3C_VERTEX* pData, 
							int			count)
{
	m_lineMultiBuffer = pData;
	m_lineMultiCount = count;

	m_pRenderer->RenderLineList( this );
}

void NDebug::DrawTriangle(const NVECTOR3& p0, const NVECTOR3& p1, const NVECTOR3& p2, D3DCOLOR		vc /*=0xffff00ff*/)
{
	m_triangleBuffer[0].Set( p0, vc );
	m_triangleBuffer[1].Set( p1, vc );
	m_triangleBuffer[2].Set( p2, vc );

	m_pRenderer->RenderTriangle( this );
}

void NDebug::DrawAxis()
{
	m_pRenderer->RenderAxis( this );
}

void NDebug::DrawBox(	const NVECTOR3& vMin,
						const NVECTOR3& vMax,
						D3DCOLOR		vc )
{
	// 앞 왼쪽 위
	m_boxBuffer[0].vp.x = vMin.x;
	m_boxBuffer[0].vp.y = vMin.y;
	m_boxBuffer[0].vp.z = vMin.z;
	m_boxBuffer[0].vc   = vc;

	// 앞 오른쪽 위
	m_boxBuffer[1].vp.x = vMin.x;
	m_boxBuffer[1].vp.y = vMin.y;
	m_boxBuffer[1].vp.z = vMin.z;
	m_boxBuffer[1].vc   = vc;
	
	// 앞 왼쪽 아래
	m_boxBuffer[2].vp.x = vMin.x;
	m_boxBuffer[2].vp.y = vMin.y;
	m_boxBuffer[2].vp.z = vMin.z;
	m_boxBuffer[2].vc   = vc;
	
	// 앞 오른쪽 아래
	m_boxBuffer[3].vp.x = vMin.x;
	m_boxBuffer[3].vp.y = vMin.y;
	m_boxBuffer[3].vp.z = vMin.z;
	m_boxBuffer[3].vc   = vc;
	
	// 뒤 왼쪽 위
	m_boxBuffer[4].vp.x = vMin.x;
	m_boxBuffer[4].vp.y = vMin.y;
	m_boxBuffer[4].vp.z = vMin.z;
	m_boxBuffer[4].vc   = vc;
	
	// 뒤 왼쪽 아래
	m_boxBuffer[5].vp.x = vMin.x;
	m_boxBuffer[5].vp.y = vMin.y;
	m_boxBuffer[5].vp.z = vMin.z;
	m_boxBuffer[5].vc   = vc;
	
	// 뒤 오른쪽 위
	m_boxBuffer[6].vp.x = vMin.x;
	m_boxBuffer[6].vp.y = vMin.y;
	m_boxBuffer[6].vp.z = vMin.z;
	m_boxBuffer[6].vc   = vc;
	
	// 뒤 오른쪽 아래
	m_boxBuffer[7].vp.x = vMin.x;
	m_boxBuffer[7].vp.y = vMin.y;
	m_boxBuffer[7].vp.z = vMin.z;
	m_boxBuffer[7].vc   = vc;

	m_pRenderer->RenderBox( this );
}

void NDebug::DrawSphere(NSphere& sphere, NMATRIX& matWorld)
{
	static NVECTOR3 worldCenter;
	static float cosMulRadius[CIRCLE_DETAIL+1];
	static float sinMulRadius[CIRCLE_DETAIL+1];

	matWorld.MultiplyCoord( worldCenter, sphere.centerPoint );

	// Precalculate radian
	float radius = sphere.radius * matWorld.GetRow3(0).Length();
	for(int i = 0; i < CIRCLE_DETAIL+1; ++i)
	{
		cosMulRadius[i] = radius * m_cosTable[i];
		sinMulRadius[i] = radius * m_sinTable[i];
	}

	for(int i = 0; i < CIRCLE_DETAIL+1; ++i)
	{
		// X Axis Circle
		m_circleBuffer[0][i].vp.x = worldCenter.x;
		m_circleBuffer[0][i].vp.y = worldCenter.y + sinMulRadius[i];
		m_circleBuffer[0][i].vp.z = worldCenter.z + cosMulRadius[i];

		// Y Axis Circle
		m_circleBuffer[1][i].vp.x = worldCenter.x + cosMulRadius[i];
		m_circleBuffer[1][i].vp.y = worldCenter.y;
		m_circleBuffer[1][i].vp.z = worldCenter.z + sinMulRadius[i]; 
		
		// Z Axis Circle
		m_circleBuffer[2][i].vp.x = worldCenter.x + cosMulRadius[i];
		m_circleBuffer[2][i].vp.y = worldCenter.y + sinMulRadius[i];
		m_circleBuffer[2][i].vp.z = worldCenter.z;
	}

	m_pRenderer->RenderSphere( this );
}

void NDebug::DrawBoundingBox(NBoundingBox& bbox, NMATRIX& matWorld)
{
	// 앞 왼쪽 위
	m_boxBuffer[0].vp.x = bbox.vMin.x;
	m_boxBuffer[0].vp.y = bbox.vMax.y;
	m_boxBuffer[0].vp.z = bbox.vMin.z;
	
	// 앞 오른쪽 위
	m_boxBuffer[1].vp.x = bbox.vMax.x;
	m_boxBuffer[1].vp.y = bbox.vMax.y;
	m_boxBuffer[1].vp.z = bbox.vMin.z;
	
	// 앞 왼쪽 아래
	m_boxBuffer[2].vp.x = bbox.vMin.x;
	m_boxBuffer[2].vp.y = bbox.vMin.y;
	m_boxBuffer[2].vp.z = bbox.vMin.z;
	
	// 앞 오른쪽 아래
	m_boxBuffer[3].vp.x = bbox.vMax.x;
	m_boxBuffer[3].vp.y = bbox.vMin.y;
	m_boxBuffer[3].vp.z = bbox.vMin.z;
	
	// 뒤 왼쪽 위
	m_boxBuffer[4].vp.x = bbox.vMin.x;
	m_boxBuffer[4].vp.y = bbox.vMax.y;
	m_boxBuffer[4].vp.z = bbox.vMax.z;
	
	// 뒤 왼쪽 아래
	m_boxBuffer[5].vp.x = bbox.vMin.x;
	m_boxBuffer[5].vp.y = bbox.vMin.y;
	m_boxBuffer[5].vp.z = bbox.vMax.z;
	
	// 뒤 오른쪽 위
	m_boxBuffer[6].vp.x = bbox.vMax.x;
	m_boxBuffer[6].vp.y = bbox.vMax.y;
	m_boxBuffer[6].vp.z = bbox.vMax.z;
	
	// 뒤 오른쪽 아래
	m_boxBuffer[7].vp.x = bbox.vMax.x;
	m_boxBuffer[7].vp.y = bbox.vMin.y;
	m_boxBuffer[7].vp.z = bbox.vMax.z;

	m_pRenderer->RenderBoundingBoxWorld( this, matWorld );
}

void NDebug::DrawOBB(NOBB& obb)
{
	static NVECTOR3 r, u, f;
	r = obb.m_dir[0] * obb.m_halfDist.x;
	u = obb.m_dir[1] * obb.m_halfDist.y;
	f = obb.m_dir[2] * obb.m_halfDist.z;

	// 앞 왼쪽 위
	m_boxBuffer[0].vp = obb.m_center - r + u - f;
	
	// 앞 오른쪽 위
	m_boxBuffer[1].vp = obb.m_center + r + u - f;

	// 앞 왼쪽 아래
	m_boxBuffer[2].vp = obb.m_center - r - u - f;

	// 앞 오른쪽 아래
	m_boxBuffer[3].vp = obb.m_center + r - u - f;

	// 뒤 왼쪽 위
	m_boxBuffer[4].vp = obb.m_center - r + u + f;

	// 뒤 왼쪽 아래
	m_boxBuffer[5].vp = obb.m_center - r - u + f;

	// 뒤 오른쪽 위
	m_boxBuffer[6].vp = obb.m_center + r + u + f;

	// 뒤 오른쪽 아래
	m_boxBuffer[7].vp = obb.m_center + r - u + f;

	m_pRenderer->RenderBox(this);
}

void NDebug::DrawCapsule( NCapsule& capsule, NMATRIX& matWorld )
{
	static NVECTOR3 worldBottom, worldTop;
	static float cosMulRadius[CIRCLE_DETAIL+1];
	static float sinMulRadius[CIRCLE_DETAIL+1];

	// Precalculate radian
	float scale = matWorld.GetRow3(0).Length();
	float height = capsule.height * scale;
	float radius = capsule.radius * scale;
	matWorld.MultiplyCoord( worldBottom, capsule.bottomPoint );
	worldTop = worldBottom + matWorld.GetRow3(1) * height;
	
	for(int i = 0; i < CIRCLE_DETAIL+1; ++i)
	{
		cosMulRadius[i] = radius * m_cosTable[i];
		sinMulRadius[i] = radius * m_sinTable[i];
	}

	// Bottom Cap
	int hemiSphere = CIRCLE_DETAIL / 2;
	int hi;
	for(hi = 0; hi <= hemiSphere; ++hi)
	{
		// X Axis Circle
		m_hemiSphereBuffer[0][hi].vp =	worldTop + 
										matWorld.GetRow3(1) * sinMulRadius[hi] + 
										matWorld.GetRow3(2) * cosMulRadius[hi];
		
		// Z Axis Circle
		m_hemiSphereBuffer[1][hi].vp =	worldTop + 
										matWorld.GetRow3(1) * sinMulRadius[hi] + 
										matWorld.GetRow3(0) * cosMulRadius[hi];
	}
	// Y Axis Circle
	for(int i = 0; i < CIRCLE_DETAIL+1; ++i)
		m_circleBuffer[0][i].vp =	worldTop + 
									matWorld.GetRow3(0) * cosMulRadius[i] + 
									matWorld.GetRow3(2) * sinMulRadius[i];

	// Top Cap
	hi--;
	for(int i = 0; i <= hemiSphere; ++i, ++hi)
	{
		// X Axis Circle
		m_hemiSphereBuffer[2][i].vp =	worldBottom +
										matWorld.GetRow3(1) * sinMulRadius[hi] + 
										matWorld.GetRow3(2) * cosMulRadius[hi];
		
		// Z Axis Circle
		m_hemiSphereBuffer[3][i].vp =	worldBottom +
										matWorld.GetRow3(1) * sinMulRadius[hi] + 
										matWorld.GetRow3(0) * cosMulRadius[hi];
	}
	// Y Axis Circle
	for(int i = 0; i < CIRCLE_DETAIL+1; ++i)
		m_circleBuffer[1][i].vp =	worldBottom + 
									matWorld.GetRow3(0) * cosMulRadius[i] + 
									matWorld.GetRow3(2) * sinMulRadius[i];

	m_pRenderer->RenderCapsule(this);
}

void NDebug::DrawCrossLine( POINT& pt )
{
	m_crossVert[0].x = 0.0f;
	m_crossVert[0].y = float(pt.y);
	m_crossVert[1].x = float(::GetSystemMetrics(SM_CXSCREEN));
	m_crossVert[1].y = float(pt.y);

	m_crossVert[2].x = float(pt.x);
	m_crossVert[2].y = 0.0f;
	m_crossVert[3].x = float(pt.x);
	m_crossVert[3].y = float(::GetSystemMetrics(SM_CYSCREEN));

	m_pRenderer->RenderCrossLine( this );
}

void NDebug::Release()
{
	
}
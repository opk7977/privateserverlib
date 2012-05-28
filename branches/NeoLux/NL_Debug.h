#pragma once

#include "NL_Base.h"
#include "NL_Singleton.h"
#include "NL_D3DFormat.h"

#define CIRCLE_DETAIL	64

//=========================================================================
//	Name   : NDebug
//	Content: 디버그 클래스 선언
//=========================================================================
class ND3d;
class NTime;
class NInput;
class NFont;
struct NOBB;
class NGrid;
class NDebugRenderer;
class NDebug : public NSingleton< NDebug >
{
	DECL_FRIEND( NSingleton< NDebug > )
	DECL_FRIEND( NDebugRenderer )

	//==================================
	//	Private Data
	//==================================
	ND3d*			m_pDev;
	NDebugRenderer* m_pRenderer;
	int				m_curRender;
	P3C_VERTEX		m_lineBuffer[2];
	P3C_VERTEX		m_triangleBuffer[3];
	P3C_VERTEX		m_3AxisBuffer[6];
	P3C_VERTEX		m_boxBuffer[8];
	P3C_VERTEX		m_circleBuffer[3][(CIRCLE_DETAIL+1)];
	P3C_VERTEX		m_hemiSphereBuffer[4][(CIRCLE_DETAIL/2+1)];
	float			m_sinTable[CIRCLE_DETAIL+1];
	float			m_cosTable[CIRCLE_DETAIL+1];
	P3C_VERTEX*		m_lineMultiBuffer;
	int				m_lineMultiCount;
	DWORD			m_boxIndex[3][8];
	NVECTOR2		m_crossVert[4];

	NFont*			m_pFont;
	NTime*			m_pTime;
	NInput*			m_pInput;
	NGrid*			m_pGrid;
	bool			m_bRenderFPS;
	bool			m_bRenderBasisAxis;
	bool			m_bRenderCursorLine;
	bool			m_bRenderGrid;
	bool			m_bSolid;
	
	//==================================
	//	Ctor & Dtor
	//==================================
	NDebug(){}
public:
	~NDebug(){}

	//==================================
	//	External Function
	//==================================
	void Init(NFont* pFont);
	void Frame();
	void Render();
	void DrawFPS();
	void DrawLine(	const NVECTOR3& p0,
					const NVECTOR3& p1,
					D3DCOLOR		vc = 0xffff00ff );
	void DrawLineMulti( P3C_VERTEX* pData,
						int			count );
	void DrawLineList(	P3C_VERTEX* pData,
						int			count );
	void DrawTriangle(	const NVECTOR3& p0,
						const NVECTOR3& p1,
						const NVECTOR3& p2,
						D3DCOLOR		vc = 0xffff00ff );
	void DrawAxis();
	void DrawBox(	const NVECTOR3& vMin,
					const NVECTOR3& vMax,
					D3DCOLOR		vc = 0xffff00ff );
	void DrawSphere( NSphere& sphere, NMATRIX& matWorld );
	void DrawBoundingBox( NBoundingBox& bbox, NMATRIX& matWorld );
	void DrawOBB( NOBB& obb );
	void DrawCapsule( NCapsule& capsule, NMATRIX& matWorld );
	void DrawCrossLine( POINT& pt );
	void Reset();
	void Release();

	bool IsBasisAxisRendered(void)	const	{	return m_bRenderBasisAxis;		}
	bool IsCursorLineRendered(void)	const	{	return m_bRenderCursorLine;		}
	bool IsFillModeSolid()			const	{	return m_bSolid;				}
	
	void SetBasisAxisRender(bool bRender)	{	m_bRenderBasisAxis = bRender;	}
	void SetCursorLineRender(bool bRender)	{	m_bRenderCursorLine = bRender;	}
	void SetFPSRender(bool bRender)			{	m_bRenderFPS = bRender;			}
	
private:
	//==================================
	//	Internal Function
	//==================================
	void PreRender();
	void PostRender();
};

#define I_NDebug NDebug::GetInstance()
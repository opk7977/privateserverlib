#pragma once

#include "NL_Base.h"
#include "NL_Math.h"

enum OBB_DIR
{
	OBD_X,
	OBD_Y,
	OBD_Z,

	OBD_MAX
};

//=========================================================================
//	Name   : NOBB
//	Content: 방향성있는 바운딩 박스 구조체
//=========================================================================
struct NBoundingBox;
struct NOBB
{
	//==================================
	//	Private Data
	//==================================
	static NDebug*	m_pDebug;
	NVECTOR3 m_halfDist;
	NVECTOR3 m_dir[OBD_MAX];
	NVECTOR3 m_center;
	
	//==================================
	//	Ctor & Dtor
	//==================================
	NOBB();
	~NOBB();

	//==================================
	//	External Function
	//==================================
	void Init(NMATRIX& objMat, NBoundingBox& loacalAABB);
	void SetAxis(NMATRIX& objMat);
	void Render();
	bool CollisionWithOBB(const NOBB& obbB);
	void SlideWithOBB(const NOBB& obbB, NVECTOR3& moveDir, float& moveAmount);
};
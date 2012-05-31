#pragma once

#include "NL_Matrix.h"
#include "NL_Plane.h"

//===================================================================
//	NAME	: NFRUSTUM
//	COMMENT	: 절두체 구조체
//===================================================================
struct NFRUSTUM
{
	//-------------------------------
	//	NFRUSTUM Data
	//-------------------------------
	enum FRUSTUM_PLANE
	{
		TOP_PLANE,
		BOTTOM_PLANE,
		NEAR_PLANE,
		FAR_PLANE,
		LEFT_PLANE,
		RIGHT_PLANE
	};
	
	NVECTOR3	vertice[8];
	NPLANE		plane[6];
	NVECTOR3	cameraPos;
	bool		bBuild;

	//-------------------------------
	//	Constructor
	//-------------------------------
	NFRUSTUM(void);

	//-------------------------------
	//	Function
	//-------------------------------
	NFRUSTUM&	Build(const NVECTOR3& pos, const NMATRIX& _invViewProj);
	NFRUSTUM&	Build(const NVECTOR3& pos, const NMATRIX& view, const NMATRIX& proj);
	bool		IsInFrustum(const NVECTOR3& pt) const;
	bool		IsInSphere(const NVECTOR3& pt, float radius) const;
	bool		IsBuild(void) const;
	NFRUSTUM&	Reset(void);
};

#include "NL_Frustum.inl"
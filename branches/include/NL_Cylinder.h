#pragma once

#include "NL_Base.h"
#include "NL_Math.h"

//=========================================================================
//	Name   : NCylinder
//	Content: 실린더 구조체
//=========================================================================
class NDebug;
struct NCylinder
{
	//==================================
	//	Public Data
	//==================================
	static NDebug* m_pDebug;
	NVECTOR3 upVector;
	NVECTOR3 centerPoint;
	float	halfHeight;
	float	radius;
	
	//==================================
	//	Ctor & Dtor
	//==================================
	NCylinder();
	~NCylinder();

	//==================================
	//	External Function
	//==================================
	void Init();
	void Set(const NVECTOR3& centerPoint, const NVECTOR3& upVector, float halfHeight, float radius);
	bool CollisionWithAACylinder(const NCylinder& cylinder, NVECTOR3* diff);
	bool CollisionWithOBB(const NOBB& obb, NVECTOR3* diff);
};

#pragma once

#include "NL_Base.h"
#include "NL_Math.h"

//=========================================================================
//	Name   : NCapsule
//	Content: Ä¸½¶ ±¸Á¶Ã¼
//=========================================================================
class NDebug;
struct NSphere;
struct NCapsule
{
	//==================================
	//	Public Data
	//==================================
	static NDebug* m_pDebug;
	NVECTOR3	bottomPoint;
	NVECTOR3	upVector;
	float		height;
	float		radius;
	
	//==================================
	//	Ctor & Dtor
	//==================================
	NCapsule();
	~NCapsule();

	//==================================
	//	External Function
	//==================================
	void Init();
	void Render(NMATRIX& matWorld);
	void Set(const NVECTOR3& bottomPoint, const NVECTOR3& upVector, float height, float radius);
	bool CollisionWithSphere(const NSphere& sphere, NVECTOR3* diff = NULL);
	bool CollisionWithAABB(const NVECTOR3& vMin, const NVECTOR3& vMax, NVECTOR3* diff = NULL);
	bool CollisionWithOBB(const NOBB& obb, NVECTOR3* diff = NULL);
	bool CollisionWithAACapsule(const NCapsule& capsule, NVECTOR3* diff = NULL);
	NVECTOR3 GetClosestPoint(const NVECTOR3& pt);
		
private:
	//==================================
	//	Internal Function
	//==================================
};
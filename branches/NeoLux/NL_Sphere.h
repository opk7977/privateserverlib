#pragma once

#include "NL_Base.h"
#include "NL_Math.h"

//=========================================================================
//	Name   : NBoundingBox
//	Content: 바운딩 박스 구조체
//=========================================================================
class NDebug;
struct NOBB;
struct NSphere
{
	//-------------------------------
	//	NSphere Data
	//-------------------------------
	static NDebug*	pDebug;
	NVECTOR3		centerPoint;
	float			radius;

	//-------------------------------
	//	Constructor
	//-------------------------------
	NSphere( void );
	NSphere( const NVECTOR3& center, float r );

	//-------------------------------
	//	Function
	//-------------------------------
	NSphere& Set( const NVECTOR3& center, float r );
	NSphere& Set( const NVECTOR3& vMin, const NVECTOR3& vMax );
	void Render(NMATRIX& matWorld);

	NL_INLINE
	bool IsIntersect(const NSphere& sphere) const
	{
		return	this->centerPoint.DistanceSq( sphere.centerPoint ) <
			(this->radius + sphere.radius) * (this->radius + sphere.radius);
	}
	bool CollisionWithAABB(const NVECTOR3& vMin, const NVECTOR3& vMax, NVECTOR3* diffDir = NULL, float* t = NULL);
	bool CollisionWithOBB(const NOBB& obb, NVECTOR3* diff = NULL);
};
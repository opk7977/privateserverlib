#include "NL_Sphere.h"
#include "NL_Debug.h"
#include "NL_OBB.h"

NDebug* NSphere::pDebug = NULL;

//===============================================================
//  NAME	  : NSphere
//  COMMENTS  : 구 구조체 구현
//===============================================================

//===============================================================
//	Constructor
//===============================================================
NSphere::NSphere( void ): radius(0.f)
{
	if(!pDebug)
		pDebug = I_NDebug;
}
NSphere::NSphere( const NVECTOR3& center, float r ): 
centerPoint(center), radius(r)
{
	if(!pDebug)
		pDebug = I_NDebug;
}

//===============================================================
//	Function
//===============================================================
NSphere& NSphere::Set( const NVECTOR3& center, float r )
{
	centerPoint = center;
	radius = r;
	return *this;
}

NSphere& NSphere::Set( const NVECTOR3& vMin, const NVECTOR3& vMax )
{
	centerPoint = (vMin + vMax) * 0.5f;
	radius      = centerPoint.Distance(vMax);
	return *this;
}

void NSphere::Render(NMATRIX& matWorld)
{
	pDebug->DrawSphere( *this, matWorld );
}

bool NSphere::CollisionWithAABB(const NVECTOR3& vMin, const NVECTOR3& vMax, NVECTOR3* diffDir, float* t)
{
	NVECTOR3 boxIntersect;
	for(int i = 0; i < 3; ++i)
	{
		if( centerPoint.v[i] < vMin.v[i] )
		{
			boxIntersect.v[i] = vMin.v[i];
		}
		else if( centerPoint.v[i] > vMax.v[i] )
		{
			boxIntersect.v[i] = vMax.v[i];
		}
		else
			boxIntersect.v[i] = centerPoint.v[i];
	}

	NVECTOR3 dist = centerPoint - boxIntersect;
	
	if( dist.LengthSq() > radius * radius )
		return false;
	
	if( diffDir )
	{
		float diffFactor = radius - dist.Length();
		dist.Normalize();
		*diffDir = dist;
		*t = diffFactor;
	}

	return true;
}

bool NSphere::CollisionWithOBB(const NOBB& obb, NVECTOR3* diff)
{
	// 6면 검사
	// 대각 벡터 구하기
	NSphere sphereBasedOBB;
	NMATRIX invOBB(
		obb.m_dir[0].x, obb.m_dir[1].x, obb.m_dir[2].x, 0.0f,
		obb.m_dir[0].y, obb.m_dir[1].y, obb.m_dir[2].y, 0.0f,
		obb.m_dir[0].z, obb.m_dir[1].z, obb.m_dir[2].z, 0.0f,
		-(obb.m_dir[0].x*obb.m_center.x + obb.m_dir[0].y*obb.m_center.y + obb.m_dir[0].z*obb.m_center.z),
		-(obb.m_dir[1].x*obb.m_center.x + obb.m_dir[1].y*obb.m_center.y + obb.m_dir[1].z*obb.m_center.z),
		-(obb.m_dir[2].x*obb.m_center.x + obb.m_dir[2].y*obb.m_center.y + obb.m_dir[2].z*obb.m_center.z),
		1.0f );

	NVECTOR3 localDiff;
	float diffFactor;
	sphereBasedOBB.radius = radius;
	invOBB.MultiplyCoord( sphereBasedOBB.centerPoint, centerPoint );
	if( !sphereBasedOBB.CollisionWithAABB( -obb.m_halfDist, obb.m_halfDist, &localDiff, &diffFactor ) )
		return false;

	NVECTOR3 tVector;
	tVector += obb.m_dir[0] * localDiff.x;
	tVector += obb.m_dir[1] * localDiff.y;
	tVector += obb.m_dir[2] * localDiff.z;
	*diff = tVector * diffFactor;

	return true;
}

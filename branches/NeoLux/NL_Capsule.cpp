#include "NL_Capsule.h"
#include "NL_Debug.h"

NDebug* NCapsule::m_pDebug = NULL;

NCapsule::NCapsule(void)
{
}

NCapsule::~NCapsule(void)
{
}

void NCapsule::Init()
{
	m_pDebug = I_NDebug;
}

void NCapsule::Render(NMATRIX& matWorld)
{
	m_pDebug->DrawCapsule( *this, matWorld );
}

void NCapsule::Set(const NVECTOR3& bottomPoint, const NVECTOR3& upVector, float height, float radius)
{
	this->bottomPoint = bottomPoint;
	this->upVector = upVector;
	this->height = height;
	this->radius = radius;
}

bool NCapsule::CollisionWithSphere(const NSphere& sphere, NVECTOR3* diff)
{
	// 가까운 점 구하기
	NVECTOR3 closestPt = GetClosestPoint(sphere.centerPoint);

	// 충돌 여부 판정
	NVECTOR3 tVector = sphere.centerPoint - closestPt;
	float dist = tVector.LengthSq();
	if( dist >	(sphere.radius + this->radius) * (sphere.radius + this->radius ) )
		return false;

	if( diff )
	{
		tVector.Normalize();
		*diff = tVector * ((sphere.radius + this->radius) - sqrtf(dist));
	}

	return true;
}

bool NCapsule::CollisionWithAABB(const NVECTOR3& vMin, const NVECTOR3& vMax, NVECTOR3* diff)
{
	NVECTOR3 vCenter = (vMin + vMax) * 0.5f;
	NSphere closestSphere;
	closestSphere.centerPoint = GetClosestPoint(vCenter);
	closestSphere.radius = this->radius;

	return closestSphere.CollisionWithAABB(vMin, vMax, diff);
}

bool NCapsule::CollisionWithOBB(const NOBB& obb, NVECTOR3* diff)
{
	NSphere closestSphere;
	closestSphere.centerPoint = GetClosestPoint(obb.m_center);
	closestSphere.radius = this->radius;

	return closestSphere.CollisionWithOBB(obb, diff);
}

bool NCapsule::CollisionWithAACapsule(const NCapsule& capsule, NVECTOR3* diff)
{
	float sumRadius = this->radius + capsule.radius;

	// 높이 비교
	NVECTOR3 tP_to_oP = bottomPoint - capsule.bottomPoint;
	if(fabs(tP_to_oP.y) > sumRadius)
		return false;

	// 가로 길이 비교
	float widthSq = fabs(tP_to_oP.x * tP_to_oP.x + tP_to_oP.z * tP_to_oP.z);
	if(widthSq > sumRadius * sumRadius)
		return false;

	if(diff)
	{
		tP_to_oP.y = 0.0f;
		tP_to_oP.Normalize();
		*diff = tP_to_oP * (sumRadius - sqrtf(widthSq));
	}

	return true;
}

NVECTOR3 NCapsule::GetClosestPoint(const NVECTOR3& pt)
{
	// 점과 가장 가까운 직선 상의 점 찾기
	NVECTOR3 bottom_to_SpC = pt - bottomPoint;
	NVECTOR3 bottom_To_up = upVector * height;
	float dotBottom_To_SpC = bottom_to_SpC.Dot(bottom_To_up);
	float ratio = dotBottom_To_SpC / bottom_To_up.LengthSq();
	ratio = max( 0.0f, min( 1.0f, ratio ) );
	NVECTOR3 closestPt = bottomPoint + bottom_To_up * ratio;

	return closestPt;
}
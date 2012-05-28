#include "NL_Cylinder.h"
#include "NL_Debug.h"

NDebug* NCylinder::m_pDebug = NULL;

NCylinder::NCylinder(void)
{
}

NCylinder::~NCylinder(void)
{
}

void NCylinder::Init()
{
	m_pDebug = I_NDebug;
}

void NCylinder::Set(const NVECTOR3& centerPoint, const NVECTOR3& upVector, float halfHeight, float radius)
{
	this->centerPoint = centerPoint;
	this->upVector = upVector;
	this->halfHeight = halfHeight;
	this->radius = radius;
}

bool NCylinder::CollisionWithAACylinder(const NCylinder& cylinder, NVECTOR3* diff)
{
	return true;
}

bool NCylinder::CollisionWithOBB(const NOBB& obb, NVECTOR3* diff)
{
	
	return true;
}
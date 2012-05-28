#include "NL_OBB.h"
#include "NL_BoundingBox.h"
#include "NL_Debug.h"

NDebug* NOBB::m_pDebug = NULL;

NOBB::NOBB(void)
{
	if(!m_pDebug)
		m_pDebug = I_NDebug;
}

NOBB::~NOBB(void)
{
}

void NOBB::Init(NMATRIX& objMat, NBoundingBox& loacalAABB)
{
	SetAxis( objMat );
	
	float scale = objMat.GetRow3(0).Length();
	m_halfDist.x = (loacalAABB.vMax.x - loacalAABB.vMin.x) * 0.5f * scale;
	m_halfDist.y = (loacalAABB.vMax.y - loacalAABB.vMin.y) * 0.5f * scale;
	m_halfDist.z = (loacalAABB.vMax.z - loacalAABB.vMin.z) * 0.5f * scale;

	NVECTOR3 localCenter = (loacalAABB.vMin + loacalAABB.vMax) * 0.5f;
	objMat.MultiplyCoord( m_center, localCenter );
}

void NOBB::SetAxis(NMATRIX& objMat)
{
	for(int i = 0; i < OBD_MAX; ++i)
	{
		m_dir[i] = objMat.GetRow3(i);
		m_dir[i].Normalize();
	}
}

void NOBB::Render()
{
	m_pDebug->DrawOBB(*this);
}

bool NOBB::CollisionWithOBB(const NOBB& obbB)
{
	float tDist[3];
	float sumDist;
	float R[3][3];

	// 두 OBB 중점 간 벡터
	NVECTOR3 tVector = obbB.m_center - this->m_center;

	//////////////////////////////////////////////////////////////////////////

	// 1. obbA의 세 축 검사
	for(int i = 0; i < 3; ++i)
	{
		// 사전 작업
		for(int k = 0; k < 3; ++k)
			R[i][k] = fabs( obbB.m_dir[i].Dot( this->m_dir[k] ) );

		// 축 검사
		tDist[i] = fabs( this->m_dir[i].Dot(tVector) );
		sumDist =	this->m_halfDist.v[i] +
					obbB.m_halfDist.x * R[i][0] +
					obbB.m_halfDist.y * R[i][1] +
					obbB.m_halfDist.z * R[i][2];
		if( tDist[i] > sumDist )
			return false;
	}

	//////////////////////////////////////////////////////////////////////////

	// 2. obbB의 세 축 검사
	float checkDist = 0;
	for(int i = 0; i < 3; ++i)
	{
		checkDist = fabs( obbB.m_dir[i].Dot(tVector) );
		sumDist =	this->m_halfDist.x * R[0][i] +
					this->m_halfDist.y * R[1][i] +
					this->m_halfDist.z * R[2][i] +
					obbB.m_halfDist.v[i];

		if( checkDist > sumDist )
			return false;
	}

	float aDist, bDist;	

	// 3. obbA와 obbB의 접선 축 검사

	//////////////////////////////////////////////////////////////////////////
	//
	// - this->xAxis X obbB.xAxis
	checkDist = fabs(tDist[2] * R[1][0] - tDist[1] * R[2][0]);
	aDist =	this->m_halfDist.y * R[2][0] + this->m_halfDist.z * R[1][0];
	bDist = obbB.m_halfDist.z * R[0][1] + obbB.m_halfDist.y * R[0][2];
	sumDist = aDist + bDist;
	if( checkDist > sumDist )
		return false;

	// - this->xAxis X obbB.yAxis
	checkDist = fabs(tDist[2] * R[1][1] - tDist[1] * R[2][1]);
	aDist =	this->m_halfDist.y * R[2][1] + this->m_halfDist.z * R[1][1];
	bDist = obbB.m_halfDist.x * R[0][2] + obbB.m_halfDist.z * R[0][0];
	sumDist = aDist + bDist;
	if( checkDist > sumDist )
		return false;

	// - this->xAxis X obbB.zAxis
	checkDist = fabs(tDist[2] * R[1][2] - tDist[1] * R[2][2]);
	aDist =	this->m_halfDist.y * R[2][2] + this->m_halfDist.z * R[1][2];
	bDist = obbB.m_halfDist.y * R[0][0] + obbB.m_halfDist.x * R[0][1];
	sumDist = aDist + bDist;
	if( checkDist > sumDist )
		return false;

	//////////////////////////////////////////////////////////////////////////
	//
	// - this->yAxis X obbB.xAxis
	checkDist = fabs(tDist[0] * R[2][0] - tDist[2] * R[0][0]);
	aDist =	this->m_halfDist.z * R[0][0] + this->m_halfDist.x * R[2][0];
	bDist = obbB.m_halfDist.z * R[1][1] + obbB.m_halfDist.y * R[1][2];
	sumDist = aDist + bDist;
	if( checkDist > sumDist )
		return false;

	// - this->yAxis X obbB.yAxis
	checkDist = fabs(tDist[0] * R[2][1] - tDist[2] * R[0][1]);
	aDist =	this->m_halfDist.z * R[0][1] + this->m_halfDist.x * R[2][1];
	bDist = obbB.m_halfDist.x * R[1][2] + obbB.m_halfDist.z * R[1][0];
	sumDist = aDist + bDist;
	if( checkDist > sumDist )
		return false;

	// - this->yAxis X obbB.zAxis
	checkDist = fabs(tDist[0] * R[2][2] - tDist[2] * R[0][2]);
	aDist =	this->m_halfDist.z * R[0][2] + this->m_halfDist.x * R[2][2];
	bDist = obbB.m_halfDist.y * R[1][0] + obbB.m_halfDist.x * R[1][1];
	sumDist = aDist + bDist;
	if( checkDist > sumDist )
		return false;

	//////////////////////////////////////////////////////////////////////////
	//
	// - this->zAxis X obbB.xAxis
	checkDist = fabs(tDist[1] * R[0][0] - tDist[0] * R[1][0]);
	aDist =	this->m_halfDist.x * R[1][0] + this->m_halfDist.y * R[0][0];
	bDist = obbB.m_halfDist.z * R[2][1] + obbB.m_halfDist.y * R[2][2];
	sumDist = aDist + bDist;
	if( checkDist > sumDist )
		return false;

	// - this->zAxis X obbB.yAxis
	checkDist = fabs(tDist[1] * R[0][1] - tDist[0] * R[1][1]);
	aDist =	this->m_halfDist.x * R[1][1] + this->m_halfDist.y * R[0][1];
	bDist = obbB.m_halfDist.x * R[2][2] + obbB.m_halfDist.z * R[2][0];
	sumDist = aDist + bDist;
	if( checkDist > sumDist )
		return false;

	// - this->zAxis X obbB.zAxis
	checkDist = fabs(tDist[1] * R[0][2] - tDist[0] * R[1][2]);
	aDist =	this->m_halfDist.x * R[1][2] + this->m_halfDist.y * R[0][2];
	bDist = obbB.m_halfDist.y * R[2][0] + obbB.m_halfDist.x * R[2][1];
	sumDist = aDist + bDist;
	if( checkDist > sumDist )
		return false;

	return true;
}
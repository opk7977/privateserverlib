#pragma once

#include "NL_Base.h"
#include "NL_Input.h"

//===============================================================
//
//	NAME	: NPicking
//	COMMENT	: 픽킹 클래스
//
//===============================================================
struct SortAscendingFunctor
{
	bool operator ()(const NIntersection& lhs,
					 const NIntersection& rhs)
	{
		return fabs(lhs.distance) < fabs(rhs.distance);
	}
};
typedef std::set< NIntersection, SortAscendingFunctor > SectSet;
typedef SectSet::iterator								SectItor;

class ND3d;
class NCameraPool;
class NInput;
struct NBoundingBox;
struct NOBB;
class NPicking
{
	//-------------------------------
	//	Private Data
	//-------------------------------
	ND3d*			m_pDevice;
	NCameraPool*	m_pCamera;
	NInput*			m_pInput;

	SectSet		m_intersectionTable;
	NRay		m_ray;
	bool		m_bIntersected;
	
public:
	//-------------------------------
	//	Public Function
	//-------------------------------
	void Init();
	void Frame();
	void Frame( const NMATRIX& world );
	void Frame( const NMATRIX& viewWorld, const NMATRIX& proj );
	bool CheckPick(	const NVECTOR3& v0,
					const NVECTOR3& v1,
					const NVECTOR3& v2 );
	bool IntersectTriangle( const NVECTOR3&	v0, 
							const NVECTOR3&	v1, 
							const NVECTOR3&	v2,
							float*			distance,
							NVECTOR3*		intersection );
	bool IntersectSphere( const NSphere& sphere );
	bool IntersectAABB( const NBoundingBox& aabb, float* pDist, NVECTOR3* intersection  );
	bool IntersectOBB( const NOBB& obb, float* pDist, NVECTOR3* intersection );
	//bool IntersectKDOP( const NKD& kd, float* pDist, NVECTOR3* intersection );
	void ResetIntersectionTable(void);
	
public:
	//-------------------------------
	//	Getter
	//-------------------------------
	const NRay&		GetRay(void)			const	{	return m_ray;							}
	const NVECTOR3&	GetRayStartPoint(void)	const	{	return m_ray.startPoint;				}
	const NVECTOR3&	GetRayDirection(void)	const	{	return m_ray.direction;					}
	const NIntersection&
					GetIntersection(void)	const	{	return *m_intersectionTable.begin();	}
	SectSet&		GetIntersectionTable(void)		{	return m_intersectionTable;				}
	bool			IsIntersected(void)		const	{	return m_bIntersected;					}

private:
	//-------------------------------
	//	Temporary Data
	//-------------------------------
	NVECTOR3 m_e1, m_e2, m_n;
	NVECTOR3 m_oriToV0;
	NVECTOR3	m_intersect;
	float	m_det, m_invDet, m_u, m_v;
	float	m_t;
	float	m_dist;

public:
	NPicking(void){}
	~NPicking(void){}
};
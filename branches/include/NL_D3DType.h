#pragma once

#include "NL_Math.h"

//===============================================================
// 픽킹 관련 구조체
//===============================================================
//==============================
//	광선 구조체
//==============================
struct NRay
{
	//-------------------------------
	//	NRay Data
	//-------------------------------
	NVECTOR3 startPoint;
	NVECTOR3 direction;

	//-------------------------------
	//	Constructor
	//-------------------------------
	NRay( void );
	NRay( const NVECTOR3& start, const NVECTOR3& dir );

	//-------------------------------
	//	Function
	//-------------------------------
	NRay& Set( const NVECTOR3& start, const NVECTOR3& dir );
	
};

//==============================
//	교점 구조체
//==============================
struct NIntersection
{
	//-------------------------------
	//	Constructor
	//-------------------------------
	NIntersection( void );
	NIntersection(	const NVECTOR3& _intersection, 
					const NVECTOR3& _v0,
					const NVECTOR3& _v1,
					const NVECTOR3& _v2,
					float _distance );

	//-------------------------------
	//	Function
	//-------------------------------
	NIntersection&	Set(const NVECTOR3& intersection, 
						const NVECTOR3& v0,
						const NVECTOR3& v1,
						const NVECTOR3& v2,
						float			distance );
	
	//-------------------------------
	//	NIntersection Data
	//-------------------------------
	NVECTOR3 intersection;
	NVECTOR3 v0, v1, v2;
	float	 distance;
};

//===============================================================
// 오브젝트 관련 구조체
//===============================================================
//===============================================================
//	씬 구조체
//===============================================================
struct NScene
{
	float	firstFrame;
	float	lastFrame;
	float	frameSpeed;
	float	ticksPerFrame;
	float	ticksPerSec;
	float	invTicksPerFrame;

	float	curTick;
	int		curFrame;
	float	delta;
	float	firstTick;
	float	lastTick;
};

//==============================
//	페이스 구조체
//==============================
// struct NFace
// {
// 	NVECTOR3		normal;
// 	INDEX3			index;
// };
// typedef std::vector< NFace >	NFaceVec;

//==============================
//	애니메이션 트랙 구조체
//==============================
struct NAniBase
{
	int tick;
};
struct NAniPos : public NAniBase
{
	NVECTOR3 position;
};
typedef std::vector< NAniPos > NAniPosVec;

struct NAniRot : public NAniBase
{
	NQUATERNION	qRot;
};
typedef std::vector< NAniRot > NAniRotVec;

struct NAniScale : public NAniBase
{
	NVECTOR3	scale;
	NQUATERNION	qRot;
};
typedef std::vector< NAniScale > NAniScaleVec;

#include "NL_D3DType.inl"
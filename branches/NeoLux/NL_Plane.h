#pragma once

#include "NL_Vector.h"

//===================================================================
//  NAME	  : NPLANE
//  COMMENTS  : 평면 구조체
//===================================================================
struct NPLANE
{
	//-------------------------------
	//	Plane Data
	//-------------------------------
	NVECTOR3	n;
	float		d;

	//-------------------------------
	//	Constructor
	//-------------------------------
	NPLANE(void);
	NPLANE(float _a, float _b, float _c, float _d);
	NPLANE(const NVECTOR3& pt, const NVECTOR3& normal);
	NPLANE(const NVECTOR3& p0, const NVECTOR3& p1, const NVECTOR3& p2);

	//-------------------------------
	//	Create
	//-------------------------------
	NPLANE& Create(const NVECTOR3& p0, const NVECTOR3& p1, const NVECTOR3& p2);
	NPLANE& Create(const NVECTOR3& pt, const NVECTOR3& normal);
	
	//-------------------------------
	//	Normalize
	//-------------------------------
	NPLANE& Normalize(void);

	//-------------------------------
	//	Dot Product
	//-------------------------------
	float DotCoord(const NVECTOR3& pt) const;
	float DotNormal(const NVECTOR3& pt) const;
};

#include "NL_Plane.inl"
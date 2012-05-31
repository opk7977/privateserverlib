#pragma once

//===================================================================
//  NAME	  : NPLANE
//  COMMENTS  : 평면 구조체 구현
//===================================================================

//===============================================================
//	Constructor
//===============================================================
NL_INLINE 
NPLANE::NPLANE(void) : d(0.f) {}

NL_INLINE 
NPLANE::NPLANE(	float a, 
				float b, 
				float c, 
				float d): n(a, b, c), d(d) {}

NL_INLINE 
NPLANE::NPLANE(const NVECTOR3& pt, const NVECTOR3& normal)
{
	Create(pt, normal);
}

NL_INLINE 
NPLANE::NPLANE(	const NVECTOR3& p0, 
				const NVECTOR3& p1, 
				const NVECTOR3& p2)
{
	Create(p0, p1, p2);
}

//===============================================================
//	Function
//===============================================================
//-------------------------------
//	Create(use 3 points)
//-------------------------------
NL_INLINE 
NPLANE& NPLANE::Create(const NVECTOR3& p0, const NVECTOR3& p1, const NVECTOR3& p2)
{
	// 		  edge1
	// 		p0----->p1
	// 		│	   
	// edge2│	   
	// 		↓      
	// 		p2
	// 
	// edge1 cross edge2 = plane normal
	(p1 - p0).Cross( n, (p2 - p0) );
	n.Normalize();

	// ax + by + cz + D = 0
	// D = -(ax + by + cz)
	d = -n.Dot(p0);
	
	return *this;
}
//-------------------------------
//	Create(use 1 point & normal)
//-------------------------------
NL_INLINE 
NPLANE& NPLANE::Create(const NVECTOR3& pt, const NVECTOR3& normal)
{
	// ax + by + cz + D = 0
	// D = -(ax + by + cz)
	n = normal;
	d = -n.Dot(pt);

	return *this;
}
//-------------------------------
//	Normalize
//-------------------------------
NL_INLINE 
NPLANE& NPLANE::Normalize(void)
{
	// Normalize plane normal
	n.Normalize();
	return *this;
}
//-------------------------------
//	DotCoord(include 'd')
//-------------------------------
NL_INLINE float NPLANE::DotCoord(const NVECTOR3& pt) const
{
	//             
	//            [+]
	//      ↑
	//	----------[0]
	//     plane
	//            [-]
	//
	//  (plane normal dot point) + d
	//  => '-': It's back side of plane.
	//  => '0': It's on plane.
	//  => '+': It's front side of plane.
	return n.Dot(pt) + d;
}
//-------------------------------
//	DotCoord(exclude 'd')
//-------------------------------
NL_INLINE float NPLANE::DotNormal(const NVECTOR3& pt) const
{
	// Calculating cosine angle between plane normal and point.
	return n.Dot(pt);
}
#pragma once

//===============================================================
//  NAME	  : NRay
//  COMMENTS  : 광선 구조체 구현
//===============================================================
//===============================================================
//	Constructor
//===============================================================
NL_INLINE
NRay::NRay( void ){}

NL_INLINE 
NRay::NRay( const NVECTOR3& start, const NVECTOR3& dir ): 
	startPoint(start), direction(dir){}

//===============================================================
//	Function
//===============================================================
NL_INLINE 
NRay& NRay::Set( const NVECTOR3& start, const NVECTOR3& dir )
{
	startPoint = start;
	direction = dir;
	return *this;
}

//===============================================================
//  NAME	  : NIntersection
//  COMMENTS  : 광선 구조체 구현
//===============================================================

//===============================================================
//	Constructor
//===============================================================
NL_INLINE 
NIntersection::NIntersection( void ): distance(0.f){}

NL_INLINE 
NIntersection::NIntersection(	const NVECTOR3&	_intersection, 
								const NVECTOR3&	_v0,
								const NVECTOR3&	_v1,
								const NVECTOR3&	_v2,
								float			_distance ):
	intersection(_intersection),
	v0(_v0),
	v1(_v1),
	v2(_v2),
	distance(_distance)
{}

//===============================================================
//	Function
//===============================================================
NL_INLINE 
NIntersection& NIntersection::Set(	const NVECTOR3&	_intersection, 
									const NVECTOR3&	_v0,
									const NVECTOR3&	_v1,
									const NVECTOR3&	_v2,
									float			_distance  )
{			
	intersection = _intersection;
	v0 = _v0;
	v1 = _v1;
	v2 = _v2;
	distance = _distance;
	return *this;
}
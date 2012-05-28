#pragma once

//===============================================================
//  NAME	  : NFRUSTUM
//  COMMENTS  : 절두체 구조체 구현
//===============================================================
//===============================================================
//	Constructor
//===============================================================
NL_INLINE
NFRUSTUM::NFRUSTUM(void): bBuild(false) {}

//===============================================================
//	Function
//===============================================================

// 월드 정점은 투영 행렬까지 거치면 
// (-1,-1,0) ~ (1,1,1) 사이의 값으로 변한다.
static const NVECTOR3 __frustum_projVertex[8] =
{
	// 위 좌상
	NVECTOR3(-1.f, 1.f, 1.f),
	// 위 우상
	NVECTOR3( 1.f, 1.f, 1.f),
	// 위 우하
	NVECTOR3( 1.f, 1.f, 0.f),
	// 위 좌하
	NVECTOR3(-1.f, 1.f, 0.f),
	// 아래 좌상
	NVECTOR3(-1.f,-1.f, 1.f),
	// 아래 우상
	NVECTOR3( 1.f,-1.f, 1.f),
	// 아래 우하
	NVECTOR3( 1.f,-1.f, 0.f),
	// 아래 좌하
	NVECTOR3(-1.f,-1.f, 0.f)
};

//-------------------------------
//	Build
//-------------------------------
NL_INLINE
NFRUSTUM& NFRUSTUM::Build(const NVECTOR3& pos, const NMATRIX& _invViewProj)
{
	// 카메라 위치 설정
	cameraPos = pos;

	// 뷰 행렬 + 투영 행렬의 역행렬을
	// 투영 정점에 곱하면 월드 정점이 된다.
	for( DWORD i = 0; i < 8; ++i )
		_invViewProj.MultiplyCoord( vertice[i], __frustum_projVertex[i] );

	// 평면 생성
	//	 
	//		          FAR
	//  	    p0━━━━━━p1
	//		    /│          /│
	//		   / │ TOP     / │
	//		  /  │        /  │
	// LEFT p3-━┿━━━━p2 │ RIGHT
	//  	┃  p4--------╂--p5
	//  	┃  /         ┃  /
	//  	┃ /   BOTTOM ┃ /
	//  	┃/           ┃/
	//  	p7━━━━━━p6
	//			 NEAR
	//
	plane[ TOP_PLANE	].Create( vertice[0], vertice[1], vertice[3] );
	plane[ BOTTOM_PLANE	].Create( vertice[7], vertice[6], vertice[4] );
	plane[ NEAR_PLANE	].Create( vertice[3], vertice[2], vertice[7] );
	plane[ FAR_PLANE	].Create( vertice[1], vertice[0], vertice[5] );
	plane[ LEFT_PLANE	].Create( vertice[0], vertice[3], vertice[4] );
	plane[ RIGHT_PLANE	].Create( vertice[2], vertice[1], vertice[6] );

	bBuild = true;

	return *this;
}
NL_INLINE 
NFRUSTUM& NFRUSTUM::Build(const NVECTOR3& pos, const NMATRIX& view, const NMATRIX& proj)
{
	return Build( pos, (view * proj).Inverse44() );
}
//-------------------------------
//	IsInFrustum
//-------------------------------
NL_INLINE 
bool NFRUSTUM::IsInFrustum(const NVECTOR3& pt) const
{
	for( DWORD i = 0; i < 6; ++i )
		if( plane[i].DotCoord(pt) > 0.0f )
			return false;
	return true;
}
//-------------------------------
//	IsInSphere
//-------------------------------
NL_INLINE 
bool NFRUSTUM::IsInSphere(const NVECTOR3& pt, float radius) const
{
	for( DWORD i = 0; i < 6; ++i )
		if( plane[i].DotCoord(pt) > radius )
			return false;
	return true;
}
//-------------------------------
//	IsBuild
//-------------------------------
NL_INLINE 
bool NFRUSTUM::IsBuild(void) const
{
	return bBuild;
}
//-------------------------------
//	Reset
//-------------------------------
NL_INLINE
NFRUSTUM& NFRUSTUM::Reset(void)
{
	bBuild = false;
	return *this;
}
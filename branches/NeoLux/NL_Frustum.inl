#pragma once

//===============================================================
//  NAME	  : NFRUSTUM
//  COMMENTS  : ����ü ����ü ����
//===============================================================
//===============================================================
//	Constructor
//===============================================================
NL_INLINE
NFRUSTUM::NFRUSTUM(void): bBuild(false) {}

//===============================================================
//	Function
//===============================================================

// ���� ������ ���� ��ı��� ��ġ�� 
// (-1,-1,0) ~ (1,1,1) ������ ������ ���Ѵ�.
static const NVECTOR3 __frustum_projVertex[8] =
{
	// �� �»�
	NVECTOR3(-1.f, 1.f, 1.f),
	// �� ���
	NVECTOR3( 1.f, 1.f, 1.f),
	// �� ����
	NVECTOR3( 1.f, 1.f, 0.f),
	// �� ����
	NVECTOR3(-1.f, 1.f, 0.f),
	// �Ʒ� �»�
	NVECTOR3(-1.f,-1.f, 1.f),
	// �Ʒ� ���
	NVECTOR3( 1.f,-1.f, 1.f),
	// �Ʒ� ����
	NVECTOR3( 1.f,-1.f, 0.f),
	// �Ʒ� ����
	NVECTOR3(-1.f,-1.f, 0.f)
};

//-------------------------------
//	Build
//-------------------------------
NL_INLINE
NFRUSTUM& NFRUSTUM::Build(const NVECTOR3& pos, const NMATRIX& _invViewProj)
{
	// ī�޶� ��ġ ����
	cameraPos = pos;

	// �� ��� + ���� ����� �������
	// ���� ������ ���ϸ� ���� ������ �ȴ�.
	for( DWORD i = 0; i < 8; ++i )
		_invViewProj.MultiplyCoord( vertice[i], __frustum_projVertex[i] );

	// ��� ����
	//	 
	//		          FAR
	//  	    p0������������p1
	//		    /��          /��
	//		   / �� TOP     / ��
	//		  /  ��        /  ��
	// LEFT p3-������������p2 �� RIGHT
	//  	��  p4--------��--p5
	//  	��  /         ��  /
	//  	�� /   BOTTOM �� /
	//  	��/           ��/
	//  	p7������������p6
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
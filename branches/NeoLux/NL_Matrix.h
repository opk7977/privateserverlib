#pragma once

#include <cstdio>
#include "NL_Vector.h"

//===================================================================
//  NAME	  : NMATRIX
//  COMMENTS  : 4X4 행렬 구조체
//===================================================================
struct D3DXMATRIX;
struct NMATRIX
{
	//-------------------------------
	//	Matrix Data
	//-------------------------------
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float		m[4][4];
	};

	//-------------------------------
	//	Constructor
	//-------------------------------
	NMATRIX(void);
	NMATRIX(float __11, float __12, float __13, float __14,
			float __21, float __22, float __23, float __24,
			float __31, float __32, float __33, float __34,
			float __41, float __42, float __43, float __44 );
	NMATRIX(const NMATRIX& rhs);

	//-------------------------------
	//	Arithmetic Operator
	//-------------------------------
	NMATRIX		operator -  (void) const;
	NMATRIX		operator +  (const NMATRIX& rhs) const;
	NMATRIX		operator -  (const NMATRIX& rhs) const;
	NMATRIX		operator *  (const NMATRIX& rhs) const;
	NVECTOR3	operator *  (const NVECTOR3& rhs) const;
	NMATRIX		operator *  (float scalar) const;
	NMATRIX		operator /  (float scalar) const;
		
	NMATRIX&	operator =  (const NMATRIX& rhs);
	NMATRIX&	operator += (const NMATRIX& rhs);
	NMATRIX&	operator -= (const NMATRIX& rhs);
	NMATRIX&	operator *= (const NMATRIX& rhs);
	NMATRIX&	operator *= (float scalar);
	NMATRIX&	operator /= (float scalar);

	friend NVECTOR3 operator * (const NVECTOR3& lhs, const NMATRIX& rhs);

	//-------------------------------
	//	Logical Operator
	//-------------------------------
	bool		operator == (const NMATRIX& rhs) const;
	bool		operator != (const NMATRIX& rhs) const;

	//-------------------------------
	//	Bracket Operator
	//-------------------------------
	float&	operator () (int row, int col);
	float	operator () (int row, int col) const;

	//-------------------------------
	//	Casting Operator
	//-------------------------------
	operator D3DXMATRIX*();
	operator const D3DXMATRIX*() const;

	//-------------------------------
	//	Getter
	//-------------------------------
	NVECTOR3&		GetRow3(int index);
	const NVECTOR3& GetRow3(int index) const;
	NVECTOR4&		GetRow4(int index);
	const NVECTOR4& GetRow4(int index) const;
	NVECTOR3		GetEulerDegree() const;
	NVECTOR3		GetEulerRadian() const;
	
	//-------------------------------
	//	Multiply
	//-------------------------------
	NMATRIX&	Multiply33WithMe(const NMATRIX& rhs);
	NMATRIX&	Multiply44WithMe(const NMATRIX& rhs);
	NMATRIX&	Multiply33(NMATRIX& out, const NMATRIX& rhs) const;
	NMATRIX&	Multiply44(NMATRIX& out, const NMATRIX& rhs) const;
	NVECTOR3&	MultiplyCoord(NVECTOR3& out, const NVECTOR3& rhs) const;
	NVECTOR3&	MultiplyNormal(NVECTOR3& out, const NVECTOR3& rhs) const;
	
	//-------------------------------
	//	Transformation
	//-------------------------------
	NMATRIX&	Identity(void);
	NMATRIX&	Set(const NVECTOR3& v1, const NVECTOR3& v2, const NVECTOR3& v3);
	NMATRIX&	Set(const NVECTOR3& v1, const NVECTOR3& v2, 
					const NVECTOR3& v3, const NVECTOR3& v4	);
	NMATRIX&	Scale(const NVECTOR3& vec);
	NMATRIX&	Scale(float x, float y, float z);
	NMATRIX&	RotateX(float radian);
	NMATRIX&	RotateY(float radian);
	NMATRIX&	RotateZ(float radian);
	NMATRIX&	RotateAxisN(const NVECTOR3& axis, float radian);
	NMATRIX&	RotateAxis(NVECTOR3& axis, float radian);
	NMATRIX&	Translation(const NVECTOR3& vec);
	NMATRIX&	Translation(float x, float y, float z);
	NMATRIX&	Transpose(void);
	NMATRIX&	Transpose(NMATRIX& out) const;
	
	//-------------------------------
	//	Inverse
	//-------------------------------
	float		Determinant33(void) const;
	float		Determinant44(void) const;
	NMATRIX&	Inverse33(void);
	NMATRIX&	Inverse44(void);
	NMATRIX&	Inverse33(NMATRIX& out) const;
	NMATRIX&	Inverse44(NMATRIX& out) const;
	NMATRIX&	InverseOrtho(void);
	NMATRIX&	InverseOrtho(NMATRIX& out) const;
	
	//------------------------------------------
	//	ObjectLookAtLH
	//------------------------------------------
	NMATRIX&	ObjectLookAtLHByDirN(	const NVECTOR3& vPosition,
										const NVECTOR3& vDirection,
										const NVECTOR3& vUp );

	NMATRIX&	ObjectLookAtLHByDir(	const NVECTOR3& vPosition,
											  NVECTOR3& vDirection,
										const NVECTOR3& vUp );
	
	NMATRIX&	ObjectLookAtLH(	const NVECTOR3& vPosition,
								const NVECTOR3& vTarget,
								const NVECTOR3& vUp	);

	//---------------------------------------
	//	View Matrix
	//---------------------------------------
	NMATRIX& SetViewMatrixLHByDirN(	const NVECTOR3& vPosition,
									const NVECTOR3& vDirection,
									const NVECTOR3& vUp );
	NMATRIX& SetViewMatrixLHByDir(	const NVECTOR3& vPosition,
										  NVECTOR3& vDirection,
									const NVECTOR3& vUp );
	NMATRIX& SetViewMatrixLH(	const NVECTOR3& vPosition,
								const NVECTOR3& vTarget,
								const NVECTOR3& vUp	);
	
	//---------------------------------------
	//	Projection Matrix
	//---------------------------------------
	NMATRIX& SetOrthoLH(float width,
						float height,
						float zNear,
						float zFar );
	NMATRIX& SetOrthoOffCenterLH(float left,
								 float right,
								 float bottom,
								 float top,
								 float zNear,
								 float zFar );
	NMATRIX& SetPerspectiveFovLH(	float fovY,
									float aspect,
									float zNear,
									float zFar );
	NMATRIX& SetPerspectiveOffCenterLH(	float left,
										float right,
										float bottom,
										float top,
										float zNear,
										float zFar );
};

#include "NL_Matrix.inl"
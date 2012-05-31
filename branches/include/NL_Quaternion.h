#pragma once

#include "NL_Matrix.h"

//===================================================================
//  NAME	  : NQUATERNION
//  COMMENTS  : 사원수 구조체
//===================================================================
struct D3DXQUATERNION;
struct NQUATERNION
{
	//-------------------------------
	//	Quaternion Data
	//-------------------------------
	union
	{
		struct  
		{
			float x, y, z, w;
		};
		float v[4];
	};
	

	//-------------------------------
	//	Constructor
	//-------------------------------
	NQUATERNION(void);
	NQUATERNION(float _x, float _y, float _z, float _w);
	NQUATERNION(const NVECTOR3& _axis, float _angle);
	NQUATERNION(const NQUATERNION& rhs);
	
	//-------------------------------
	//	Arithmetic Operator
	//-------------------------------
	NQUATERNION		operator - (void) const;
	NQUATERNION		operator + (const NQUATERNION& rhs) const;
	NQUATERNION		operator - (const NQUATERNION& rhs) const;
	NQUATERNION		operator * (const NQUATERNION& rhs) const;
	NQUATERNION		operator * (float scalar) const;
	NQUATERNION		operator / (float scalar) const;
	
	NQUATERNION&	operator =  (const NQUATERNION& rhs);
	NQUATERNION&	operator += (const NQUATERNION& rhs);
	NQUATERNION&	operator -= (const NQUATERNION& rhs);
	NQUATERNION&	operator *= (const NQUATERNION& rhs);
	NQUATERNION&	operator *= (float scalar);
	NQUATERNION&	operator /= (float scalar);

	friend NQUATERNION operator * (float scalar, const NQUATERNION& rhs);

	//-------------------------------
	//	Logical Operator
	//-------------------------------
	bool	operator == (const NQUATERNION& rhs) const;
	bool	operator != (const NQUATERNION& rhs) const;

	//-------------------------------
	//	Casting Operator
	//-------------------------------
	operator D3DXQUATERNION* ();
	operator const D3DXQUATERNION* () const;

	//-------------------------------
	//	Function
	//-------------------------------
	NQUATERNION&	Identity(void);
	bool			IsIdentity(void) const;
	float 			LengthSq(void) const;
	float 			Length(void) const;
	NQUATERNION&	Normalize(void);
	NQUATERNION&	Conjugate(void);
	NQUATERNION&	Conjugate(NQUATERNION& out) const;
	float 			Dot(const NQUATERNION& rhs) const;
	NQUATERNION&	Multiply(NQUATERNION& out, const NQUATERNION& rhs) const;
	NQUATERNION&	Lerp(NQUATERNION& out, const NQUATERNION& rhs, float t) const;
	NQUATERNION&	SLerp(NQUATERNION& out, const NQUATERNION& rhs, float t) const;

	NQUATERNION&	SetAxisAngle(const NVECTOR3& axis, float angle);
	NQUATERNION&	SetAxisAngle(float axisX, float axisY, float axisZ, float angle);
	void			GetAxisAngle(NVECTOR3& outAxis, float& outAngle) const;
	NMATRIX&		GetMatrix(NMATRIX& out) const;
};

#include "NL_Quaternion.inl"
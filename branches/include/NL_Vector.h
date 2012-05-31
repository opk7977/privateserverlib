#pragma once

#include "NL_MathBase.h"

//===================================================================
//  NAME	  : NVECTOR2
//  COMMENTS  : 2D ∫§≈Õ ±∏¡∂√º
//===================================================================
struct D3DXVECTOR2;
struct NVECTOR2
{
	//-------------------------------
	//	Vector Data
	//-------------------------------
	union
	{
		struct { float x, y; };
		float v[2];
	};

	//-------------------------------
	//	Constructor
	//-------------------------------
	NVECTOR2(void);
	NVECTOR2(float _x, float _y);
	NVECTOR2(const NVECTOR2& rhs);

	//-------------------------------
	//	Arithmetical Operator
	//-------------------------------
	NVECTOR2	operator -  (void)					const;
	NVECTOR2	operator +  (const NVECTOR2& rhs)	const;
	NVECTOR2	operator -  (const NVECTOR2& rhs)	const;
	NVECTOR2	operator *  (float scalar)			const;
	NVECTOR2	operator /  (float scalar)			const;
	
	NVECTOR2&	operator =  (const NVECTOR2& rhs);
	NVECTOR2&	operator += (const NVECTOR2& rhs);
	NVECTOR2&	operator -= (const NVECTOR2& rhs);
	NVECTOR2&	operator *= (float scalar);
	NVECTOR2&	operator /= (float scalar);
	
	friend NVECTOR2	operator * (float scalar, const NVECTOR2& vec);
	friend NVECTOR2	operator / (float scalar, const NVECTOR2& vec);

	//-------------------------------
	//	Logical Operator
	//-------------------------------
	bool	operator == (const NVECTOR2& rhs) const;
	bool	operator != (const NVECTOR2& rhs) const;

	//-------------------------------
	//	Dot Operator
	//-------------------------------
	float	operator | (const NVECTOR2& rhs) const;

	//-------------------------------
	//	Casting Operator
	//-------------------------------
	operator D3DXVECTOR2* ();
	operator const D3DXVECTOR2* () const;
	
	//-------------------------------
	//	Function
	//-------------------------------
	NVECTOR2&	Set(float value);
	NVECTOR2&	Set(float _x, float _y);
	NVECTOR2&	Normalize(void);
	float		LengthSq(void) const;
	float		Length(void) const;
	float		DistanceSq(const NVECTOR2& rhs) const;
	float		Distance(const NVECTOR2& rhs)	const;
	float		Angle(const NVECTOR2& rhs)		const;
	float		Dot(const NVECTOR2& rhs)		const;
	NVECTOR2&	Lerp(NVECTOR2& out, const NVECTOR2& rhs, float t) const;
};

//===================================================================
//  NAME	  : NVECTOR
//  COMMENTS  : 3D ∫§≈Õ ±∏¡∂√º
//===================================================================
struct D3DXVECTOR3;
struct NVECTOR3
{
	//-------------------------------
	//	Vector Data
	//-------------------------------
	union
	{
		struct { float x, y, z;	};
		float v[3];
	};

	//-------------------------------
	//	Constructor
	//-------------------------------
	NVECTOR3(void);
	NVECTOR3(float _x, float _y, float _z);
	NVECTOR3(const NVECTOR3& rhs);

	//-------------------------------
	//	Arithmetical Operator
	//-------------------------------
	NVECTOR3	operator -  (void) const;
	NVECTOR3	operator +  (const NVECTOR3& rhs) const;
	NVECTOR3	operator -  (const NVECTOR3& rhs) const;
	NVECTOR3	operator *  (float scalar) const;
	NVECTOR3	operator /  (float scalar) const;
	
	NVECTOR3&	operator =  (const NVECTOR3& rhs);
	NVECTOR3&	operator += (const NVECTOR3& rhs);
	NVECTOR3&	operator -= (const NVECTOR3& rhs);
	NVECTOR3&	operator *= (float scalar);
	NVECTOR3&	operator /= (float scalar);
	
	friend NVECTOR3	operator * (float scalar, const NVECTOR3& vec);
	friend NVECTOR3	operator / (float scalar, const NVECTOR3& vec);

	//-------------------------------
	//	Logical Operator
	//-------------------------------
	bool	operator == (const NVECTOR3& rhs) const;
	bool	operator != (const NVECTOR3& rhs) const;

	//-------------------------------
	//	Dot & Cross Operator
	//-------------------------------
	float		operator | (const NVECTOR3& rhs) const;
	NVECTOR3	operator ^ (const NVECTOR3& rhs) const;

	//-------------------------------
	//	Casting Operator
	//-------------------------------
	operator D3DXVECTOR3* ();
	operator const D3DXVECTOR3* () const;

	//-------------------------------
	//	Function
	//-------------------------------
	NVECTOR3&	Set(float value);
	NVECTOR3&	Set(float _x, float _y, float _z);
	NVECTOR3&	Normalize(void);
	float		LengthSq(void) const;
	float		Length(void) const;
	float		DistanceSq(const NVECTOR3& rhs) const;
	float		Distance(const NVECTOR3& rhs) const;
	float		Angle(const NVECTOR3& rhs) const;
	float		Dot(const NVECTOR3& rhs) const;
	NVECTOR3&	Cross(NVECTOR3& out, const NVECTOR3& rhs) const;
	NVECTOR3&	Lerp(NVECTOR3& out, const NVECTOR3& rhs, float t) const;
};

//===================================================================
//  NAME	  : NVECTOR4
//  COMMENTS  : 4D ∫§≈Õ ±∏¡∂√º
//===================================================================
struct D3DXVECTOR4;
struct NVECTOR4
{
	//-------------------------------
	//	Vector Data
	//-------------------------------
	union
	{
		struct { float x, y, z, w;			};
		float	v[4];
	};

	//-------------------------------
	//	Constructor
	//-------------------------------
	NVECTOR4(void);
	NVECTOR4(float _x, float _y, float _z, float _w);
	NVECTOR4(const NVECTOR3& xyz, float _w);
	NVECTOR4(const NVECTOR4& rhs);

	//-------------------------------
	//	Arithmetical Operator
	//-------------------------------
	NVECTOR4	operator -  (void) const;
	NVECTOR4	operator +  (const NVECTOR4& rhs) const;
	NVECTOR4	operator -  (const NVECTOR4& rhs) const;
	NVECTOR4	operator *  (float scalar) const;
	NVECTOR4	operator /  (float scalar) const;
	
	NVECTOR4&	operator =  (const NVECTOR4& rhs);
	NVECTOR4&	operator += (const NVECTOR4& rhs);
	NVECTOR4&	operator -= (const NVECTOR4& rhs);
	NVECTOR4&	operator *= (float scalar);
	NVECTOR4&	operator /= (float scalar);
	
	friend NVECTOR4	operator * (float scalar, const NVECTOR4& vec);
	friend NVECTOR4	operator / (float scalar, const NVECTOR4& vec);

	//-------------------------------
	//	Logical Operator
	//-------------------------------
	bool	operator == (const NVECTOR4& rhs) const;
	bool	operator != (const NVECTOR4& rhs) const;

	//-------------------------------
	//	Dot Operator
	//-------------------------------
	float		operator | (const NVECTOR4& rhs) const;

	//-------------------------------
	//	Casting Operator
	//-------------------------------
	operator D3DXVECTOR4* ();
	operator const D3DXVECTOR4* () const;

	//-------------------------------
	//	Getter
	//-------------------------------
	const NVECTOR3& GetVector() const;
	NVECTOR3&		GetVector();

	//-------------------------------
	//	Function
	//-------------------------------
	NVECTOR4&	Set(float value);
	NVECTOR4&	Set(const NVECTOR3& _xyz, float _w);
	NVECTOR4&	Set(float _x, float _y, float _z, float _w);
	NVECTOR4&	Normalize(void);
	float		LengthSq(void) const;
	float		Length(void) const;
	float		DistanceSq(const NVECTOR4& rhs) const;
	float		Distance(const NVECTOR4& rhs) const;
	float		Angle(const NVECTOR4& rhs) const;
	float		Dot(const NVECTOR4& rhs) const;
	NVECTOR4&	Lerp(NVECTOR4& out, const NVECTOR4& rhs, float t) const;
};

#include "NL_Vector.inl"
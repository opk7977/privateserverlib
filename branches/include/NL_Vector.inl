#pragma once

//===============================================================
//  NAME	  : NVECTOR2
//  COMMENTS  : 2D 벡터 구조체 구현
//===============================================================
//===============================================================
//	Constructor
//===============================================================
NL_INLINE 
NVECTOR2::NVECTOR2(void)					
	: x(0.0f), y(0.0f)	{}

NL_INLINE 
NVECTOR2::NVECTOR2(float _x, float _y)
	: x(_x), y(_y)		{}

NL_INLINE 
NVECTOR2::NVECTOR2(const NVECTOR2& rhs)
	: x(rhs.x), y(rhs.y){}

//===============================================================
//	Arithmetic Operator
//===============================================================
//-------------------------------
//	operator - (single)
//-------------------------------
NL_INLINE 
NVECTOR2 NVECTOR2::operator - (void) const
{
	return NVECTOR2(-x, -y);
}
//-------------------------------
//	operator +
//-------------------------------
NL_INLINE 
NVECTOR2 NVECTOR2::operator + (const NVECTOR2& rhs) const
{
	return NVECTOR2( x+rhs.x, y+rhs.y );
}
//-------------------------------
//	operator -
//-------------------------------
NL_INLINE 
NVECTOR2 NVECTOR2::operator - (const NVECTOR2& rhs) const
{
	return NVECTOR2( x-rhs.x, y-rhs.y );
}
//-------------------------------
//	operator *
//-------------------------------
NL_INLINE 
NVECTOR2 NVECTOR2::operator * (float scalar) const
{
	return NVECTOR2( x*scalar, y*scalar );
}
//-------------------------------
//	operator /
//-------------------------------
NL_INLINE 
NVECTOR2 NVECTOR2::operator / (float scalar) const
{
	float	invScalar = 1.0f / scalar;
	return NVECTOR2( x*invScalar, y*invScalar );
}
//-------------------------------
//	operator =
//-------------------------------
NL_INLINE 
NVECTOR2& NVECTOR2::operator = (const NVECTOR2& rhs)
{
	x = rhs.x;
	y = rhs.y;
	return *this;
}
//-------------------------------
//	operator +=
//-------------------------------
NL_INLINE 
NVECTOR2& NVECTOR2::operator += (const NVECTOR2& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}
//-------------------------------
//	operator -=
//-------------------------------
NL_INLINE 
NVECTOR2& NVECTOR2::operator -= (const NVECTOR2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}
//-------------------------------
//	operator *=
//-------------------------------
NL_INLINE 
NVECTOR2& NVECTOR2::operator *= (float scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}
//-------------------------------
//	operator /=
//-------------------------------
NL_INLINE 
NVECTOR2& NVECTOR2::operator /= (float scalar)
{
	float	invScalar = 1.0f / scalar;
	x *= invScalar;
	y *= invScalar;
	return *this;
}

//===============================================================
//	Logical Operator
//===============================================================
//-------------------------------
//	operator ==
//-------------------------------
NL_INLINE 
bool NVECTOR2::operator == (const NVECTOR2& rhs) const
{
	if( fabs(x - rhs.x) >= NL_EPSILON )
		return false;
	if( fabs(y - rhs.y) >= NL_EPSILON )
		return false;
	return true;
}
//-------------------------------
//	operator !=
//-------------------------------
NL_INLINE 
bool NVECTOR2::operator != (const NVECTOR2& rhs) const
{
	return !( this->operator==(rhs) );
}

//-------------------------------
//	Dot Operator
//-------------------------------
NL_INLINE 
float NVECTOR2::operator | (const NVECTOR2& rhs) const
{
	return x*rhs.x + y*rhs.y;
}

//-------------------------------
//	Casting Operator
//-------------------------------
NL_INLINE
NVECTOR2::operator D3DXVECTOR2* ()
{
	return (D3DXVECTOR2*)&x;
}
NL_INLINE
NVECTOR2::operator const D3DXVECTOR2* () const
{
	return (const D3DXVECTOR2*)&x;
}

//===============================================================
//	Function
//===============================================================
//-------------------------------
//	Set(same value)
//-------------------------------
NL_INLINE 
NVECTOR2& NVECTOR2::Set( float value )
{
	x = y = value;
	return *this;
}
//-------------------------------
//	Set(x, y, z)
//-------------------------------
NL_INLINE 
NVECTOR2& NVECTOR2::Set( float _x, float _y )
{
	x = _x; y = _y;
	return *this;
}
//-------------------------------
//	Normalize
//-------------------------------
NL_INLINE 
NVECTOR2& NVECTOR2::Normalize(void)
{
	float invLen = 1.0f / Length();
	x *= invLen;
	y *= invLen;
	return *this;
}
//-------------------------------
//	LengthSq
//-------------------------------
NL_INLINE 
float NVECTOR2::LengthSq(void) const
{
	return x*x + y*y;
}
//-------------------------------
//	Length
//-------------------------------
NL_INLINE 
float NVECTOR2::Length(void) const
{
	return sqrtf( LengthSq() );
}
//-------------------------------
//	DistanceSq
//-------------------------------
NL_INLINE 
float NVECTOR2::DistanceSq(const NVECTOR2& rhs) const
{
	return (rhs.x-x)*(rhs.x-x) + (rhs.y-y)*(rhs.y-y);
}
//-------------------------------
//	Distance
//-------------------------------
NL_INLINE 
float NVECTOR2::Distance(const NVECTOR2& rhs) const
{
	return sqrtf( DistanceSq(rhs) );
}
//-------------------------------
//	Angle
//-------------------------------
NL_INLINE 
float NVECTOR2::Angle(const NVECTOR2& rhs) const
{
	return acosf( ( (*this) | rhs ) / ( Length() * rhs.Length() ) );
}
//-------------------------------
//	Dot
//-------------------------------
NL_INLINE 
float NVECTOR2::Dot(const NVECTOR2& rhs) const
{
	return x*rhs.x + y*rhs.y;
}
//-------------------------------
//	Lerp
//-------------------------------
NL_INLINE
NVECTOR2& NVECTOR2::Lerp(NVECTOR2& out, const NVECTOR2& rhs, float t) const
{
	out.x = x + t * (rhs.x - x);
    out.y = y + t * (rhs.y - y);
	return out;
}

//===============================================================
//	Global Operator
//===============================================================
//-------------------------------
//	operator * (Global)
//-------------------------------
NL_INLINE 
NVECTOR2 operator * (float scalar, const NVECTOR2& vec)
{
	return NVECTOR2( vec.x*scalar, vec.y*scalar );
}
//-------------------------------
//	operator / (Global)
//-------------------------------
NL_INLINE 
NVECTOR2 operator / (float scalar, const NVECTOR2& vec)
{
	float invScalar = 1.0f / scalar;
	return NVECTOR2( vec.x*invScalar, vec.y*invScalar );
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

//===============================================================
//  NAME	  : NVECTOR3
//  COMMENTS  : 3D 벡터 구조체 구현
//===============================================================
//===============================================================
//	Constructor
//===============================================================
NL_INLINE 
NVECTOR3::NVECTOR3(void)					
	: x(0.0f), y(0.0f), z(0.0f)	{}

NL_INLINE 
NVECTOR3::NVECTOR3(float _x, float _y, float _z)
	: x(_x), y(_y), z(_z) {}

NL_INLINE 
NVECTOR3::NVECTOR3(const NVECTOR3& rhs)
	: x(rhs.x), y(rhs.y), z(rhs.z) {}

//===============================================================
//	Arithmetic Operator
//===============================================================
//-------------------------------
//	operator - (single)
//-------------------------------
NL_INLINE 
NVECTOR3 NVECTOR3::operator - (void) const
{
	return NVECTOR3(-x, -y, -z);
}
//-------------------------------
//	operator +
//-------------------------------
NL_INLINE
NVECTOR3 NVECTOR3::operator + (const NVECTOR3& rhs) const
{
	return NVECTOR3( x+rhs.x, y+rhs.y, z+rhs.z );
}
//-------------------------------
//	operator -
//-------------------------------
NL_INLINE 
NVECTOR3 NVECTOR3::operator - (const NVECTOR3& rhs) const
{
	return NVECTOR3( x-rhs.x, y-rhs.y, z-rhs.z );
}
//-------------------------------
//	operator *
//-------------------------------
NL_INLINE 
NVECTOR3 NVECTOR3::operator * (float scalar) const
{
	return NVECTOR3( x*scalar, y*scalar, z*scalar );
}
//-------------------------------
//	operator /
//-------------------------------
NL_INLINE 
NVECTOR3 NVECTOR3::operator / (float scalar) const
{
	float invScalar = 1.0f / scalar;
	return NVECTOR3( x*invScalar, y*invScalar, z*invScalar );
}
//-------------------------------
//	operator =
//-------------------------------
NL_INLINE 
NVECTOR3& NVECTOR3::operator = (const NVECTOR3& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}
//-------------------------------
//	operator +=
//-------------------------------
NL_INLINE 
NVECTOR3& NVECTOR3::operator += (const NVECTOR3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}
//-------------------------------
//	operator -=
//-------------------------------
NL_INLINE 
NVECTOR3& NVECTOR3::operator -= (const NVECTOR3& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}
//-------------------------------
//	operator *=
//-------------------------------
NL_INLINE 
NVECTOR3& NVECTOR3::operator *= (float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}
//-------------------------------
//	operator /=
//-------------------------------
NL_INLINE 
NVECTOR3& NVECTOR3::operator /= (float scalar)
{
	float invScalar = 1.0f / scalar;
	x *= invScalar;
	y *= invScalar;
	z *= invScalar;
	return *this;
}

//===============================================================
//	Logical Operator
//===============================================================
//-------------------------------
//	operator ==
//-------------------------------
NL_INLINE 
bool NVECTOR3::operator == (const NVECTOR3& rhs) const
{
	if( fabs(x - rhs.x) >= NL_EPSILON )
		return false;
	if( fabs(y - rhs.y) >= NL_EPSILON )
		return false;
	if( fabs(z - rhs.z) >= NL_EPSILON )
		return false;
	return true;
}
//-------------------------------
//	operator !=
//-------------------------------
NL_INLINE 
bool NVECTOR3::operator != (const NVECTOR3& rhs) const
{
	return !( this->operator==(rhs) );
}

//===============================================================
//	Dot & Cross Operator
//===============================================================
//-------------------------------
//	Dot Operator
//-------------------------------
NL_INLINE 
float NVECTOR3::operator | (const NVECTOR3& rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z;
}
//-------------------------------
//	Cross Operator
//-------------------------------
NL_INLINE
NVECTOR3 NVECTOR3::operator ^ (const NVECTOR3& rhs) const
{
	return NVECTOR3( y*rhs.z - z*rhs.y,
					 z*rhs.x - x*rhs.z,
					 x*rhs.y - y*rhs.x ); 
}

//-------------------------------
//	Casting Operator
//-------------------------------
NL_INLINE
NVECTOR3::operator D3DXVECTOR3* ()
{
	return (D3DXVECTOR3*)&x;
}
NL_INLINE
NVECTOR3::operator const D3DXVECTOR3* () const
{
	return (const D3DXVECTOR3*)&x;
}

//===============================================================
//	Function
//===============================================================
//-------------------------------
//	Set(same value)
//-------------------------------
NL_INLINE 
NVECTOR3& NVECTOR3::Set( float value )
{
	x = y = z = value;
	return *this;
}
//-------------------------------
//	Set(x, y, z)
//-------------------------------
NL_INLINE 
NVECTOR3& NVECTOR3::Set( float _x, float _y, float _z )
{
	x = _x; y = _y; z = _z;
	return *this;
}
//-------------------------------
//	Normalize
//-------------------------------
NL_INLINE 
NVECTOR3& NVECTOR3::Normalize(void)
{
	float invLen = 1.0f / Length();
	x *= invLen;
	y *= invLen;
	z *= invLen;
	return *this;
}
//-------------------------------
//	LengthSq
//-------------------------------
NL_INLINE 
float NVECTOR3::LengthSq(void) const
{
	return x*x + y*y + z*z;	
}
//-------------------------------
//	Length
//-------------------------------
NL_INLINE 
float NVECTOR3::Length(void) const
{
	return sqrtf( LengthSq() );
}
//-------------------------------
//	DistanceSq
//-------------------------------
NL_INLINE 
float NVECTOR3::DistanceSq(const NVECTOR3& rhs) const
{
	return (rhs.x-x)*(rhs.x-x) + (rhs.y-y)*(rhs.y-y) + (rhs.z-z)*(rhs.z-z);
}
//-------------------------------
//	Distance
//-------------------------------
NL_INLINE 
float NVECTOR3::Distance(const NVECTOR3& rhs) const
{
	return sqrtf( DistanceSq(rhs) );
}
//-------------------------------
//	Angle
//-------------------------------
NL_INLINE 
float NVECTOR3::Angle(const NVECTOR3& rhs) const
{
	return acosf( ( (*this) | rhs ) / ( Length() * rhs.Length() ) );
}
//-------------------------------
//	Dot
//-------------------------------
NL_INLINE 
float NVECTOR3::Dot(const NVECTOR3& rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z;
}
//-------------------------------
//	Cross
//-------------------------------
NL_INLINE
NVECTOR3& NVECTOR3::Cross(NVECTOR3& out, const NVECTOR3& rhs) const
{
	out.x = y*rhs.z - z*rhs.y;
	out.y = z*rhs.x - x*rhs.z;
	out.z = x*rhs.y - y*rhs.x;
	return out;
}
//-------------------------------
//	Lerp
//-------------------------------
NL_INLINE
NVECTOR3& NVECTOR3::Lerp(NVECTOR3& out, const NVECTOR3& rhs, float t) const
{
	out.x = x + t * (rhs.x - x);
    out.y = y + t * (rhs.y - y);
	out.z = z + t * (rhs.z - z);
	return out;
}

//===============================================================
//	Global Operator
//===============================================================
//-------------------------------
//	operator * (Global)
//-------------------------------
NL_INLINE 
NVECTOR3 operator * (float scalar, const NVECTOR3& vec)
{
	return NVECTOR3( vec.x*scalar, vec.y*scalar, vec.z*scalar );
}
//-------------------------------
//	operator / (Global)
//-------------------------------
NL_INLINE 
NVECTOR3 operator / (float scalar, const NVECTOR3& vec)
{
	float invScalar = 1.0f / scalar;
	return NVECTOR3( vec.x*invScalar, vec.y*invScalar, vec.z*invScalar );
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

//===============================================================
//  NAME	  : NVECTOR4
//  COMMENTS  : 4D 벡터 구조체 구현
//===============================================================
//===============================================================
//	Constructor
//===============================================================
NL_INLINE 
NVECTOR4::NVECTOR4(void)					
	: x(0.0f), y(0.0f), z(0.0f), w(0.0f) 
{

}

NL_INLINE 
NVECTOR4::NVECTOR4(float _x, float _y, float _z, float _w)
	: x(_x), y(_y), z(_z), w(_w) 
{

}

NL_INLINE 
NVECTOR4::NVECTOR4(const NVECTOR3& xyz, float _w)
	: x(xyz.x), y(xyz.y), z(xyz.z), w(_w) 
{

}

NL_INLINE 
NVECTOR4::NVECTOR4(const NVECTOR4& rhs)
	: x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) 
{

}

//===============================================================
//	Arithmetic Operator
//===============================================================
//-------------------------------
//	operator - (single)
//-------------------------------
NL_INLINE 
NVECTOR4 NVECTOR4::operator - (void) const
{
	return NVECTOR4(-x, -y, -z, -w);
}
//-------------------------------
//	operator +
//-------------------------------
NL_INLINE
NVECTOR4 NVECTOR4::operator + (const NVECTOR4& rhs) const
{
	return NVECTOR4( x+rhs.x, y+rhs.y, z+rhs.z, w+rhs.w );
}
//-------------------------------
//	operator -
//-------------------------------
NL_INLINE 
NVECTOR4 NVECTOR4::operator - (const NVECTOR4& rhs) const
{
	return NVECTOR4( x-rhs.x, y-rhs.y, z-rhs.z, w-rhs.w );
}
//-------------------------------
//	operator *
//-------------------------------
NL_INLINE 
NVECTOR4 NVECTOR4::operator * (float scalar) const
{
	return NVECTOR4( x*scalar, y*scalar, z*scalar, w*scalar );
}
//-------------------------------
//	operator /
//-------------------------------
NL_INLINE 
NVECTOR4 NVECTOR4::operator / (float scalar) const
{
	float invScalar = 1.0f / scalar;
	return NVECTOR4( x*invScalar, y*invScalar, z*invScalar, w*invScalar );
}
//-------------------------------
//	operator =
//-------------------------------
NL_INLINE 
NVECTOR4& NVECTOR4::operator = (const NVECTOR4& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}
//-------------------------------
//	operator +=
//-------------------------------
NL_INLINE 
NVECTOR4& NVECTOR4::operator += (const NVECTOR4& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}
//-------------------------------
//	operator -=
//-------------------------------
NL_INLINE 
NVECTOR4& NVECTOR4::operator -= (const NVECTOR4& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}
//-------------------------------
//	operator *=
//-------------------------------
NL_INLINE 
NVECTOR4& NVECTOR4::operator *= (float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}
//-------------------------------
//	operator /=
//-------------------------------
NL_INLINE 
NVECTOR4& NVECTOR4::operator /= (float scalar)
{
	float invScalar = 1.0f / scalar;
	x *= invScalar;
	y *= invScalar;
	z *= invScalar;
	w *= invScalar;
	return *this;
}

//===============================================================
//	Logical Operator
//===============================================================
//-------------------------------
//	operator ==
//-------------------------------
NL_INLINE 
bool NVECTOR4::operator == (const NVECTOR4& rhs) const
{
	if( fabs(x - rhs.x) >= NL_EPSILON )
		return false;
	if( fabs(y - rhs.y) >= NL_EPSILON )
		return false;
	if( fabs(z - rhs.z) >= NL_EPSILON )
		return false;
	if( fabs(w - rhs.w) >= NL_EPSILON )
		return false;
	return true;
}
//-------------------------------
//	operator !=
//-------------------------------
NL_INLINE 
bool NVECTOR4::operator != (const NVECTOR4& rhs) const
{
	return !( this->operator==(rhs) );
}

//===============================================================
//	Dot Operator
//===============================================================
//-------------------------------
//	Dot Operator
//-------------------------------
NL_INLINE 
float NVECTOR4::operator | (const NVECTOR4& rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w;
}

//-------------------------------
//	Casting Operator
//-------------------------------
NL_INLINE
NVECTOR4::operator D3DXVECTOR4* ()
{
	return (D3DXVECTOR4*)&x;
}
NL_INLINE
NVECTOR4::operator const D3DXVECTOR4* () const
{
	return (const D3DXVECTOR4*)&x;
}

//-------------------------------
//	Getter
//-------------------------------
NL_INLINE
const NVECTOR3& NVECTOR4::GetVector() const
{
	return (const NVECTOR3&)x;
}

NL_INLINE
NVECTOR3&		NVECTOR4::GetVector()
{
	return (NVECTOR3&)x;
}

//===============================================================
//	Function
//===============================================================
//-------------------------------
//	Set(same value)
//-------------------------------
NL_INLINE 
NVECTOR4& NVECTOR4::Set( float value )
{
	x = y = z = w = value;
	return *this;
}
//-------------------------------
//	Set(xyz vec, w)
//-------------------------------
NL_INLINE
NVECTOR4& NVECTOR4::Set(const NVECTOR3& _xyz, float _w)
{
	x = _xyz.x;
	y = _xyz.y;
	z = _xyz.z;
	w = _w;
	return *this;
}
//-------------------------------
//	Set(x, y, z)
//-------------------------------
NL_INLINE 
NVECTOR4& NVECTOR4::Set( float _x, float _y, float _z, float _w )
{
	x = _x; y = _y; z = _z; w = _w;
	return *this;
}
//-------------------------------
//	Normalize
//-------------------------------
NL_INLINE 
NVECTOR4& NVECTOR4::Normalize(void)
{
	float invLen = 1.0f / Length();
	x *= invLen;
	y *= invLen;
	z *= invLen;
	w *= invLen;
	return *this;
}
//-------------------------------
//	LengthSq
//-------------------------------
NL_INLINE 
float NVECTOR4::LengthSq(void) const
{
	return x*x + y*y + z*z + w*w;	
}
//-------------------------------
//	Length
//-------------------------------
NL_INLINE 
float NVECTOR4::Length(void) const
{
	return sqrtf( LengthSq() );
}
//-------------------------------
//	DistanceSq
//-------------------------------
NL_INLINE 
float NVECTOR4::DistanceSq(const NVECTOR4& rhs) const
{
	return	(rhs.x-x)*(rhs.x-x) + (rhs.y-y)*(rhs.y-y) + 
			(rhs.z-z)*(rhs.z-z) + (rhs.w-w)*(rhs.w-w);
}
//-------------------------------
//	Distance
//-------------------------------
NL_INLINE 
float NVECTOR4::Distance(const NVECTOR4& rhs) const
{
	return sqrtf( DistanceSq(rhs) );
}
//-------------------------------
//	Angle
//-------------------------------
NL_INLINE 
float NVECTOR4::Angle(const NVECTOR4& rhs) const
{
	return acosf( ( (*this) | rhs ) / ( Length() * rhs.Length() ) );
}
//-------------------------------
//	Dot
//-------------------------------
NL_INLINE 
float NVECTOR4::Dot(const NVECTOR4& rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w;
}
//-------------------------------
//	Lerp
//-------------------------------
NL_INLINE
NVECTOR4& NVECTOR4::Lerp(NVECTOR4& out, const NVECTOR4& rhs, float t) const
{
	out.x = x + t * (rhs.x - x);
    out.y = y + t * (rhs.y - y);
	out.z = z + t * (rhs.z - z);
	out.w = w + t * (rhs.w - w);
	return out;
}

//===============================================================
//	Global Operator
//===============================================================
//-------------------------------
//	operator * (Global)
//-------------------------------
NL_INLINE 
NVECTOR4 operator * (float scalar, const NVECTOR4& vec)
{
	return NVECTOR4( vec.x*scalar, vec.y*scalar, vec.z*scalar, vec.w*scalar );
}
//-------------------------------
//	operator / (Global)
//-------------------------------
NL_INLINE 
NVECTOR4 operator / (float scalar, const NVECTOR4& vec)
{
	float invScalar = 1.0f / scalar;
	return NVECTOR4( vec.x*invScalar, vec.y*invScalar, vec.z*invScalar, vec.w*invScalar );
}
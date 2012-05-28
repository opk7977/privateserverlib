#pragma once

//===============================================================
//  NAME	  : NQUATERNION
//  COMMENTS  : 사원수 구조체 구현
//===============================================================
//===============================================================
//	Constructor
//===============================================================
NL_INLINE 
NQUATERNION::NQUATERNION(void) : x(0.0f), y(0.0f), z(0.0f), w(1.0f) 
{

}

NL_INLINE 
NQUATERNION::NQUATERNION(float _x, float _y, float _z, float _w) :
								x(_x), y(_y), z(_z), w(_w) 
{

}

NL_INLINE 
NQUATERNION::NQUATERNION(const NVECTOR3& axis, float angle)
{	
	SetAxisAngle( axis.x, axis.y, axis.z, angle );
}

NL_INLINE 
NQUATERNION::NQUATERNION(const NQUATERNION& rhs):
								x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) 
{

}

//-------------------------------
//	Arithmetic Operator
//-------------------------------
NL_INLINE 
NQUATERNION	NQUATERNION::operator - (void) const
{
	return NQUATERNION(-x, -y, -z, -w);
}
NL_INLINE 
NQUATERNION	NQUATERNION::operator + (const NQUATERNION& rhs) const
{
	return NQUATERNION( x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w );
}
NL_INLINE 
NQUATERNION	NQUATERNION::operator - (const NQUATERNION& rhs) const
{
	return NQUATERNION( x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w );
}
NL_INLINE 
NQUATERNION	NQUATERNION::operator * (const NQUATERNION& rhs) const
{
	return NQUATERNION(
		w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y,
		w*rhs.y - x*rhs.z + y*rhs.w + z*rhs.x,
		w*rhs.z + x*rhs.y - y*rhs.x + z*rhs.w,
		w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z);
}
NL_INLINE 
NQUATERNION	NQUATERNION::operator * (float scalar) const
{
	return NQUATERNION( x * scalar, y * scalar, z * scalar, w * scalar );
}
NL_INLINE 
NQUATERNION	NQUATERNION::operator / (float scalar) const
{
	float inv = 1.0f / scalar;
	return NQUATERNION( x*inv, y*inv, z*inv, w*inv );
}
NL_INLINE 
NQUATERNION operator * (float scalar, const NQUATERNION& rhs)
{
	return NQUATERNION( scalar*rhs.x, scalar*rhs.y, scalar*rhs.z, scalar*rhs.w );
}
NL_INLINE 
NQUATERNION& NQUATERNION::operator = (const NQUATERNION& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}
NL_INLINE 
NQUATERNION& NQUATERNION::operator += (const NQUATERNION& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}
NL_INLINE 
NQUATERNION& NQUATERNION::operator -= (const NQUATERNION& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}
NL_INLINE 
NQUATERNION& NQUATERNION::operator *= (const NQUATERNION& rhs)
{
	NQUATERNION qCpy(
		w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y,
		w*rhs.y - x*rhs.z + y*rhs.w + z*rhs.x,
		w*rhs.z + x*rhs.y - y*rhs.x + z*rhs.w,
		w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z);
	*this = qCpy;
	return *this;
}
NL_INLINE 
NQUATERNION& NQUATERNION::operator *= (float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}
NL_INLINE 
NQUATERNION& NQUATERNION::operator /= (float scalar)
{
	float inv = 1.0f / scalar;
	x *= inv;
	y *= inv;
	z *= inv;
	w *= inv;
	return *this;
}

//-------------------------------
//	Logical Operator
//-------------------------------
NL_INLINE
bool NQUATERNION::operator == (const NQUATERNION& rhs) const
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
NL_INLINE
bool NQUATERNION::operator != (const NQUATERNION& rhs) const
{
	return !( this->operator==(rhs) );
}

//-------------------------------
//	Casting Operator
//-------------------------------
NL_INLINE
NQUATERNION::operator D3DXQUATERNION* ()
{
	return (D3DXQUATERNION*)&x;
}
NL_INLINE
NQUATERNION::operator const D3DXQUATERNION* () const
{
	return (const D3DXQUATERNION*)&x;
}

//-------------------------------
//	Function
//-------------------------------
NL_INLINE 
NQUATERNION& NQUATERNION::Identity(void)
{
	x = y = z = 0.0f; w = 1.0f;
}
NL_INLINE 
bool NQUATERNION::IsIdentity( void ) const
{
	if(fabs(x) >= NL_EPSILON)			return false;
	if(fabs(y) >= NL_EPSILON)			return false;
	if(fabs(z) >= NL_EPSILON)			return false;
	if(fabs(1.0f - w) >= NL_EPSILON)	return false;
	return true;
}
NL_INLINE 
float NQUATERNION::LengthSq(void) const
{
	return x*x + y*y + z*z + w*w;
}
NL_INLINE 
float NQUATERNION::Length(void) const
{
	return sqrtf( LengthSq() );
}
NL_INLINE 
NQUATERNION& NQUATERNION::Normalize(void)
{
	*this /= Length();
	return *this;
}
NL_INLINE 
NQUATERNION& NQUATERNION::Conjugate(void)
{
	x = -x; y = -y; z = -z;
	return *this;
}
NL_INLINE 
NQUATERNION& NQUATERNION::Conjugate(NQUATERNION& out) const
{
	out.x = -x;
	out.y = -y;
	out.z = -z;
	out.w = w;
	return out;
}
NL_INLINE 
float NQUATERNION::Dot(const NQUATERNION& rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w;
}
NL_INLINE 
NQUATERNION& NQUATERNION::Multiply(NQUATERNION& out, const NQUATERNION& rhs) const
{
	out.x = w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y;
	out.y = w*rhs.y - x*rhs.z + y*rhs.w + z*rhs.x;
	out.z = w*rhs.z + x*rhs.y - y*rhs.x + z*rhs.w;
	out.w = w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z;
	return out;
}
NL_INLINE 
NQUATERNION& NQUATERNION::Lerp(NQUATERNION& out, const NQUATERNION& rhs, float t) const
{
	float cosAlpha = Dot(rhs);
	
	float f = 1.0f - 0.7878088f * cosAlpha;
	float k = 0.5069269f;
	f *= f;
	k *= f;
	float b = 2*k;
	float c = -3*k;
	float d = 1.0f + k;
	t = t*(b*t + c) + d;

	out = (*this)*(1.0f - t) + rhs*t;
	out.Normalize();
	return out;
}
NL_INLINE 
NQUATERNION& NQUATERNION::SLerp(NQUATERNION& out, const NQUATERNION& rhs, float t) const
{
	float cosTheta = Dot(rhs);
	if( 0.0f > cosTheta )
	{
		out = -rhs;
	}
	else
		out = rhs;

	float sinTheta = sqrtf( 1.0f - cosTheta*cosTheta );
	float ratio1, ratio2;
	if( NL_EPSILON < fabs(sinTheta) )
	{
		float angle = atan2f(sinTheta, cosTheta);
		float invSinTheta = 1.0f / sinTheta;
		ratio1 = sinf(angle*(1.0f-t)) * invSinTheta;
		ratio2 = sinf(angle*t) * invSinTheta;
	}
	else
	{
		ratio1 = 1.0f - t;
		ratio2 = t;
	}

	out = (*this)*ratio1 + out*ratio2;
	out.Normalize();
	return out;
}
NL_INLINE 
NQUATERNION& NQUATERNION::SetAxisAngle(const NVECTOR3& axis, float angle)
{
	return SetAxisAngle(axis.x, axis.y, axis.z, angle);
}
NL_INLINE 
NQUATERNION& NQUATERNION::SetAxisAngle(float axisX, float axisY, float axisZ, float angle)
{
	float halfAngle = angle * 0.5f;
	float sinVal = sinf( halfAngle );
	x = sinVal * axisX;
	y = sinVal * axisY;
	z = sinVal * axisZ;
	w = cosf( halfAngle );
	return *this;
}
NL_INLINE 
void NQUATERNION::GetAxisAngle(NVECTOR3& outAxis, float& outAngle) const
{
	outAngle = acosf(w);
	float invSinTheta = 1.0f / sinf( outAngle );
	outAxis.x = x * invSinTheta;
	outAxis.y = y * invSinTheta;
	outAxis.z = z * invSinTheta;

	outAngle *= 2.0f;
}
NL_INLINE 
NMATRIX& NQUATERNION::GetMatrix(NMATRIX& out) const
{
	float tx = x*2.0f;
	float ty = y*2.0f;
	float tz = z*2.0f;
	float txw = tx*w;
	float tyw = ty*w;
	float tzw = tz*w;
	float txx = tx*x;
	float txy = tx*y;
	float txz = tx*z;
	float tyy = ty*y;
	float tyz = ty*z;
	float tzz = tz*z;
	
	out._11 = 1.0f - tyy - tzz;
	out._21 = txy - tzw;
	out._31 = txz + tyw;
	out._12 = txy + tzw;
	out._22 = 1.0f - txx - tzz;
	out._32 = tyz - txw;
	out._13 = txz - tyw;
	out._23 = tyz + txw;
	out._33 = 1.0f - txx - tyy;

	return out;
}
#pragma once

//===================================================================
//  NAME	  : NMATRIX
//  COMMENTS  : 4X4 행렬 구조체 구현
//===================================================================
//===============================================================
//	Constructor
//===============================================================
NL_INLINE 
NMATRIX::NMATRIX(void) : 
						_11(1.0f), _12(0.0f), _13(0.0f), _14(0.0f),
						_21(0.0f), _22(1.0f), _23(0.0f), _24(0.0f),
						_31(0.0f), _32(0.0f), _33(1.0f), _34(0.0f),
						_41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f) {}
NL_INLINE 
NMATRIX::NMATRIX(	float __11, float __12, float __13, float __14,
					float __21, float __22, float __23, float __24,
					float __31, float __32, float __33, float __34,
					float __41, float __42, float __43, float __44) :
						_11(__11), _12(__12), _13(__13), _14(__14),
						_21(__21), _22(__22), _23(__23), _24(__24),
						_31(__31), _32(__32), _33(__33), _34(__34),
						_41(__41), _42(__42), _43(__43), _44(__44) {}
NL_INLINE 
NMATRIX::NMATRIX(const NMATRIX& rhs) :
						_11(rhs._11), _12(rhs._12), _13(rhs._13), _14(rhs._14),
						_21(rhs._21), _22(rhs._22), _23(rhs._23), _24(rhs._24),
						_31(rhs._31), _32(rhs._32), _33(rhs._33), _34(rhs._34),
						_41(rhs._41), _42(rhs._42), _43(rhs._43), _44(rhs._44) {}

//===============================================================
//	Arithmetic Operator
//===============================================================
//-------------------------------
//	operator - (unary)
//-------------------------------
NL_INLINE 
NMATRIX	NMATRIX::operator - (void) const
{
	return NMATRIX(
		-_11, -_12, -_13, -_14,
		-_21, -_22, -_23, -_24,
		-_31, -_32, -_33, -_34,
		-_41, -_42, -_43, -_44 );
}
//-------------------------------
//	operator +
//-------------------------------
NL_INLINE 
NMATRIX	NMATRIX::operator + (const NMATRIX& rhs) const
{
	return NMATRIX(
		_11+rhs._11, _12+rhs._12, _13+rhs._13, _14+rhs._14,
		_21+rhs._21, _22+rhs._22, _23+rhs._23, _24+rhs._24,
		_31+rhs._31, _32+rhs._32, _33+rhs._33, _34+rhs._34,
		_41+rhs._41, _42+rhs._42, _43+rhs._43, _44+rhs._44 );
}
//-------------------------------
//	operator -
//-------------------------------
NL_INLINE 
NMATRIX	NMATRIX::operator-(const NMATRIX& rhs) const
{
	return NMATRIX(
		_11-rhs._11, _12-rhs._12, _13-rhs._13, _14-rhs._14,
		_21-rhs._21, _22-rhs._22, _23-rhs._23, _24-rhs._24,
		_31-rhs._31, _32-rhs._32, _33-rhs._33, _34-rhs._34,
		_41-rhs._41, _42-rhs._42, _43-rhs._43, _44-rhs._44 );
}
//-------------------------------
//	Operator * (with Matrix)
//-------------------------------
NL_INLINE 
NMATRIX	NMATRIX::operator*(const NMATRIX& rhs) const
{
	NMATRIX retMatrix;
	for( int row = 0; row < 4; ++row )
	{
		for( int col = 0; col < 4; ++col )
		{
			retMatrix.m[row][col] =	m[row][0] * rhs.m[0][col] + 
									m[row][1] * rhs.m[1][col] +
									m[row][2] * rhs.m[2][col] +
									m[row][3] * rhs.m[3][col];
		}
	}
	return retMatrix;
}
//-------------------------------
//	Operator * (with Vector)
//-------------------------------
NL_INLINE 
NVECTOR3 NMATRIX::operator * (const NVECTOR3& rhs) const
{
	return NVECTOR3(
		_11*rhs.x + _21*rhs.y + _31*rhs.z,
		_12*rhs.x + _22*rhs.y + _32*rhs.z,
		_13*rhs.x + _23*rhs.y + _33*rhs.z );
}
//-------------------------------
//	operator *
//-------------------------------
NL_INLINE 
NMATRIX	NMATRIX::operator * (float scalar) const
{
	return NMATRIX(
		_11*scalar, _12*scalar, _13*scalar, _14*scalar,
		_21*scalar, _22*scalar, _23*scalar, _24*scalar,
		_31*scalar, _32*scalar, _33*scalar, _34*scalar,
		_41*scalar, _42*scalar, _43*scalar, _44*scalar );
}
//-------------------------------
//	operator /
//-------------------------------
NL_INLINE 
NMATRIX	NMATRIX::operator / (float scalar) const
{
	float invScalar = 1.0f / scalar;
	return NMATRIX(
		_11*invScalar, _12*invScalar, _13*invScalar, _14*invScalar,
		_21*invScalar, _22*invScalar, _23*invScalar, _24*invScalar,
		_31*invScalar, _32*invScalar, _33*invScalar, _34*invScalar,
		_41*invScalar, _42*invScalar, _43*invScalar, _44*invScalar );
}
//-------------------------------
//	operator =
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::operator = (const NMATRIX& rhs)
{
	memcpy( (void*)this, (void*)&rhs, sizeof( NMATRIX ) );

	return *this;
}
//-------------------------------
//	operator +=
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::operator += (const NMATRIX& rhs)
{
	_11+=rhs._11, _12+=rhs._12, _13+=rhs._13, _14+=rhs._14,
	_21+=rhs._21, _22+=rhs._22, _23+=rhs._23, _24+=rhs._24,
	_31+=rhs._31, _32+=rhs._32, _33+=rhs._33, _34+=rhs._34,
	_41+=rhs._41, _42+=rhs._42, _43+=rhs._43, _44+=rhs._44;
	
	return *this;
}
//-------------------------------
//	operator -=
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::operator -= (const NMATRIX& rhs)
{
	_11-=rhs._11, _12-=rhs._12, _13-=rhs._13, _14-=rhs._14,
	_21-=rhs._21, _22-=rhs._22, _23-=rhs._23, _24-=rhs._24,
	_31-=rhs._31, _32-=rhs._32, _33-=rhs._33, _34-=rhs._34,
	_41-=rhs._41, _42-=rhs._42, _43-=rhs._43, _44-=rhs._44;

	return *this;
}
//-------------------------------
//	operator *= (with matrix)
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::operator *= (const NMATRIX& rhs)
{
	return Multiply44WithMe( rhs );
}
//-------------------------------
//	operator *= (with float)
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::operator *= (float scalar)
{
	_11*=scalar, _12*=scalar, _13*=scalar, _14*=scalar,
	_21*=scalar, _22*=scalar, _23*=scalar, _24*=scalar,
	_31*=scalar, _32*=scalar, _33*=scalar, _34*=scalar,
	_41*=scalar, _42*=scalar, _43*=scalar, _44*=scalar;

	return *this;
}
//-------------------------------
//	operator /=
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::operator /= (float scalar)
{
	float invScalar = 1.0f / scalar;
	_11*=invScalar, _12*=invScalar, _13*=invScalar, _14*=invScalar,
	_21*=invScalar, _22*=invScalar, _23*=invScalar, _24*=invScalar,
	_31*=invScalar, _32*=invScalar, _33*=invScalar, _34*=invScalar,
	_41*=invScalar, _42*=invScalar, _43*=invScalar, _44*=invScalar;

	return *this;
}

//===============================================================
//	Logical Operator
//===============================================================
//-------------------------------
//	operator ==
//-------------------------------
NL_INLINE
bool NMATRIX::operator == (const NMATRIX& rhs) const
{
	for( int row = 0; row < 4; ++row )
		for( int col = 0; col < 4; ++col )
			if( fabs(m[row][col] - rhs.m[row][col]) >= NL_EPSILON )
				return false;
	return true;
}
//-------------------------------
//	operator !=
//-------------------------------
NL_INLINE 
bool NMATRIX::operator != (const NMATRIX& rhs) const
{
	return !this->operator==(rhs);
}

//===============================================================
//	Bracket Operator
//===============================================================
//-------------------------------
//	operator ()
//-------------------------------
NL_INLINE 
float& NMATRIX::operator()(int row, int col)
{
	return m[row][col];
}
//-------------------------------
//	operator () const
//-------------------------------
NL_INLINE 
float  NMATRIX::operator()(int row, int col) const
{
	return m[row][col];
}
//-------------------------------
//	Casting Operator
//-------------------------------
NL_INLINE
NMATRIX::operator D3DXMATRIX*()
{
	return (D3DXMATRIX*)&_11;
}
NL_INLINE
NMATRIX::operator const D3DXMATRIX*() const
{
	return (const D3DXMATRIX*)&_11;
}

//-------------------------------
//	Getter
//-------------------------------
NL_INLINE
NVECTOR3&		NMATRIX::GetRow3(int index)
{
	return (NVECTOR3&)*m[index];
}
NL_INLINE
const NVECTOR3& NMATRIX::GetRow3(int index) const
{
	return (const NVECTOR3&)*m[index];
}
NL_INLINE
NVECTOR4&		NMATRIX::GetRow4(int index)
{
	return (NVECTOR4&)*m[index];
}
NL_INLINE
const NVECTOR4& NMATRIX::GetRow4(int index) const
{
	return (const NVECTOR4&)*m[index];
}
NL_INLINE 
NVECTOR3 NMATRIX::GetEulerDegree() const
{
	return this->GetEulerRadian() * NL_180_DIV_PI;
}
NL_INLINE 
NVECTOR3 NMATRIX::GetEulerRadian() const
{
	NVECTOR3 ret;
	float scaleY;
	scaleY = this->GetRow3(1).Length();
	ret.y = -asinf( this->_13 / scaleY );

	float cosY = cosf( ret.y );
	if( fabs( cosY ) < NL_EPSILON )
	{
		ret.x = 0.0f;
		ret.z = atan2f( -this->_21, this->_22 );
	}
	else
	{
		float scaleZ = this->GetRow3(2).Length();

		float invCosY = 1.0f / cosY;
		ret.x = -atan2f( this->_32 * invCosY * scaleZ, this->_33 * invCosY * scaleY );
		ret.z = -atan2f( this->_12 * invCosY, this->_11 * invCosY );
	}

	return ret;
}

//===============================================================
//	Multiply Function
//===============================================================
//-------------------------------
//	Multiply (with Matrix)	
//-------------------------------
NL_INLINE
NMATRIX& NMATRIX::Multiply33WithMe(const NMATRIX& rhs)
{
	NMATRIX tempMat( *this );
	for( int row = 0; row < 3; ++row )
		for( int col = 0; col < 3; ++col )
			this->m[row][col] =	tempMat.m[row][0] * rhs.m[0][col] + 
								tempMat.m[row][1] * rhs.m[1][col] +
								tempMat.m[row][2] * rhs.m[2][col];
	return *this;
}

NL_INLINE
NMATRIX& NMATRIX::Multiply44WithMe(const NMATRIX& rhs)
{
	NMATRIX tempMat( *this );
	for( int row = 0; row < 4; ++row )
		for( int col = 0; col < 4; ++col )
			this->m[row][col] =	tempMat.m[row][0] * rhs.m[0][col] + 
								tempMat.m[row][1] * rhs.m[1][col] +
								tempMat.m[row][2] * rhs.m[2][col] +
								tempMat.m[row][3] * rhs.m[3][col];
	return *this;
}

NL_INLINE
NMATRIX& NMATRIX::Multiply33(NMATRIX& out, const NMATRIX& rhs) const
{
	for( int row = 0; row < 3; ++row )
	{
		for( int col = 0; col < 3; ++col )
		{
			out.m[row][col] =	m[row][0] * rhs.m[0][col] + 
								m[row][1] * rhs.m[1][col] +
								m[row][2] * rhs.m[2][col];
		}
	}

	return out;
}
NL_INLINE 
NMATRIX& NMATRIX::Multiply44(NMATRIX& out, const NMATRIX& rhs) const
{
	for( int row = 0; row < 4; ++row )
	{
		for( int col = 0; col < 4; ++col )
		{
			out.m[row][col] =	m[row][0] * rhs.m[0][col] + 
								m[row][1] * rhs.m[1][col] +
								m[row][2] * rhs.m[2][col] +
								m[row][3] * rhs.m[3][col];
		}
	}

	return out;
}

//-------------------------------
//	MultiplyCoord (with 'W = 1')
//-------------------------------
NL_INLINE 
NVECTOR3& NMATRIX::MultiplyCoord(NVECTOR3& out, const NVECTOR3& rhs) const
{
	out.x			= _11*rhs.x + _21*rhs.y + _31*rhs.z + _41;
	out.y			= _12*rhs.x + _22*rhs.y + _32*rhs.z + _42;
	out.z			= _13*rhs.x + _23*rhs.y + _33*rhs.z + _43;
	float tempW	= _14*rhs.x + _24*rhs.y + _34*rhs.z + _44;

	tempW = 1.0f / tempW;
	out.x *= tempW;
	out.y *= tempW;
	out.z *= tempW;

	return out;
}
//-------------------------------
//	MultiplyNormal (with 'W = 0')
//-------------------------------
NL_INLINE 
NVECTOR3& NMATRIX::MultiplyNormal(NVECTOR3& out, const NVECTOR3& rhs) const
{
	out.x	= _11*rhs.x + _21*rhs.y + _31*rhs.z;
	out.y	= _12*rhs.x + _22*rhs.y + _32*rhs.z;
	out.z	= _13*rhs.x + _23*rhs.y + _33*rhs.z;

	return out;
}

//===============================================================
//	Function
//===============================================================
//-------------------------------
//	Identity
//-------------------------------
static const NMATRIX __matrix_identityMat(	1.0f, 0.0f, 0.0f, 0.0f,
											0.0f, 1.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 1.0f, 0.0f,
											0.0f, 0.0f, 0.0f, 1.0f );
NL_INLINE 
NMATRIX& NMATRIX::Identity(void)
{
	memcpy( (void*)this, (void*)&__matrix_identityMat, sizeof(NMATRIX) );
	return *this;
}
//-------------------------------
//	Set
//-------------------------------
NL_INLINE
NMATRIX& NMATRIX::Set(const NVECTOR3& v1, const NVECTOR3& v2, const NVECTOR3& v3)
{
	GetRow3(0) = v1;		_14 = 0.0f;
	GetRow3(1) = v2;		_24 = 0.0f;
	GetRow3(2) = v3;		_34 = 0.0f;
	GetRow3(3).Set(0.0f);	_44 = 1.0f;
	return *this;
}
NL_INLINE
NMATRIX& NMATRIX::Set(	const NVECTOR3& v1, const NVECTOR3& v2, 
						const NVECTOR3& v3, const NVECTOR3& v4	)
{
	GetRow3(0) = v1; _14 = 0.0f;
	GetRow3(1) = v2; _24 = 0.0f;
	GetRow3(2) = v3; _34 = 0.0f;
	GetRow3(3) = v4; _44 = 1.0f; 
	return *this;
}
//-------------------------------
//	Scale(vector)
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::Scale(const NVECTOR3& vec)
{
	_11 = vec.x;
	_22 = vec.y;
	_33 = vec.z;
	return *this;
}
//-------------------------------
//	Scale(xyz)
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::Scale(float x, float y, float z)
{
	_11 = x;
	_22 = y;
	_33 = z;
	return *this;
}
//-------------------------------
//	RotateX
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::RotateX(float radian)
{
	float cosTheta = cosf(radian);
	float sinTheta = sinf(radian);
	_22 = cosTheta;
	_23 = sinTheta;
	_32 = -sinTheta;
	_33 = cosTheta;
	return *this;
}
//-------------------------------
//	RotateY
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::RotateY(float radian)
{
	float cosTheta = cosf(radian);
	float sinTheta = sinf(radian);
	_11 = cosTheta;
	_13 = -sinTheta;
	_31 = sinTheta;
	_33 = cosTheta;
	return *this;
}
//-------------------------------
//	RotateZ
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::RotateZ( float radian )
{
	float cosTheta = cosf(radian);
	float sinTheta = sinf(radian);
	_11 = cosTheta;
	_12 = -sinTheta;
	_21 = sinTheta;
	_22 = cosTheta;
	return *this;
}
//-------------------------------
//	RotateAxis(axis is normalized)
//-------------------------------
NL_INLINE
NMATRIX& NMATRIX::RotateAxisN(const NVECTOR3& axis, float radian)
{
	float invCosTheta = 1.0f - cosf(radian);
	float sinTheta = sinf(radian);
	
	_11 = axis.x * axis.x * ( invCosTheta ) + ( 1.0f - invCosTheta );
	_12 = axis.x * axis.y * ( invCosTheta ) + axis.z * sinTheta;
	_13 = axis.x * axis.z * ( invCosTheta ) + axis.y * sinTheta;

	_21 = axis.y * axis.x * ( invCosTheta ) - axis.z * sinTheta;
	_22 = axis.y * axis.y * ( invCosTheta ) + ( 1.0f - invCosTheta );
	_23 = axis.y * axis.z * ( invCosTheta ) + axis.x * sinTheta;

	_31 = axis.z * axis.x * ( invCosTheta ) + axis.y * sinTheta;
	_32 = axis.z * axis.y * ( invCosTheta ) - axis.x * sinTheta;
	_33 = axis.z * axis.z * ( invCosTheta ) + ( 1.0f - invCosTheta );

	_41 = _42 = _43 = _14 = _24 = _34 = 0.0f;
	_44 = 1.0f;
	return *this;
}
//-------------------------------
//	RotateAxis
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::RotateAxis(NVECTOR3& axis, float radian)
{
	axis.Normalize();
	return RotateAxisN(axis, radian);
}
//-------------------------------
//	Translation(vector)	
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::Translation(const NVECTOR3& vec)
{
	_41 = vec.x;
	_42 = vec.y;
	_43 = vec.z;
	return *this;
}
//-------------------------------
//	Translation(xyz)	
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::Translation( float x, float y, float z )
{
	_41 = x;
	_42 = y;
	_43 = z;
	return *this;
}
//-------------------------------
//	Transpose
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::Transpose(void)
{
	std::swap(_12, _21); 
	std::swap(_13, _31); 
	std::swap(_14, _41);
	std::swap(_23, _32); 
	std::swap(_24, _42); 
	std::swap(_34, _43);
	return *this;
}
//-------------------------------
//	Transpose(out version)
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::Transpose(NMATRIX& out) const
{
	out._11 = _11;	out._12 = _21;	out._13 = _31;	out._14 = _41;
	out._21 = _12;	out._22 = _22;	out._23 = _32;	out._24 = _42;
	out._31 = _13;	out._32 = _23;	out._33 = _33;	out._34 = _43;
	out._41 = _14;	out._42 = _24;	out._43 = _34;	out._44 = _44;
	return out;
}
//===============================================================
//	Inverse Function
//===============================================================
//-------------------------------
//	Determinant33
//-------------------------------
NL_INLINE 
float NMATRIX::Determinant33(void) const
{
	return	_11*(_22*_33 - _23*_32) - 
			_12*(_21*_33 - _23*_31) + 
			_13*(_21*_32 - _22*_31);
}
//-------------------------------
//	Determinant44
//-------------------------------
NL_INLINE 
float NMATRIX::Determinant44(void) const
{
	float det12 = _31*_42 - _32*_41;
	float det13 = _31*_43 - _33*_41;
	float det14 = _31*_44 - _34*_41;
	float det23 = _32*_43 - _33*_42;
	float det24 = _32*_44 - _34*_42;
	float det34 = _33*_44 - _34*_43;

	return	_11*( _22*det34 - _23*det24 + _24*det23 ) - 
			_12*( _21*det34 - _23*det14 + _24*det13 ) + 
			_13*( _21*det24 - _22*det14 + _24*det12 ) - 
			_14*( _21*det23 - _22*det13 + _23*det12 );
}
//-------------------------------
//	Inverse33
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::Inverse33(void)
{
	NMATRIX cpMat( *this );

	float det11 = cpMat._22*cpMat._33 - cpMat._23*cpMat._32;
	float det21 = cpMat._12*cpMat._33 - cpMat._13*cpMat._32;
	float det31 = cpMat._12*cpMat._23 - cpMat._13*cpMat._22;

	float invDet = 1.0f / ( _11*det11 - _21*det21 + _31*det31 );
	
	_11 =  det11 * invDet;
	_12 = -det21 * invDet;
	_13 =  det31 * invDet;

	_21 = -( cpMat._21*cpMat._33 - cpMat._23*cpMat._31 ) * invDet;
	_22 =  ( cpMat._11*cpMat._33 - cpMat._13*cpMat._31 ) * invDet;
	_23 = -( cpMat._11*cpMat._23 - cpMat._13*cpMat._21 ) * invDet;

	_31 =  ( cpMat._21*cpMat._32 - cpMat._22*cpMat._31 ) * invDet;
	_32 = -( cpMat._11*cpMat._32 - cpMat._12*cpMat._31 ) * invDet;
	_33 =  ( cpMat._11*cpMat._22 - cpMat._12*cpMat._21 ) * invDet;

	return *this;
}
//-------------------------------
//	Inverse44
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::Inverse44(void)
{
	NMATRIX cpMat( *this );
	
	// Up Determinant of 2 X 2
	float det12 = cpMat._11*cpMat._22 - cpMat._12*cpMat._21;
	float det13 = cpMat._11*cpMat._23 - cpMat._13*cpMat._21;
	float det14 = cpMat._11*cpMat._24 - cpMat._14*cpMat._21;
	float det23 = cpMat._12*cpMat._23 - cpMat._13*cpMat._22;
	float det24 = cpMat._12*cpMat._24 - cpMat._14*cpMat._22;
	float det34 = cpMat._13*cpMat._24 - cpMat._14*cpMat._23;

	float detTmp1 = cpMat._42*det34 - cpMat._43*det24 + cpMat._44*det23;
	float detTmp2 = cpMat._41*det34 - cpMat._43*det14 + cpMat._44*det13;
	float detTmp3 = cpMat._41*det24 - cpMat._42*det14 + cpMat._44*det12;
	float detTmp4 = cpMat._41*det23 - cpMat._42*det13 + cpMat._43*det12;

	float invDet = 1.0f / (	cpMat._31*detTmp1 - cpMat._32*detTmp2 + 
								cpMat._33*detTmp3 - cpMat._34*detTmp4	);
	
	_13 =  detTmp1 * invDet;
	_23 = -detTmp2 * invDet;
	_33 =  detTmp3 * invDet;
	_43 = -detTmp4 * invDet;

	_14 = -( cpMat._32*det34 - cpMat._33*det24 + cpMat._34*det23 ) * invDet;
	_24 =  ( cpMat._31*det34 - cpMat._33*det14 + cpMat._34*det13 ) * invDet;
	_34 = -( cpMat._31*det24 - cpMat._32*det14 + cpMat._34*det12 ) * invDet;
	_44 =  ( cpMat._31*det23 - cpMat._32*det13 + cpMat._33*det12 ) * invDet;

	// Down Determinant of 2 X 2
	det12 = cpMat._31*cpMat._42 - cpMat._32*cpMat._41;
	det13 = cpMat._31*cpMat._43 - cpMat._33*cpMat._41;
	det14 = cpMat._31*cpMat._44 - cpMat._34*cpMat._41;
	det23 = cpMat._32*cpMat._43 - cpMat._33*cpMat._42;
	det24 = cpMat._32*cpMat._44 - cpMat._34*cpMat._42;
	det34 = cpMat._33*cpMat._44 - cpMat._34*cpMat._43;

	_11 =  ( cpMat._22*det34 - cpMat._23*det24 + cpMat._24*det23 ) * invDet;
	_21 = -( cpMat._21*det34 - cpMat._23*det14 + cpMat._24*det13 ) * invDet;
	_31 =  ( cpMat._21*det24 - cpMat._22*det14 + cpMat._24*det12 ) * invDet;
	_41 = -( cpMat._21*det23 - cpMat._22*det13 + cpMat._23*det12 ) * invDet;

	_12 = -( cpMat._12*det34 - cpMat._13*det24 + cpMat._14*det23 ) * invDet;
	_22 =  ( cpMat._11*det34 - cpMat._13*det14 + cpMat._14*det13 ) * invDet;
	_32 = -( cpMat._11*det24 - cpMat._12*det14 + cpMat._14*det12 ) * invDet;
	_42 =  ( cpMat._11*det23 - cpMat._12*det13 + cpMat._13*det12 ) * invDet;

	return *this;
}
//-------------------------------
//	Inverse33(out version)
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::Inverse33(NMATRIX& out) const
{
	float det11 = _22*_33 - _23*_32;
	float det21 = _12*_33 - _13*_32;
	float det31 = _12*_23 - _13*_22;

	float invDet = 1.0f / ( _11*det11 - _21*det21 + _31*det31 );
	
	out._11 =  det11 * invDet;
	out._12 = -det21 * invDet;
	out._13 =  det31 * invDet;

	out._21 = -(_21*_33 - _23*_31) * invDet;
	out._22 =  (_11*_33 - _13*_31) * invDet;
	out._23 = -(_11*_23 - _13*_21) * invDet;

	out._31 =  (_21*_32 - _22*_31) * invDet;
	out._32 = -(_11*_32 - _12*_31) * invDet;
	out._33 =  (_11*_22 - _12*_21) * invDet;
	
	return out;
}

//-------------------------------
//	Inverse44(out version)
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::Inverse44(NMATRIX& out) const
{
	// Up Determinant of 2 X 2
	float det12 = _11*_22 - _12*_21;
	float det13 = _11*_23 - _13*_21;
	float det14 = _11*_24 - _14*_21;
	float det23 = _12*_23 - _13*_22;
	float det24 = _12*_24 - _14*_22;
	float det34 = _13*_24 - _14*_23;

	float detTmp1 = _42*det34 - _43*det24 + _44*det23;
	float detTmp2 = _41*det34 - _43*det14 + _44*det13;
	float detTmp3 = _41*det24 - _42*det14 + _44*det12;
	float detTmp4 = _41*det23 - _42*det13 + _43*det12;

	float invDet = 1.0f / ( _31*detTmp1 - _32*detTmp2 + _33*detTmp3 - _34*detTmp4 );
	
	out._13 =  detTmp1 * invDet;
	out._23 = -detTmp2 * invDet;
	out._33 =  detTmp3 * invDet;
	out._43 = -detTmp4 * invDet;

	out._14 = -( _32*det34 - _33*det24 + _34*det23 ) * invDet;
	out._24 =  ( _31*det34 - _33*det14 + _34*det13 ) * invDet;
	out._34 = -( _31*det24 - _32*det14 + _34*det12 ) * invDet;
	out._44 =  ( _31*det23 - _32*det13 + _33*det12 ) * invDet;

	// Down Determinant of 2 X 2
	det12 = _31*_42 - _32*_41;
	det13 = _31*_43 - _33*_41;
	det14 = _31*_44 - _34*_41;
	det23 = _32*_43 - _33*_42;
	det24 = _32*_44 - _34*_42;
	det34 = _33*_44 - _34*_43;

	out._11 =  ( _22*det34 - _23*det24 + _24*det23 ) * invDet;
	out._21 = -( _21*det34 - _23*det14 + _24*det13 ) * invDet;
	out._31 =  ( _21*det24 - _22*det14 + _24*det12 ) * invDet;
	out._41 = -( _21*det23 - _22*det13 + _23*det12 ) * invDet;

	out._12 = -( _12*det34 - _13*det24 + _14*det23 ) * invDet;
	out._22 =  ( _11*det34 - _13*det14 + _14*det13 ) * invDet;
	out._32 = -( _11*det24 - _12*det14 + _14*det12 ) * invDet;
	out._42 =  ( _11*det23 - _12*det13 + _13*det12 ) * invDet;
	
	return out;
}
//-------------------------------
//	InverseOrtho
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::InverseOrtho(void)
{
	*this = NMATRIX(
		_11, _21, _31, 0.0f,
		_12, _22, _32, 0.0f,
		_13, _23, _33, 0.0f,
		-(_11*_41 + _12*_42 + _13*_43),
		-(_21*_41 + _22*_42 + _23*_43),
		-(_31*_41 + _32*_42 + _33*_43),
		1.0f);
	
	return *this;
}
//-------------------------------
//	InverseOrtho(out version)
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::InverseOrtho(NMATRIX& out) const
{
	out._11 = _11;	out._12 = _21;	out._13 = _31;	out._14 = 0.0f;
	out._21 = _12;	out._22 = _22;	out._23 = _32;	out._24 = 0.0f;
	out._31 = _13;	out._32 = _23;	out._33 = _33;	out._34 = 0.0f;
	
	out._41 = -(_11*_41 + _12*_42 + _13*_43);
	out._42 = -(_21*_41 + _22*_42 + _23*_43);
	out._43 = -(_31*_41 + _32*_42 + _33*_43);
	out._44 = 1.0f;
	
	return out;
}
//===============================================================
//	World, View, Projection Matrix Function
//===============================================================
//-------------------------------
//	ObjectLookAtLHByDir( Position, Direction(Normalized), Up )	
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::ObjectLookAtLHByDirN(	const NVECTOR3& vPosition,
										const NVECTOR3& vDirection,
										const NVECTOR3& vUp )
{
	float dist = vDirection.Dot(vUp);
	NVECTOR3 vUpVector = vUp - (vDirection * dist);
	vUpVector.Normalize();

	NVECTOR3 vRightVector;
	vUpVector.Cross(vRightVector, vDirection);

	Set(vRightVector, vUpVector, vDirection, vPosition);

	return *this;
}
//-------------------------------
//	ObjectLookAtLHByDir( Position, Direction, Up )	
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::ObjectLookAtLHByDir(	const NVECTOR3& vPosition,
											  NVECTOR3& vDirection,
										const NVECTOR3& vUp )
{
	vDirection.Normalize();
	return ObjectLookAtLHByDirN( vPosition, vDirection, vUp );
}
//-------------------------------
//	ObjectLookAtLH( Position, Target, Up )	
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::ObjectLookAtLH(	const NVECTOR3& vPosition, 
									const NVECTOR3& vTarget, 
									const NVECTOR3& vUp	)
{
	return ObjectLookAtLHByDir( vPosition, (vTarget - vPosition), vUp );
}
//-------------------------------
//	SetViewMatrixLH( Position, Direction(Normalized), Up )	
//-------------------------------
NL_INLINE
NMATRIX& NMATRIX::SetViewMatrixLHByDirN(const NVECTOR3& vPosition,
										const NVECTOR3& vDirection,
										const NVECTOR3& vUp )
{
	return ObjectLookAtLHByDirN( vPosition, vDirection, vUp ).InverseOrtho();
}
//-------------------------------
//	SetViewMatrixLH( Position, Direction, Up )	
//-------------------------------
NL_INLINE
NMATRIX& NMATRIX::SetViewMatrixLHByDir(	const NVECTOR3& vPosition,
											  NVECTOR3& vDirection,
										const NVECTOR3& vUp )
{
	return ObjectLookAtLHByDir( vPosition, vDirection, vUp ).InverseOrtho();
}
//-------------------------------
//	SetViewMatrixLH( Position, Target, Up )	
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::SetViewMatrixLH(	const NVECTOR3& vPosition,
									const NVECTOR3& vTarget,
									const NVECTOR3& vUp	)
{	
	return ObjectLookAtLH( vPosition, vTarget, vUp ).InverseOrtho();
}

//===============================================================
//	Projection Matrix Function
//===============================================================
//-------------------------------
//	SetOrthoLH
//-------------------------------
NL_INLINE
NMATRIX& NMATRIX::SetOrthoLH(	float width,
								float height,
								float zNear,
								float zFar )
{
	_11 = 2.0f / width;
	_22 = 2.0f / height;
	_33 = 1.0f / (zFar - zNear);
	_43 = -zNear / (zFar - zNear);
	_44 = 1.0f;
	return *this;
}
//-------------------------------
//	SetOrthoOffCenterLH
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::SetOrthoOffCenterLH(	float left,
										float right,
										float bottom,
										float top,
										float zNear,
										float zFar )
{
	_11 = 2.0f / (right - left);
	_22 = 2.0f / (top - bottom);
	_33 = 1.0f / (zFar - zNear);
	_41 = (left + right) / (left - right);
	_42 = (top + bottom) / (bottom - top);
	_43 = -zNear / (zFar - zNear);
	_44 = 1.0f;
	return *this;
}
//-------------------------------
//	SetPerspectiveFovLH
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::SetPerspectiveFovLH(	float fovY,
										float aspect,
										float zNear,
										float zFar )
{
	_22 = 1.0f / tanf(fovY * 0.5f);
	_11 = _22 / aspect;
	_33 = zFar / (zFar - zNear);
	_34 = 1.0f;
	_43 = -zNear * _33;
	_44 = 0.0f;
	return *this;
}
//-------------------------------
//	SetPerspectiveOffCenterLH
//-------------------------------
NL_INLINE 
NMATRIX& NMATRIX::SetPerspectiveOffCenterLH(float left,
											float right,
											float bottom,
											float top,
											float zNear,
											float zFar )
{
	_11 = 2.0f * zNear / (right - left);
	_22 = 2.0f * zNear / (top - bottom);
	_31 = (left + right) / (left - right);
	_32 = (top + bottom) / (bottom - top);
	_33 = zFar / (zFar - zNear);
	_34 = 1.0f;
	_43 = zNear * zFar / (zNear - zFar);
	_44 = 0.0f;
	return *this;
}

//===============================================================
//	Friend Function
//===============================================================
//-------------------------------
//	Global Operator *	
//-------------------------------
NL_INLINE 
NVECTOR3 operator * (const NVECTOR3& lhs, const NMATRIX& rhs)
{
	return NVECTOR3(
			rhs._11*lhs.x + rhs._21*lhs.y + rhs._31*lhs.z,
			rhs._12*lhs.x + rhs._22*lhs.y + rhs._32*lhs.z,
			rhs._13*lhs.x + rhs._23*lhs.y + rhs._33*lhs.z );
}
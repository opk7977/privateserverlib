#pragma once

#include <cmath>
#include <cstring>

//===============================================================
//  NAME	  : NL_MathBase.h
//  COMMENTS  : 수학 관련 변수, 인라인 함수
//===============================================================

// Global Variables
static const float	NL_EPSILON		=  0.001f;

static const float	NL_180_DIV_PI	= 57.295791f;
static const float	NL_PI_X_2		=  6.283185f;
static const float	NL_PI_X_1_5		=  4.712388f;
static const float	NL_PI			=  3.141592f;
static const float	NL_PI_HALF		=  1.570796f;
static const float	NL_PI_THIRD		=  1.047197f;
static const float	NL_PI_QUAT		=  0.785398f;
static const float	NL_PI_SIXTH		=  0.523598f;
static const float	NL_PI_OCTO  	=  0.392699f;
static const float	NL_PI_DIV_180	=  0.017453f;

// Inline function
#define NL_INLINE __forceinline

NL_INLINE
float DEG_TO_RAD(float degree)
{
	return degree * NL_PI_DIV_180;
}

NL_INLINE
float RAD_TO_DEG(float radian)
{
	return radian * NL_180_DIV_PI;
}
// math.h will tell our engine whether or not to use the SIMD library
#ifndef _MATH_H_
#define _MATH_H_

// Change this to 0 if you don't want to use SIMD
#define USE_SIMD 1

#if USE_SIMD

#include "fastmath.h"
namespace ITP485
{
	typedef FastVector3 Vector3;
	typedef FastMatrix4 Matrix4;
	typedef FastQuaternion Quaternion;
}

#else

#include "slowmath.h"
namespace ITP485
{
	typedef SlowVector3 Vector3;
	typedef SlowMatrix4 Matrix4;
	typedef SlowQuaternion Quaternion;
}

#endif // USE_SIMD

#endif // _MATH_H_

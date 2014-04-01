// SlowMath.cpp defines statics for the slow math library
#include "slowmath.h"

namespace ITP485
{
float _ident2[4][4] = {1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f};
const SlowMatrix4 SlowMatrix4::Identity(_ident2);

const SlowVector3 SlowVector3::Zero(0.0f, 0.0f, 0.0f);
const SlowVector3 SlowVector3::UnitX(1.0f, 0.0f, 0.0f);
const SlowVector3 SlowVector3::UnitY(0.0f, 1.0f, 0.0f);
const SlowVector3 SlowVector3::UnitZ(0.0f, 0.0f, 1.0f);
const SlowVector3 SlowVector3::NegativeUnitX(-1.0f, 0.0f, 0.0f);
const SlowVector3 SlowVector3::NegativeUnitY(0.0f, -1.0f, 0.0f);
const SlowVector3 SlowVector3::NegativeUnitZ(0.0f, 0.0f, -1.0f);

const SlowQuaternion SlowQuaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);

void SlowMatrix4::CreateTranslation(const SlowVector3& rhs)
{
	memset(_matrix, 0, sizeof(float) * 16);
	_matrix[0][0] = 1.0f;
	_matrix[0][3] = rhs._x;
	_matrix[1][1] = 1.0f;
	_matrix[1][3] = rhs._y;
	_matrix[2][2] = 1.0f;
	_matrix[2][3] = rhs._z;
	_matrix[3][3] = 1.0f;
}

void SlowMatrix4::CreateFromQuaternion(const SlowQuaternion& q)
{
	float x = q.GetVectorX();
	float y = q.GetVectorY();
	float z = q.GetVectorZ();
	float w = q.GetScalar();

	//rows[0] = _mm_set_ps(1.0f-2.0f*y*y-2.0f*z*z, 2.0f*x*y-2.0f*z*w, 2.0f*x*z+2.0f*y*w, 0.0f);
	//rows[1] = _mm_set_ps(2.0f*x*y+2.0f*z*w, 1.0f-2.0f*x*x-2.0f*z*z, 2.0f*y*z-2.0f*x*w, 0.0f);
	//rows[2] = _mm_set_ps(2.0f*x*z-2.0f*y*w, 2.0f*y*z+2.0f*x*w, 1.0f-2.0f*x*x-2.0f*y*y, 0.0f);
	//rows[3] = _mm_set_ss(1.0f);

	_matrix[0][0] = 1.0f-2.0f*y*y-2.0f*z*z; 
	_matrix[0][1] = 2.0f*x*y-2.0f*z*w;
	_matrix[0][2] = 2.0f*x*z+2.0f*y*w;
	_matrix[0][3] = 0.0f;

	_matrix[1][0] = 2.0f*x*y+2.0f*z*w;
	_matrix[1][1] = 1.0f-2.0f*x*x-2.0f*z*z;
	_matrix[1][2] = 2.0f*y*z-2.0f*x*w;
	_matrix[1][3] = 0.0f;

	_matrix[2][0] = 2.0f*x*z-2.0f*y*w;
	_matrix[2][1] = 2.0f*y*z+2.0f*x*w;
	_matrix[2][2] = 1.0f-2.0f*x*x-2.0f*y*y;
	_matrix[2][3] = 0.0f;

	_matrix[3][0] = 0.0f;
	_matrix[3][1] = 0.0f;
	_matrix[3][2] = 0.0f;
	_matrix[3][3] = 1.0f;
}

void SlowMatrix4::CreateLookAt( const SlowVector3& vEye, const SlowVector3& vAt, const SlowVector3& vUp )
{
	SlowVector3 vFront = vAt;
	vFront.Sub(vEye);
	vFront.Normalize();

	SlowVector3 vLeft = Cross(vUp, vFront);
	vLeft.Normalize();

	SlowVector3 vNewUp = Cross(vFront, vLeft);
	vNewUp.Normalize();

	_matrix[0][0] = vLeft.GetX();
	_matrix[0][1] = vLeft.GetY();
	_matrix[0][2] = vLeft.GetZ();
	_matrix[0][3] = -1.0f * vLeft.Dot(vEye);

	_matrix[1][0] = vNewUp.GetX();
	_matrix[1][1] = vNewUp.GetY();
	_matrix[1][2] = vNewUp.GetZ();
	_matrix[1][3] = -1.0f * vNewUp.Dot(vEye);

	_matrix[2][0] = vFront.GetX();
	_matrix[2][1] = vFront.GetY();
	_matrix[2][2] = vFront.GetZ();
	_matrix[2][3] = -1.0f * vFront.Dot(vEye);

	_matrix[3][0] = 0.0f;
	_matrix[3][1] = 0.0f;
	_matrix[3][2] = 0.0f;
	_matrix[3][3] = 1.0f;
}

void SlowMatrix4::CreatePerspectiveFOV( float fFOVy, float fAspectRatio, float fNear, float fFar )
{
	float fYScale = tanf(1.57079633f - (fFOVy/2)); // cot(x) is the same as tan(pi/2 - x)
	float fXScale = fYScale / fAspectRatio;

	memset(_matrix, 0, sizeof(float) * 16);
	_matrix[0][0] = fXScale;
	_matrix[1][1] = fYScale;
	_matrix[2][2] = fFar/(fFar-fNear);
	_matrix[2][3] = -fNear*fFar/(fFar-fNear);
	_matrix[3][2] = 1.0f;
}

void SlowMatrix4::Invert()
{
	float tmp[12]; /* temp array for pairs */
	float src[16]; /* array of transpose source matrix */
	float dst[16]; /* storage */
	float det; /* determinant */
	/* transpose matrix */

	// row 1 to col 1
	src[0] = _matrix[0][0];
	src[4] = _matrix[0][1];
	src[8] = _matrix[0][2];
	src[12] = _matrix[0][3];

	// row 2 to col 2
	src[1] = _matrix[1][0];
	src[5] = _matrix[1][1];
	src[9] = _matrix[1][2];
	src[13] = _matrix[1][3];

	// row 3 to col 3
	src[2] = _matrix[2][0];
	src[6] = _matrix[2][1];
	src[10] = _matrix[2][2];
	src[14] = _matrix[2][3];

	// row 4 to col 4
	src[3] = _matrix[3][0];
	src[7] = _matrix[3][1];
	src[11] = _matrix[3][2];
	src[15] = _matrix[3][3];

	// 	for (int i = 0; i < 4; i++) {
	// 		src[i] = mat[i*4];
	// 		src[i + 4] = mat[i*4 + 1];
	// 		src[i + 8] = mat[i*4 + 2];
	// 		src[i + 12] = mat[i*4 + 3];
	// 	}
	/* calculate pairs for first 8 elements (cofactors) */
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];
	/* calculate first 8 elements (cofactors) */
	dst[0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
	dst[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
	dst[1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
	dst[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
	dst[2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
	dst[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
	dst[3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
	dst[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
	dst[4] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
	dst[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
	dst[5] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
	dst[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
	dst[6] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
	dst[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
	dst[7] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
	dst[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
	/* calculate pairs for second 8 elements (cofactors) */
	tmp[0] = src[2]*src[7];
	tmp[1] = src[3]*src[6];
	tmp[2] = src[1]*src[7];
	tmp[3] = src[3]*src[5];
	tmp[4] = src[1]*src[6];
	tmp[5] = src[2]*src[5];
	tmp[6] = src[0]*src[7];
	tmp[7] = src[3]*src[4];
	tmp[8] = src[0]*src[6];
	tmp[9] = src[2]*src[4];
	tmp[10] = src[0]*src[5];
	tmp[11] = src[1]*src[4];
	/* calculate second 8 elements (cofactors) */
	dst[8] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
	dst[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
	dst[9] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
	dst[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
	dst[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
	dst[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
	dst[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
	dst[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
	dst[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
	dst[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
	dst[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
	dst[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
	dst[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
	dst[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
	dst[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
	dst[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
	/* calculate determinant */
	det=src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3];
	/* calculate matrix inverse */
	det = 1/det;
	for (int j = 0; j < 16; j++)
		dst[j] *= det;

	// Set it back
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			_matrix[i][j] = dst[i * 4 + j];
		}
	}
}

void SlowVector3::Rotate(const SlowQuaternion& q)
{
	// v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
	SlowVector3 temp(*this);
	temp.Multiply(q.GetScalar());
	temp.Add(Cross(q._qv, *this));

	temp = Cross(q._qv, temp);
	temp.Multiply(2.0f);
	temp.Add(*this);

	_x = temp._x;
	_y = temp._y;
	_z = temp._z;
	_w = temp._w;
}

} // namespace ITP485

// SlowMath.h defines vector, matrix, and quaternion libraries,
// only using scalar floats.
#ifndef _SLOWMATH_H_
#define _SLOWMATH_H_

#include <cmath>
#include <memory.h>

// Define D3D Matrix here so we don't have to replicate
#ifndef D3DMATRIX_DEFINED
typedef struct _D3DMATRIX {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
} D3DMATRIX;
#define D3DMATRIX_DEFINED
#endif

namespace ITP485
{

#ifndef _MATH_PI_CONSTANTS_
#define _MATH_PI_CONSTANTS_
const float TwoPi = 3.1415926535f * 2.0f;
const float Pi = 3.1415926535f;
const float PiOver2 = 3.1415926535f / 2.0f;
const float PiOver4 = 3.1415926535f / 4.0f;
#endif

class SlowMatrix4
{
private:
	union
	{
		float _matrix[4][4];
		D3DMATRIX _d3dm;
	};
public:
	friend class SlowVector3;
	friend class SlowQuaternion;

	__forceinline SlowMatrix4() {}
	__forceinline SlowMatrix4(float mat[4][4])
	{
		memcpy(_matrix, mat, sizeof(float) * 16);
	}

	__forceinline void Set(float mat[4][4])
	{
		memcpy(_matrix, mat, sizeof(float) * 16);
	}

	__forceinline SlowMatrix4(const SlowMatrix4& rhs)
	{
		memcpy(_matrix, rhs._matrix, sizeof(float) * 16);
	}

	__forceinline SlowMatrix4& operator=(const SlowMatrix4& rhs)
	{
		memcpy(_matrix, rhs._matrix, sizeof(float) * 16);
		return *this;
	}

	__forceinline D3DMATRIX* ToD3D()
	{
		/*static D3DMATRIX _d3d_actual;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				_d3d_actual.m[i][j] = _d3dm.m[j][i];
			}
		}*/
		return &_d3dm;
	}

	void Multiply(const SlowMatrix4& rhs)
	{
		float tmp[4][4];
		memcpy(tmp, _matrix, sizeof(float) * 16);

		// row 0
		_matrix[0][0] = tmp[0][0] * rhs._matrix[0][0] + tmp[0][1] * rhs._matrix[1][0] + tmp[0][2] * rhs._matrix[2][0] + tmp[0][3] * rhs._matrix[3][0];
		_matrix[0][1] = tmp[0][0] * rhs._matrix[0][1] + tmp[0][1] * rhs._matrix[1][1] + tmp[0][2] * rhs._matrix[2][1] + tmp[0][3] * rhs._matrix[3][1];
		_matrix[0][2] = tmp[0][0] * rhs._matrix[0][2] + tmp[0][1] * rhs._matrix[1][2] + tmp[0][2] * rhs._matrix[2][2] + tmp[0][3] * rhs._matrix[3][2];
		_matrix[0][3] = tmp[0][0] * rhs._matrix[0][3] + tmp[0][1] * rhs._matrix[1][3] + tmp[0][2] * rhs._matrix[2][3] + tmp[0][3] * rhs._matrix[3][3];

		// row 1
		_matrix[1][0] = tmp[1][0] * rhs._matrix[0][0] + tmp[1][1] * rhs._matrix[1][0] + tmp[1][2] * rhs._matrix[2][0] + tmp[1][3] * rhs._matrix[3][0];
		_matrix[1][1] = tmp[1][0] * rhs._matrix[0][1] + tmp[1][1] * rhs._matrix[1][1] + tmp[1][2] * rhs._matrix[2][1] + tmp[1][3] * rhs._matrix[3][1];
		_matrix[1][2] = tmp[1][0] * rhs._matrix[0][2] + tmp[1][1] * rhs._matrix[1][2] + tmp[1][2] * rhs._matrix[2][2] + tmp[1][3] * rhs._matrix[3][2];
		_matrix[1][3] = tmp[1][0] * rhs._matrix[0][3] + tmp[1][1] * rhs._matrix[1][3] + tmp[1][2] * rhs._matrix[2][3] + tmp[1][3] * rhs._matrix[3][3];

		// row 2
		_matrix[2][0] = tmp[2][0] * rhs._matrix[0][0] + tmp[2][1] * rhs._matrix[1][0] + tmp[2][2] * rhs._matrix[2][0] + tmp[2][3] * rhs._matrix[3][0];
		_matrix[2][1] = tmp[2][0] * rhs._matrix[0][1] + tmp[2][1] * rhs._matrix[1][1] + tmp[2][2] * rhs._matrix[2][1] + tmp[2][3] * rhs._matrix[3][1];
		_matrix[2][2] = tmp[2][0] * rhs._matrix[0][2] + tmp[2][1] * rhs._matrix[1][2] + tmp[2][2] * rhs._matrix[2][2] + tmp[2][3] * rhs._matrix[3][2];
		_matrix[2][3] = tmp[2][0] * rhs._matrix[0][3] + tmp[2][1] * rhs._matrix[1][3] + tmp[2][2] * rhs._matrix[2][3] + tmp[2][3] * rhs._matrix[3][3];

		// row 3
		_matrix[3][0] = tmp[3][0] * rhs._matrix[0][0] + tmp[3][1] * rhs._matrix[1][0] + tmp[3][2] * rhs._matrix[2][0] + tmp[3][3] * rhs._matrix[3][0];
		_matrix[3][1] = tmp[3][0] * rhs._matrix[0][1] + tmp[3][1] * rhs._matrix[1][1] + tmp[3][2] * rhs._matrix[2][1] + tmp[3][3] * rhs._matrix[3][1];
		_matrix[3][2] = tmp[3][0] * rhs._matrix[0][2] + tmp[3][1] * rhs._matrix[1][2] + tmp[3][2] * rhs._matrix[2][2] + tmp[3][3] * rhs._matrix[3][2];
		_matrix[3][3] = tmp[3][0] * rhs._matrix[0][3] + tmp[3][1] * rhs._matrix[1][3] + tmp[3][2] * rhs._matrix[2][3] + tmp[3][3] * rhs._matrix[3][3];
	}

	__forceinline void Add(SlowMatrix4& rhs)
	{
		_matrix[0][0] += rhs._matrix[0][0];
		_matrix[0][1] += rhs._matrix[0][1];
		_matrix[0][2] += rhs._matrix[0][2];
		_matrix[0][3] += rhs._matrix[0][3];

		_matrix[1][0] += rhs._matrix[1][0];
		_matrix[1][1] += rhs._matrix[1][1];
		_matrix[1][2] += rhs._matrix[1][2];
		_matrix[1][3] += rhs._matrix[1][3];

		_matrix[2][0] += rhs._matrix[2][0];
		_matrix[2][1] += rhs._matrix[2][1];
		_matrix[2][2] += rhs._matrix[2][2];
		_matrix[2][3] += rhs._matrix[2][3];

		_matrix[3][0] += rhs._matrix[3][0];
		_matrix[3][1] += rhs._matrix[3][1];
		_matrix[3][2] += rhs._matrix[3][2];
		_matrix[3][3] += rhs._matrix[3][3];
	}

	__forceinline void Sub(SlowMatrix4& rhs)
	{
		_matrix[0][0] -= rhs._matrix[0][0];
		_matrix[0][1] -= rhs._matrix[0][1];
		_matrix[0][2] -= rhs._matrix[0][2];
		_matrix[0][3] -= rhs._matrix[0][3];

		_matrix[1][0] -= rhs._matrix[1][0];
		_matrix[1][1] -= rhs._matrix[1][1];
		_matrix[1][2] -= rhs._matrix[1][2];
		_matrix[1][3] -= rhs._matrix[1][3];

		_matrix[2][0] -= rhs._matrix[2][0];
		_matrix[2][1] -= rhs._matrix[2][1];
		_matrix[2][2] -= rhs._matrix[2][2];
		_matrix[2][3] -= rhs._matrix[2][3];

		_matrix[3][0] -= rhs._matrix[3][0];
		_matrix[3][1] -= rhs._matrix[3][1];
		_matrix[3][2] -= rhs._matrix[3][2];
		_matrix[3][3] -= rhs._matrix[3][3];
	}

	__forceinline void CreateScale(float scale)
	{
		memset(_matrix, 0, sizeof(float) * 16);
		_matrix[0][0] = scale;
		_matrix[1][1] = scale;
		_matrix[2][2] = scale;
		_matrix[3][3] = 1.0f;
	}

	__forceinline void CreateRotationX(float angle)
	{
		memset(_matrix, 0, sizeof(float) * 16);

		float cos_theta = cosf(angle);
		float sin_theta = sinf(angle);

		_matrix[0][0] = 1.0f;
		_matrix[1][1] = cos_theta;
		_matrix[1][2] = sin_theta * -1.0f;
		_matrix[2][1] = sin_theta;
		_matrix[2][2] = cos_theta;
		_matrix[3][3] = 1.0f;
	}

	__forceinline void CreateRotationY(float angle)
	{
		memset(_matrix, 0, sizeof(float) * 16);

		float cos_theta = cosf(angle);
		float sin_theta = sinf(angle);

		_matrix[0][0] = cos_theta;
		_matrix[0][2] = sin_theta;
		_matrix[1][1] = 1.0f;
		_matrix[2][0] = sin_theta * -1.0f;
		_matrix[2][2] = cos_theta;
		_matrix[3][3] = 1.0f;
	}

	__forceinline void CreateRotationZ(float angle)
	{
		memset(_matrix, 0, sizeof(float) * 16);

		float cos_theta = cosf(angle);
		float sin_theta = sinf(angle);

		_matrix[0][0] = cos_theta;
		_matrix[0][1] = sin_theta * -1.0f;
		_matrix[1][0] = sin_theta;
		_matrix[1][1] = cos_theta;
		_matrix[2][2] = 1.0f;
		_matrix[3][3] = 1.0f;
	}

	__forceinline friend SlowMatrix4 Lerp(const SlowMatrix4& a, const SlowMatrix4& b, float f)
	{
		SlowMatrix4 retval;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				retval._matrix[i][j] = b._matrix[i][j] * f + (1.0f - f) * a._matrix[i][j];
			}
		}
		
		return retval;
	}

	// Has to be defined in Slowmath.cpp because of circular dependency
	void CreateTranslation(const SlowVector3& rhs);
	void CreateFromQuaternion(const SlowQuaternion& q);

	// Constructs a Look-At matrix
	// vUp MUST be normalized or bad things will happen
	void CreateLookAt(const SlowVector3& vEye, const SlowVector3& vAt, const SlowVector3& vUp);

	void CreatePerspectiveFOV(float fFOVy, float fAspectRatio, float fNear, float fFar);

	// Inverts the matrix
	void Invert();

	static const SlowMatrix4 Identity;
};

class SlowVector3
{
private:
	float _x, _y, _z, _w;
public:
	__forceinline SlowVector3() {}
	__forceinline SlowVector3(float x, float y, float z)
	{
		_x = x;
		_y = y;
		_z = z;
		_w = 1.0f;
	}

	__forceinline SlowVector3(const SlowVector3& rhs)
	{
		_x = rhs._x;
		_y = rhs._y;
		_z = rhs._z;
	}

	__forceinline SlowVector3& operator=(const SlowVector3& rhs)
	{
		_x = rhs._x;
		_y = rhs._y;
		_z = rhs._z;
		return *this;
	}

	__forceinline float GetX() const
	{
		return _x;
	}
	
	__forceinline float GetY() const
	{
		return _y;
	}
	
	__forceinline float GetZ() const
	{
		return _z;
	}

	__forceinline float GetW() const
	{
		return _w;
	}
	
	__forceinline void Set(float x, float y, float z)
	{
		_x = x;
		_y = y;
		_z = z;
		_w = 1.0f;
	}

	__forceinline void SetX(float x)
	{
		_x = x;
	}
	
	__forceinline void SetY(float y)
	{
		_y = y;
	}
	
	__forceinline void SetZ(float z)
	{
		_z = z;
	}

	__forceinline float Dot(const SlowVector3& rhs) const
	{
		return (_x * rhs._x + _y * rhs._y + _z * rhs._z);
	}

	__forceinline void Add(const SlowVector3& rhs)
	{
		_x += rhs._x;
		_y += rhs._y;
		_z += rhs._z;
	}

	__forceinline void Sub(const SlowVector3& rhs)
	{
		_x -= rhs._x;
		_y -= rhs._y;
		_z -= rhs._z;
	}

	__forceinline void Multiply(float scalar)
	{
		_x *= scalar;
		_y *= scalar;
		_z *= scalar;
	}

	__forceinline void Normalize()
	{
		float temp = _x * _x + _y * _y + _z * _z;
		temp = 1.0f / sqrtf(temp);

		_x *= temp;
		_y *= temp;
		_z *= temp;
	}

	__forceinline float LengthSquared() const
	{
		return (_x * _x + _y * _y + _z * _z);
	}

	__forceinline float Length() const
	{
		float lengthsquared = _x * _x + _y * _y + _z * _z;
		return sqrtf(lengthsquared);
	}

	__forceinline friend SlowVector3 Cross(const SlowVector3& lhs, const SlowVector3& rhs)
	{
		// Cx = Ay * Bz - Az * B.y
		// Cy = Az * Bx - Ax * Bz
		// Cz = Ax * By - Ay * Bx
		SlowVector3 result;
		result._x = lhs._y * rhs._z - lhs._z * rhs._y;
		result._y = lhs._z * rhs._x - lhs._x * rhs._z;
		result._z = lhs._x * rhs._y - lhs._y * rhs._x;
		return result;
	}

	__forceinline friend SlowVector3 Lerp(const SlowVector3 &a, const SlowVector3 &b, float f)
	{
		SlowVector3(result);
		result._x = b._x * f;
		result._y = b._y * f;
		result._z = b._z * f;

		f = 1.0f - f;
		result._x += a._x * f;
		result._y += a._y * f;
		result._z += a._z * f;

		return result;
	}

	// does a 4-way blend
	// result = a * fa + b * fb + c * fc + d * fd
	// fd = 1.0f - fa - fb - fc
	__forceinline friend SlowVector3 Blend(const SlowVector3& a, const SlowVector3& b, const SlowVector3& c, const SlowVector3& d,
		float fa, float fb, float fc)
	{
		SlowVector3 result;
		float fd = 1.0f - fa - fb - fc;
		result._x = a._x * fa + b._x * fb + c._x * fc + d._x * fd;
		result._y = a._y * fa + b._y * fb + c._y * fc + d._y * fd;
		result._z = a._z * fa + b._z * fb + c._z * fc + d._z * fd;
		return result;
	}

	__forceinline void Transform(SlowMatrix4 &mat)
	{
		// set W component to 1.0f before we do anything
		_w = 1.0f;
		
		float new_x = mat._matrix[0][0] * _x + mat._matrix[0][1] * _y + mat._matrix[0][2] * _z + mat._matrix[0][3] * _w;
		float new_y = mat._matrix[1][0] * _x + mat._matrix[1][1] * _y + mat._matrix[1][2] * _z + mat._matrix[1][3] * _w;
		float new_z = mat._matrix[2][0] * _x + mat._matrix[2][1] * _y + mat._matrix[2][2] * _z + mat._matrix[2][3] * _w;
		float new_w = mat._matrix[3][0] * _x + mat._matrix[3][1] * _y + mat._matrix[3][2] * _z + mat._matrix[3][3] * _w;

		_x = new_x;
		_y = new_y;
		_z = new_z;
		_w = new_w;
	}

	void Rotate(const SlowQuaternion& q);

	__forceinline void TransformAsVector(SlowMatrix4 &mat)
	{
		// set W component to 0.0f before we do anything
		_w = 0.0f;

		float new_x = mat._matrix[0][0] * _x + mat._matrix[0][1] * _y + mat._matrix[0][2] * _z + mat._matrix[0][3] * _w;
		float new_y = mat._matrix[1][0] * _x + mat._matrix[1][1] * _y + mat._matrix[1][2] * _z + mat._matrix[1][3] * _w;
		float new_z = mat._matrix[2][0] * _x + mat._matrix[2][1] * _y + mat._matrix[2][2] * _z + mat._matrix[2][3] * _w;
		float new_w = mat._matrix[3][0] * _x + mat._matrix[3][1] * _y + mat._matrix[3][2] * _z + mat._matrix[3][3] * _w;

		_x = new_x;
		_y = new_y;
		_z = new_z;
		_w = new_w;
	}

	friend class SlowMatrix4;
	friend class SlowQuaternion;

	static const SlowVector3 Zero;
	static const SlowVector3 UnitX;
	static const SlowVector3 UnitY;
	static const SlowVector3 UnitZ;
	static const SlowVector3 NegativeUnitX;
	static const SlowVector3 NegativeUnitY;
	static const SlowVector3 NegativeUnitZ;
};

class SlowQuaternion
{
private:
	SlowVector3 _qv;
	float _qs;
public:
	SlowQuaternion() {}
	SlowQuaternion(const SlowVector3& axis, float angle):
		_qv(axis)
	{
		float sinangleover2 = sinf(angle / 2.0f);
		_qv.Multiply(sinangleover2);
		_qs = cosf(angle / 2.0f);
	}

	__forceinline SlowQuaternion(float qv_x, float qv_y, float qv_z, float qs)
	{
		_qv.Set(qv_x, qv_y, qv_z);
		_qs = qs;
	}

	__forceinline void Set(float qv_x, float qv_y, float qv_z, float qs)
	{
		_qv.Set(qv_x, qv_y, qv_z);
		_qs = qs;
	}

	__forceinline SlowQuaternion(const SlowQuaternion& rhs)
	{
		_qv = rhs._qv;
		_qs = rhs._qs;
	}

	__forceinline SlowQuaternion& operator=(const SlowQuaternion& rhs)
	{
		_qv = rhs._qv;
		_qs = rhs._qs;
		return *this;
	}

	__forceinline float GetVectorX() const
	{
		return _qv.GetX();
	}

	__forceinline float GetVectorY() const
	{
		return _qv.GetY();
	}

	__forceinline float GetVectorZ() const
	{
		return _qv.GetZ();
	}

	__forceinline float GetScalar() const
	{
		return _qs;
	}

	// Rotate by THIS quaternion, followed by rhs
	// Or in other words, p = rhs, q = this
	void Multiply(const SlowQuaternion& rhs)
	{
		// vector component is:
		// ps * qv + qs * pv + pv x qv
		SlowVector3 new_qv(_qv);
		new_qv.Multiply(rhs._qs);

		SlowVector3 temp(rhs._qv);
		temp.Multiply(_qs);

		new_qv.Add(temp);
		new_qv.Add(Cross(rhs._qv, _qv));

		// scalar component is ps * qs - pv . qv
		_qs = rhs._qs * _qs - rhs._qv.Dot(_qv);
		_qv = new_qv;
	}

	__forceinline void Conjugate()
	{
		_qv.Multiply(-1.0f);
	}

	__forceinline float LengthSquared() const
	{
		return (_qv.Dot(_qv) + _qs * _qs);
	}

	__forceinline float Length() const
	{
		float lsquared = _qv.Dot(_qv) + _qs * _qs;
		return sqrtf(lsquared);
	}

	__forceinline void Normalize()
	{
		float length = sqrtf(_qv.Dot(_qv) + _qs * _qs);
		_qv.Multiply(1.0f / length);
		_qs /= length;
	}

	__forceinline friend SlowQuaternion Lerp(const SlowQuaternion& a, const SlowQuaternion& b, float f)
	{
		SlowVector3 new_qv = Lerp(a._qv, b._qv, f);
		float new_qs = a._qs * (1.0f - f) + b._qs * f;
		
		SlowQuaternion result(new_qv.GetX(), new_qv.GetY(), new_qv.GetZ(), new_qs);
		result.Normalize();
		
		return result;
	}

	// does a 4-way blend, normalizing the result
	// result = a * fa + b * fb + c * fc + d * fd
	// fd = 1.0f - fa - fb - fc
	__forceinline friend SlowQuaternion Blend(const SlowQuaternion& a, const SlowQuaternion& b, 
											  const SlowQuaternion& c, const SlowQuaternion& d,
											  float fa, float fb, float fc)
	{
		float fd = 1.0f - fa - fb - fc;
		float qv_x = a._qv.GetX() * fa + b._qv.GetX() * fb + c._qv.GetX() * fc + d._qv.GetX() * fd;
		float qv_y = a._qv.GetY() * fa + b._qv.GetY() * fb + c._qv.GetY() * fc + d._qv.GetY() * fd;
		float qv_z = a._qv.GetZ() * fa + b._qv.GetZ() * fb + c._qv.GetZ() * fc + d._qv.GetZ() * fd;
		float qs = a._qs * fa + b._qs * fb + c._qs * fc + d._qs * fd;

		SlowQuaternion result(qv_x, qv_y, qv_z, qs);
		result.Normalize();
		return result;
	}

	friend class SlowVector3;
	friend class SlowMatrix4;

	static const SlowQuaternion Identity;
};

} // namespace ITP485
#endif // _SLOWMATH_H_

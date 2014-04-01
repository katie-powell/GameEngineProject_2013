#ifndef _UNITTESTS_HPP_
#define _UNITTESTS_HPP_
#include "stdafx.h"
#include <iostream>
#include "..\core\fastmath.h"
#include "..\core\slowmath.h"
#include "..\MiniCppUnit-2.5\MiniCppUnit.hxx"
#include "..\core\singleton.h"
#include "..\core\poolalloc.h"
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

namespace ITP485
{

class FastVector3Test : public TestFixture<FastVector3Test>
{
public:
	void PrintVector3(const FastVector3& data)
	{
		printf("<%f, %f, %f>\n", data.GetX(), data.GetY(), data.GetZ());
	}

	TEST_FIXTURE_DESCRIBE(FastVector3Test, "Testing FastVector3...")
	{
		//TEST_CASE_DESCRIBE(testConstructorGettersSetters, "Getters/Setters");
		TEST_CASE_DESCRIBE(testNormalize, "Normalize");
		TEST_CASE_DESCRIBE(testDotProduct, "Dot Product");
		TEST_CASE_DESCRIBE(testAdd, "Addition");
		TEST_CASE_DESCRIBE(testSub, "Subtraction");
		TEST_CASE_DESCRIBE(testLengthSquared, "Length Squared");
		TEST_CASE_DESCRIBE(testLength, "Length");
		TEST_CASE_DESCRIBE(testCross, "Cross Product");
		TEST_CASE_DESCRIBE(testLerp, "Linear Interpolation");
		//TEST_CASE_DESCRIBE(testScalarMult, "Scalar Multiply");
		TEST_CASE_DESCRIBE(testBlend, "4-Way Blend");
	}
	void testConstructorGettersSetters()
	{
		FastVector3 v(1.0f, 2.0f, 3.0f);
		ASSERT_EQUALS(1.0f, v.GetX());
		ASSERT_EQUALS(2.0f, v.GetY());
		ASSERT_EQUALS(3.0f, v.GetZ());

		v.SetX(2.0f);
		v.SetY(4.0f);
		v.SetZ(6.0f);
		ASSERT_EQUALS(2.0f, v.GetX());
		ASSERT_EQUALS(4.0f, v.GetY());
		ASSERT_EQUALS(6.0f, v.GetZ());

		v.Set(3.0f, 5.0f, 7.0f);
		ASSERT_EQUALS(3.0f, v.GetX());
		ASSERT_EQUALS(5.0f, v.GetY());
		ASSERT_EQUALS(7.0f, v.GetZ());
	}
	void testNormalize()
	{
		FastVector3 v(0.0f, 3.0f, 4.0f);
		v.Normalize();
		ASSERT_EQUALS_EPSILON(0.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.6f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.8f, v.GetZ(), 0.01f);
	}
	void testDotProduct()
	{
		FastVector3 v(2.0f, 4.0f, 6.0f);
		ASSERT_EQUALS_EPSILON(56.0f, v.Dot(v), 0.01f);
	}
	void testAdd()
	{
		FastVector3 v1(1.0f, 2.0f, 3.0f);
		FastVector3 v2(2.0f, 4.0f, 6.0f);
		v1.Add(v2);
		ASSERT_EQUALS_EPSILON(3.0f, v1.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(6.0f, v1.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(9.0f, v1.GetZ(), 0.01f);
	}
	void testSub()
	{
		FastVector3 v1(1.0f, 2.0f, 3.0f);
		FastVector3 v2(2.0f, 4.0f, 6.0f);
		v1.Sub(v2);
		ASSERT_EQUALS_EPSILON(-1.0f, v1.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(-2.0f, v1.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(-3.0f, v1.GetZ(), 0.01f);
	}
	void testLengthSquared()
	{
		FastVector3 v(3.0f, 1.0f, 4.0f);
		ASSERT_EQUALS_EPSILON(26.0f, v.LengthSquared(), 0.01f);
	}
	void testLength()
	{
		FastVector3 v(3.0f, 0.0f, 4.0f);
		ASSERT_EQUALS_EPSILON(5.0f, v.Length(), 0.01f);
	}
	void testCross()
	{
		FastVector3 v1(3.0f, 2.0f, 5.0f);
		FastVector3 v2(4.0f, 6.0f, 1.0f);
		FastVector3 v3 = Cross(v1, v2);
		ASSERT_EQUALS_EPSILON(-28.0f, v3.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(17.0f, v3.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(10.0f, v3.GetZ(), 0.01f);
	}
	void testLerp()
	{
		FastVector3 v1(1.0f, 10.0f, 100.0f);
		FastVector3 v2(0.0f, 0.0f, 0.0f);
		FastVector3 v3 = Lerp(v1, v2, 0.75f);
		ASSERT_EQUALS_EPSILON(0.25f, v3.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(2.5f, v3.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(25.0f, v3.GetZ(), 0.01f);
	}
	void testScalarMult()
	{
		FastVector3 v(1.0f, 1.0f, 1.0f);
		v.Multiply(3.0f);
		ASSERT_EQUALS_EPSILON(3.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(3.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(3.0f, v.GetZ(), 0.01f);
	}
	void testBlend()
	{
		FastVector3 v1(1.0f, 1.0f, 1.0f);
		FastVector3 v2(4.0f, 4.0f, 4.0f);
		FastVector3 v3(8.0f, 8.0f, 8.0f);
		FastVector3 v4(16.0f, 16.0f, 16.0f);

		FastVector3 vBlendResult = Blend(v1, v2, v3, v4, 0.25f, 0.25f, 0.25f);
		ASSERT_EQUALS_EPSILON(7.25f, vBlendResult.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(7.25f, vBlendResult.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(7.25f, vBlendResult.GetZ(), 0.01f);
	}
};

class FastMatrix4Test : public TestFixture<FastMatrix4Test>
{
public:
	void PrintVector3(const FastVector3& data)
	{
		printf("<%f, %f, %f>\n", data.GetX(), data.GetY(), data.GetZ());
	}

	TEST_FIXTURE_DESCRIBE(FastMatrix4Test, "Testing FastMatrix4...")
	{
		TEST_CASE_DESCRIBE(testVectorTransform, "Transform a vector (as a position) by a matrix");
		TEST_CASE_DESCRIBE(testVectorTransformX2, "Transform a vector (as a position) by a matrix and back");
		TEST_CASE_DESCRIBE(testVectorTransformAsVector, "Transform a vector (as a vector) by a matrix");
		TEST_CASE_DESCRIBE(testMatrixMult, "Multiply two matrices together (then apply to vector)");
// 		TEST_CASE_DESCRIBE(testMatrixAdd, "Add two matrices together (then apply to vector)");
// 		TEST_CASE_DESCRIBE(testMatrixSub, "Subtract two matrices together (then apply to vector)");
// 		TEST_CASE_DESCRIBE(testMatrixScale, "Creates scale matrix (then apply to vector)");
// 		TEST_CASE_DESCRIBE(testMatrixRotateX, "Creates rotate X matrix (then apply to vector)");
// 		TEST_CASE_DESCRIBE(testMatrixRotateY, "Creates rotate Y matrix (then apply to vector)");
// 		TEST_CASE_DESCRIBE(testMatrixRotateZ, "Creates rotate Z matrix (then apply to vector)");
// 		TEST_CASE_DESCRIBE(testMatrixTranslate, "Creates translate matrix (then apply to vector)");
//		TEST_CASE_DESCRIBE(testMatrixInvert, "Inverts a matrix");
	}
	void testVectorTransform()
	{
		FastVector3 v(FastVector3::Zero);
		float trans[4][4] = {1.0f, 0.0f, 0.0f, -5.0f,
			0.0f, 1.0f, 0.0f, -10.0f,
			0.0f, 0.0f, 1.0f, 4.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		FastMatrix4 translate(trans);

		v.Transform(translate);
		ASSERT_EQUALS_EPSILON(-5.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(-10.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(4.0f, v.GetZ(), 0.01f);
	}
	void testVectorTransformX2()
	{
		FastVector3 v(FastVector3::Zero);
		float trans[4][4] = {1.0f, 0.0f, 0.0f, -5.0f,
			0.0f, 1.0f, 0.0f, -10.0f,
			0.0f, 0.0f, 1.0f, 4.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		FastMatrix4 translate(trans);
		v.Transform(translate);

		ASSERT_EQUALS_EPSILON(-5.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(-10.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(4.0f, v.GetZ(), 0.01f);

		float trans2[4][4] = {1.0f, 0.0f, 0.0f, 5.0f,
			0.0f, 1.0f, 0.0f, 10.0f,
			0.0f, 0.0f, 1.0f, -4.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		FastMatrix4 translate2(trans2);
		v.Transform(translate2);

		ASSERT_EQUALS_EPSILON(0.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, v.GetZ(), 0.01f);
	}
	void testVectorTransformAsVector()
	{
		FastVector3 v(1.0f, 1.0f, 1.0f);
		float trans[4][4] = {2.0f, 0.0f, 0.0f, -5.0f,
			0.0f, 2.0f, 0.0f, -10.0f,
			0.0f, 0.0f, 2.0f, 4.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		FastMatrix4 translate(trans);

		v.TransformAsVector(translate);
		ASSERT_EQUALS_EPSILON(2.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(2.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(2.0f, v.GetZ(), 0.01f);
	}
	void testMatrixMult()
	{
		FastVector3 v(1.0f, 1.0f, 1.0f);

		float trans[4][4] = {3.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 3.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 3.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		FastMatrix4 world_transform(trans);

		float trans2[4][4] = {1.0f, 0.0f, 0.0f, 5.0f,
			0.0f, 1.0f, 0.0f, 10.0f,
			0.0f, 0.0f, 1.0f, -4.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		FastMatrix4 translate(trans2);

		world_transform.Multiply(translate);

		v.Transform(world_transform);

		ASSERT_EQUALS_EPSILON(18.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(33.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(-9.0f, v.GetZ(), 0.01f);
	}
	void testMatrixAdd()
	{
		FastVector3 v(1.0f, 1.0f, 1.0f);

		float trans[4][4] = {3.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 3.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 3.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		FastMatrix4 world_transform(trans);

		float trans2[4][4] = {1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		FastMatrix4 translate(trans2);

		world_transform.Add(translate);

		v.Transform(world_transform);

		ASSERT_EQUALS_EPSILON(4.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(4.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(4.0f, v.GetZ(), 0.01f);
	}
	void testMatrixSub()
	{
		FastVector3 v(1.0f, 1.0f, 1.0f);

		float trans[4][4] = {3.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 3.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 3.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		FastMatrix4 world_transform(trans);

		float trans2[4][4] = {1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		FastMatrix4 translate(trans2);

		world_transform.Sub(translate);

		v.Transform(world_transform);

		ASSERT_EQUALS_EPSILON(2.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(2.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(2.0f, v.GetZ(), 0.01f);
	}
	void testMatrixScale()
	{
		FastMatrix4 scale;
		scale.CreateScale(3.0f);
		FastVector3 v(1.0f, 1.0f, 1.0f);
		v.Transform(scale);

		ASSERT_EQUALS_EPSILON(3.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(3.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(3.0f, v.GetZ(), 0.01f);
	}
	void testMatrixRotateX()
	{
		FastMatrix4 rot;
		rot.CreateRotationX(PiOver2);
		FastVector3 v(FastVector3::UnitY);
		v.Transform(rot);

		ASSERT_EQUALS_EPSILON(0.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(1.0f, v.GetZ(), 0.01f);
	}
	void testMatrixRotateY()
	{
		FastMatrix4 rot;
		rot.CreateRotationY(PiOver2);
		FastVector3 v(FastVector3::UnitX);
		v.Transform(rot);

		ASSERT_EQUALS_EPSILON(0.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(-1.0f, v.GetZ(), 0.01f);
	}
	void testMatrixRotateZ()
	{
		FastMatrix4 rot;
		rot.CreateRotationZ(PiOver2);
		FastVector3 v(FastVector3::UnitX);
		v.Transform(rot);

		ASSERT_EQUALS_EPSILON(0.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(1.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, v.GetZ(), 0.01f);
	}
	void testMatrixTranslate()
	{
		FastMatrix4 trans;
		trans.CreateTranslation(FastVector3(5.0f, 10.0f, -4.0f));
		FastVector3 v(FastVector3::Zero);
		v.Transform(trans);

		ASSERT_EQUALS_EPSILON(5.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(10.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(-4.0f, v.GetZ(), 0.01f);
	}
	void testMatrixInvert()
	{
		FastMatrix4 trans;
		trans.CreateTranslation(FastVector3(1.0f, 2.0f, -3.0f));

		trans.Invert();
		FastVector3 v(FastVector3::Zero);
		v.Transform(trans);

		ASSERT_EQUALS_EPSILON(-1.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(-2.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(3.0f, v.GetZ(), 0.01f);
	}
};

class FastQuaternionTest : public TestFixture<FastQuaternionTest>
{
public:
	void PrintVector3(const FastVector3& data)
	{
		printf("<%f, %f, %f>\n", data.GetX(), data.GetY(), data.GetZ());
	}
	void PrintQuaternion(const FastQuaternion& data)
	{
		printf("qv = <%f, %f, %f>, qs = %f\n", data.GetVectorX(), data.GetVectorY(), data.GetVectorZ(), data.GetScalar());
	}
	TEST_FIXTURE_DESCRIBE(FastQuaternionTest, "Testing FastQuaternion...")
	{
		TEST_CASE_DESCRIBE(testConstructor, "Passing in axis/angle results in correct quaternion");
		TEST_CASE_DESCRIBE(testMultiplication, "Multiply two quaternions together");
		TEST_CASE_DESCRIBE(testConjugate, "Conjugate");
		TEST_CASE_DESCRIBE(testLength, "Length");
		TEST_CASE_DESCRIBE(testLengthSquared, "Length Squared");
		TEST_CASE_DESCRIBE(testNormalize, "Normalize");
		TEST_CASE_DESCRIBE(testRotate, "Rotate a vector by a quaternion");
		TEST_CASE_DESCRIBE(testMatrix, "Create a matrix from a quaternion, and apply to vector");
		TEST_CASE_DESCRIBE(testLerp, "Lerp");
		TEST_CASE_DESCRIBE(testBlend, "4-Way Blend");
	}
	void testConstructor()
	{
		FastVector3 axis(1.0f, 0.0f, 0.0f);
		
		FastQuaternion rot(axis, PiOver4);

		ASSERT_EQUALS_EPSILON(0.383f, rot.GetVectorX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, rot.GetVectorY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, rot.GetVectorZ(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.924f, rot.GetScalar(), 0.01f);
	}
	void testMultiplication()
	{
		FastVector3 axis(1.0f, 0.0f, 0.0f);
		FastQuaternion q(axis, PiOver4);
		FastQuaternion p(axis, PiOver4);

		q.Multiply(p);
		
		ASSERT_EQUALS_EPSILON(0.707f, q.GetVectorX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, q.GetVectorY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, q.GetVectorZ(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.707f, q.GetScalar(), 0.01f);
	}
	void testConjugate()
	{
		FastVector3 axis(1.0f, 1.0f, 1.0f);
		axis.Normalize();
		FastQuaternion q(axis, PiOver4);
		q.Conjugate();

		ASSERT_EQUALS_EPSILON(-0.221f, q.GetVectorX(), 0.01f);
		ASSERT_EQUALS_EPSILON(-0.221f, q.GetVectorY(), 0.01f);
		ASSERT_EQUALS_EPSILON(-0.221f, q.GetVectorZ(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.9238f, q.GetScalar(), 0.01f);
	}
	void testLength()
	{
		FastVector3 axis(1.0f, 1.0f, 1.0f);
		axis.Normalize();
		FastQuaternion q(axis, PiOver4);

		ASSERT_EQUALS_EPSILON(1.0f, q.Length(), 0.01f);
	}
	void testLengthSquared()
	{
		FastQuaternion q(1.0f, 2.0f, 3.0f, 4.0f);

		ASSERT_EQUALS_EPSILON(30.0f, q.LengthSquared(), 0.01f);
	}
	void testNormalize()
	{
		FastQuaternion q(1.0f, 2.0f, 3.0f, 4.0f);
		q.Normalize();

		ASSERT_EQUALS_EPSILON(0.183f, q.GetVectorX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.365f, q.GetVectorY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.548f, q.GetVectorZ(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.730f, q.GetScalar(), 0.01f);
	}
	void testRotate()
	{
		FastVector3 axis(1.0f, 0.0f, 0.0f);
		FastQuaternion q(axis, PiOver2);
		FastVector3 v(FastVector3::UnitY);
		
		v.Rotate(q);
		
		ASSERT_EQUALS_EPSILON(0.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(1.0f, v.GetZ(), 0.01f);
	}
	void testMatrix()
	{
		FastVector3 axis(1.0f, 0.0f, 0.0f);
		FastQuaternion q(axis, Pi / 6.0f);
		FastVector3 v(FastVector3::UnitY);
		FastMatrix4 m;
		
		m.CreateFromQuaternion(q);
		v.Transform(m);

		ASSERT_EQUALS_EPSILON(0.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.866f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.5f, v.GetZ(), 0.01f);
	}
	void testLerp()
	{
		FastQuaternion q1(0.707107f, 0.0f, 0.0f, 0.707107f);
		FastQuaternion q2(-0.707107f, 0.0f, 0.0f, 0.707107f);

		FastQuaternion q_result = Lerp(q1, q2, 0.5f);
		
		ASSERT_EQUALS_EPSILON(0.0f, q_result.GetVectorX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, q_result.GetVectorY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, q_result.GetVectorZ(), 0.01f);
		ASSERT_EQUALS_EPSILON(1.0f, q_result.GetScalar(), 0.01f);
	}
	void testBlend()
	{
		FastQuaternion q1(1.0f, 1.0f, 1.0f, 1.0f);
		FastQuaternion q2(4.0f, 4.0f, 4.0f, 4.0f);
		FastQuaternion q3(8.0f, 8.0f, 8.0f, 8.0f);
		FastQuaternion q4(16.0f, 16.0f, 16.0f, 16.0f);

		FastQuaternion qBlendResult = Blend(q1, q2, q3, q4, 0.25f, 0.25f, 0.25f);
		ASSERT_EQUALS_EPSILON(0.499f, qBlendResult.GetVectorX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.499f, qBlendResult.GetVectorY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.499f, qBlendResult.GetVectorZ(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.499f, qBlendResult.GetScalar(), 0.01f);
	}
};

class SlowVector3Test : public TestFixture<SlowVector3Test>
{
public:
	void PrintVector3(const SlowVector3& data)
	{
		printf("<%f, %f, %f>\n", data.GetX(), data.GetY(), data.GetZ());
	}

	TEST_FIXTURE_DESCRIBE(SlowVector3Test, "Testing SlowVector3...")
	{
		TEST_CASE_DESCRIBE(testConstructorGettersSetters, "Getters/Setters");
		TEST_CASE_DESCRIBE(testNormalize, "Normalize");
		TEST_CASE_DESCRIBE(testDotProduct, "Dot Product");
		TEST_CASE_DESCRIBE(testAdd, "Addition");
		TEST_CASE_DESCRIBE(testSub, "Subtraction");
		TEST_CASE_DESCRIBE(testLengthSquared, "Length Squared");
		TEST_CASE_DESCRIBE(testLength, "Length");
		TEST_CASE_DESCRIBE(testCross, "Cross Product");
		TEST_CASE_DESCRIBE(testLerp, "Linear Interpolation");
		TEST_CASE_DESCRIBE(testBlend, "4-Way Blend");
	}
	void testConstructorGettersSetters()
	{
		SlowVector3 v(1.0f, 2.0f, 3.0f);
		ASSERT_EQUALS(1.0f, v.GetX());
		ASSERT_EQUALS(2.0f, v.GetY());
		ASSERT_EQUALS(3.0f, v.GetZ());

		v.SetX(2.0f);
		v.SetY(4.0f);
		v.SetZ(6.0f);
		ASSERT_EQUALS(2.0f, v.GetX());
		ASSERT_EQUALS(4.0f, v.GetY());
		ASSERT_EQUALS(6.0f, v.GetZ());

		v.Set(3.0f, 5.0f, 7.0f);
		ASSERT_EQUALS(3.0f, v.GetX());
		ASSERT_EQUALS(5.0f, v.GetY());
		ASSERT_EQUALS(7.0f, v.GetZ());
	}
	void testNormalize()
	{
		SlowVector3 v(0.0f, 3.0f, 4.0f);
		v.Normalize();
		ASSERT_EQUALS_EPSILON(0.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.6f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.8f, v.GetZ(), 0.01f);
	}
	void testDotProduct()
	{
		SlowVector3 v(2.0f, 4.0f, 6.0f);
		ASSERT_EQUALS_EPSILON(56.0f, v.Dot(v), 0.01f);
	}
	void testAdd()
	{
		SlowVector3 v1(1.0f, 2.0f, 3.0f);
		SlowVector3 v2(2.0f, 4.0f, 6.0f);
		v1.Add(v2);
		ASSERT_EQUALS_EPSILON(3.0f, v1.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(6.0f, v1.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(9.0f, v1.GetZ(), 0.01f);
	}
	void testSub()
	{
		SlowVector3 v1(1.0f, 2.0f, 3.0f);
		SlowVector3 v2(2.0f, 4.0f, 6.0f);
		v1.Sub(v2);
		ASSERT_EQUALS_EPSILON(-1.0f, v1.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(-2.0f, v1.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(-3.0f, v1.GetZ(), 0.01f);
	}
	void testLengthSquared()
	{
		SlowVector3 v(3.0f, 1.0f, 4.0f);
		ASSERT_EQUALS_EPSILON(26.0f, v.LengthSquared(), 0.01f);
	}
	void testLength()
	{
		SlowVector3 v(3.0f, 0.0f, 4.0f);
		ASSERT_EQUALS_EPSILON(5.0f, v.Length(), 0.01f);
	}
	void testCross()
	{
		SlowVector3 v1(3.0f, 2.0f, 5.0f);
		SlowVector3 v2(4.0f, 6.0f, 1.0f);
		SlowVector3 v3 = Cross(v1, v2);
		ASSERT_EQUALS_EPSILON(-28.0f, v3.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(17.0f, v3.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(10.0f, v3.GetZ(), 0.01f);
	}
	void testLerp()
	{
		SlowVector3 v1(1.0f, 10.0f, 100.0f);
		SlowVector3 v2(0.0f, 0.0f, 0.0f);
		SlowVector3 v3 = Lerp(v1, v2, 0.75f);
		ASSERT_EQUALS_EPSILON(0.25f, v3.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(2.5f, v3.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(25.0f, v3.GetZ(), 0.01f);
	}
	void testBlend()
	{
		SlowVector3 v1(1.0f, 1.0f, 1.0f);
		SlowVector3 v2(4.0f, 4.0f, 4.0f);
		SlowVector3 v3(8.0f, 8.0f, 8.0f);
		SlowVector3 v4(16.0f, 16.0f, 16.0f);

		SlowVector3 vBlendResult = Blend(v1, v2, v3, v4, 0.25f, 0.25f, 0.25f);
		ASSERT_EQUALS_EPSILON(7.25f, vBlendResult.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(7.25f, vBlendResult.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(7.25f, vBlendResult.GetZ(), 0.01f);
	}
};

class SlowMatrix4Test : public TestFixture<SlowMatrix4Test>
{
public:
	void PrintVector3(const SlowVector3& data)
	{
		printf("<%f, %f, %f>\n", data.GetX(), data.GetY(), data.GetZ());
	}

	TEST_FIXTURE_DESCRIBE(SlowMatrix4Test, "Testing SlowMatrix4...")
	{
		TEST_CASE_DESCRIBE(testVectorTransform, "Transform a vector (as a position) by a matrix");
		TEST_CASE_DESCRIBE(testVectorTransformX2, "Transform a vector (as a position) by a matrix and back");
		TEST_CASE_DESCRIBE(testVectorTransformAsVector, "Transform a vector (as a vector) by a matrix");
		TEST_CASE_DESCRIBE(testMatrixMult, "Multiply two matrices together (then apply to vector)");
		TEST_CASE_DESCRIBE(testMatrixAdd, "Add two matrices together (then apply to vector)");
		TEST_CASE_DESCRIBE(testMatrixSub, "Subtract two matrices together (then apply to vector)");
		TEST_CASE_DESCRIBE(testMatrixScale, "Creates scale matrix (then apply to vector)");
		TEST_CASE_DESCRIBE(testMatrixRotateX, "Creates rotate X matrix (then apply to vector)");
		TEST_CASE_DESCRIBE(testMatrixRotateY, "Creates rotate Y matrix (then apply to vector)");
		TEST_CASE_DESCRIBE(testMatrixRotateZ, "Creates rotate Z matrix (then apply to vector)");
		TEST_CASE_DESCRIBE(testMatrixTranslate, "Creates translate matrix (then apply to vector)");
		TEST_CASE_DESCRIBE(testMatrixInvert, "Inverts the matrix");
	}
	void testVectorTransform()
	{
		SlowVector3 v(SlowVector3::Zero);
		float trans[4][4] = {1.0f, 0.0f, 0.0f, -5.0f,
			0.0f, 1.0f, 0.0f, -10.0f,
			0.0f, 0.0f, 1.0f, 4.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		SlowMatrix4 translate(trans);

		v.Transform(translate);
		ASSERT_EQUALS_EPSILON(-5.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(-10.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(4.0f, v.GetZ(), 0.01f);
	}
	void testVectorTransformX2()
	{
		SlowVector3 v(SlowVector3::Zero);
		float trans[4][4] = {1.0f, 0.0f, 0.0f, -5.0f,
			0.0f, 1.0f, 0.0f, -10.0f,
			0.0f, 0.0f, 1.0f, 4.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		SlowMatrix4 translate(trans);
		v.Transform(translate);

		float trans2[4][4] = {1.0f, 0.0f, 0.0f, 5.0f,
			0.0f, 1.0f, 0.0f, 10.0f,
			0.0f, 0.0f, 1.0f, -4.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		SlowMatrix4 translate2(trans2);
		v.Transform(translate2);

		ASSERT_EQUALS_EPSILON(0.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, v.GetZ(), 0.01f);
	}
	void testVectorTransformAsVector()
	{
		SlowVector3 v(1.0f, 1.0f, 1.0f);
		float trans[4][4] = {2.0f, 0.0f, 0.0f, -5.0f,
			0.0f, 2.0f, 0.0f, -10.0f,
			0.0f, 0.0f, 2.0f, 4.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		SlowMatrix4 translate(trans);

		v.TransformAsVector(translate);
		ASSERT_EQUALS_EPSILON(2.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(2.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(2.0f, v.GetZ(), 0.01f);
	}
	void testMatrixMult()
	{
		SlowVector3 v(1.0f, 1.0f, 1.0f);

		float trans[4][4] = {3.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 3.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 3.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		SlowMatrix4 world_transform(trans);

		float trans2[4][4] = {1.0f, 0.0f, 0.0f, 5.0f,
			0.0f, 1.0f, 0.0f, 10.0f,
			0.0f, 0.0f, 1.0f, -4.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		SlowMatrix4 translate(trans2);

		world_transform.Multiply(translate);

		v.Transform(world_transform);

		ASSERT_EQUALS_EPSILON(18.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(33.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(-9.0f, v.GetZ(), 0.01f);
	}
	void testMatrixAdd()
	{
		SlowVector3 v(1.0f, 1.0f, 1.0f);

		float trans[4][4] = {3.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 3.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 3.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		SlowMatrix4 world_transform(trans);

		float trans2[4][4] = {1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		SlowMatrix4 translate(trans2);

		world_transform.Add(translate);

		v.Transform(world_transform);

		ASSERT_EQUALS_EPSILON(4.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(4.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(4.0f, v.GetZ(), 0.01f);
	}
	void testMatrixSub()
	{
		SlowVector3 v(1.0f, 1.0f, 1.0f);

		float trans[4][4] = {3.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 3.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 3.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		SlowMatrix4 world_transform(trans);

		float trans2[4][4] = {1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
		SlowMatrix4 translate(trans2);

		world_transform.Sub(translate);

		v.Transform(world_transform);

		ASSERT_EQUALS_EPSILON(2.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(2.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(2.0f, v.GetZ(), 0.01f);
	}
	void testMatrixScale()
	{
		SlowMatrix4 scale;
		scale.CreateScale(3.0f);
		SlowVector3 v(1.0f, 1.0f, 1.0f);
		v.Transform(scale);

		ASSERT_EQUALS_EPSILON(3.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(3.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(3.0f, v.GetZ(), 0.01f);
	}
	void testMatrixRotateX()
	{
		SlowMatrix4 rot;
		rot.CreateRotationX(PiOver2);
		SlowVector3 v(SlowVector3::UnitY);
		v.Transform(rot);

		ASSERT_EQUALS_EPSILON(0.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(1.0f, v.GetZ(), 0.01f);
	}
	void testMatrixRotateY()
	{
		SlowMatrix4 rot;
		rot.CreateRotationY(PiOver2);
		SlowVector3 v(SlowVector3::UnitX);
		v.Transform(rot);

		ASSERT_EQUALS_EPSILON(0.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(-1.0f, v.GetZ(), 0.01f);
	}
	void testMatrixRotateZ()
	{
		SlowMatrix4 rot;
		rot.CreateRotationZ(PiOver2);
		SlowVector3 v(SlowVector3::UnitX);
		v.Transform(rot);

		ASSERT_EQUALS_EPSILON(0.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(1.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, v.GetZ(), 0.01f);
	}
	void testMatrixTranslate()
	{
		SlowMatrix4 trans;
		trans.CreateTranslation(SlowVector3(5.0f, 10.0f, -4.0f));
		SlowVector3 v(SlowVector3::Zero);
		v.Transform(trans);

		ASSERT_EQUALS_EPSILON(5.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(10.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(-4.0f, v.GetZ(), 0.01f);
	}

	void testMatrixInvert()
	{
		SlowMatrix4 trans;
		trans.CreateTranslation(SlowVector3(1.0f, 2.0f, -3.0f));

		trans.Invert();
		SlowVector3 v(SlowVector3::Zero);
		v.Transform(trans);

		ASSERT_EQUALS_EPSILON(-1.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(-2.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(3.0f, v.GetZ(), 0.01f);
	}
};

class SlowQuaternionTest : public TestFixture<SlowQuaternionTest>
{
public:
	void PrintVector3(const SlowVector3& data)
	{
		printf("<%f, %f, %f>\n", data.GetX(), data.GetY(), data.GetZ());
	}
	void PrintQuaternion(const SlowQuaternion& data)
	{
		printf("qv = <%f, %f, %f>, qs = %f\n", data.GetVectorX(), data.GetVectorY(), data.GetVectorZ(), data.GetScalar());
	}
	TEST_FIXTURE_DESCRIBE(SlowQuaternionTest, "Testing SlowQuaternion...")
	{
		TEST_CASE_DESCRIBE(testConstructor, "Passing in axis/angle results in correct quaternion");
		TEST_CASE_DESCRIBE(testMultiplication, "Multiply two quaternions together");
		TEST_CASE_DESCRIBE(testConjugate, "Conjugate");
		TEST_CASE_DESCRIBE(testLength, "Length");
		TEST_CASE_DESCRIBE(testLengthSquared, "Length Squared");
		TEST_CASE_DESCRIBE(testNormalize, "Normalize");
		TEST_CASE_DESCRIBE(testRotate, "Rotate a vector by a quaternion");
		TEST_CASE_DESCRIBE(testMatrix, "Create a matrix from a quaternion, and apply to vector");
		TEST_CASE_DESCRIBE(testLerp, "Lerp");
		TEST_CASE_DESCRIBE(testBlend, "4-Way Blend");
	}
	void testConstructor()
	{
		SlowVector3 axis(1.0f, 0.0f, 0.0f);

		SlowQuaternion rot(axis, PiOver4);

		ASSERT_EQUALS_EPSILON(0.383f, rot.GetVectorX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, rot.GetVectorY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, rot.GetVectorZ(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.924f, rot.GetScalar(), 0.01f);
	}
	void testMultiplication()
	{
		SlowVector3 axis(1.0f, 0.0f, 0.0f);
		SlowQuaternion q(axis, PiOver4);
		SlowQuaternion p(axis, PiOver4);

		q.Multiply(p);

		ASSERT_EQUALS_EPSILON(0.707f, q.GetVectorX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, q.GetVectorY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, q.GetVectorZ(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.707f, q.GetScalar(), 0.01f);
	}
	void testConjugate()
	{
		SlowVector3 axis(1.0f, 1.0f, 1.0f);
		axis.Normalize();
		SlowQuaternion q(axis, PiOver4);
		q.Conjugate();

		ASSERT_EQUALS_EPSILON(-0.221f, q.GetVectorX(), 0.01f);
		ASSERT_EQUALS_EPSILON(-0.221f, q.GetVectorY(), 0.01f);
		ASSERT_EQUALS_EPSILON(-0.221f, q.GetVectorZ(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.9238f, q.GetScalar(), 0.01f);
	}
	void testLength()
	{
		SlowVector3 axis(1.0f, 1.0f, 1.0f);
		axis.Normalize();
		SlowQuaternion q(axis, PiOver4);

		ASSERT_EQUALS_EPSILON(1.0f, q.Length(), 0.01f);
	}
	void testLengthSquared()
	{
		SlowQuaternion q(1.0f, 2.0f, 3.0f, 4.0f);

		ASSERT_EQUALS_EPSILON(30.0f, q.LengthSquared(), 0.01f);
	}
	void testNormalize()
	{
		SlowQuaternion q(1.0f, 2.0f, 3.0f, 4.0f);
		q.Normalize();

		ASSERT_EQUALS_EPSILON(0.183f, q.GetVectorX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.365f, q.GetVectorY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.548f, q.GetVectorZ(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.730f, q.GetScalar(), 0.01f);
	}
	void testRotate()
	{
		SlowVector3 axis(1.0f, 0.0f, 0.0f);
		SlowQuaternion q(axis, PiOver2);
		SlowVector3 v(SlowVector3::UnitY);

		v.Rotate(q);

		ASSERT_EQUALS_EPSILON(0.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(1.0f, v.GetZ(), 0.01f);
	}
	void testMatrix()
	{
		SlowVector3 axis(1.0f, 0.0f, 0.0f);
		SlowQuaternion q(axis, Pi / 6.0f);
		SlowVector3 v(SlowVector3::UnitY);
		SlowMatrix4 m;

		m.CreateFromQuaternion(q);
		v.Transform(m);

		ASSERT_EQUALS_EPSILON(0.0f, v.GetX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.866f, v.GetY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.5f, v.GetZ(), 0.01f);
	}
	void testLerp()
	{
		SlowQuaternion q1(0.707107f, 0.0f, 0.0f, 0.707107f);
		SlowQuaternion q2(-0.707107f, 0.0f, 0.0f, 0.707107f);

		SlowQuaternion q_result = Lerp(q1, q2, 0.5f);

		ASSERT_EQUALS_EPSILON(0.0f, q_result.GetVectorX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, q_result.GetVectorY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, q_result.GetVectorZ(), 0.01f);
		ASSERT_EQUALS_EPSILON(1.0f, q_result.GetScalar(), 0.01f);
	}
	void testBlend()
	{
		FastQuaternion q1(1.0f, 1.0f, 1.0f, 1.0f);
		FastQuaternion q2(4.0f, 4.0f, 4.0f, 4.0f);
		FastQuaternion q3(8.0f, 8.0f, 8.0f, 8.0f);
		FastQuaternion q4(16.0f, 16.0f, 16.0f, 16.0f);

		FastQuaternion qBlendResult = Blend(q1, q2, q3, q4, 0.25f, 0.25f, 0.25f);
		ASSERT_EQUALS_EPSILON(0.499f, qBlendResult.GetVectorX(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.499f, qBlendResult.GetVectorY(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.499f, qBlendResult.GetVectorZ(), 0.01f);
		ASSERT_EQUALS_EPSILON(0.499f, qBlendResult.GetScalar(), 0.01f);
	}
};

class SingletonTestClass : public Singleton<SingletonTestClass>
{
	DECLARE_SINGLETON(SingletonTestClass);
protected:
	SingletonTestClass() { value = 0; }
private:
	int value;
public:
	int GetValue() { return value; }
	void IncrementValue() { value++; }
};

class SingletonTest : public TestFixture<SingletonTest>
{
public:
	TEST_FIXTURE_DESCRIBE(SingletonTest, "Testing Singleton...")
	{
		TEST_CASE_DESCRIBE(testSingleton, "Determine whether or not the Singleton works");
	}
	void testSingleton()
	{
		bool passed = true;
		for (int i = 0; i < 10; i++)
		{
			ASSERT_EQUALS(i, SingletonTestClass::get().GetValue());
			SingletonTestClass::get().IncrementValue();
		}
	}
};

typedef PoolAllocator<32, 64> SmallPool;
typedef PoolAllocator<16, 16> TestPool;

class TestPoolUser
{
public:
	unsigned int randomvar1;
	DECLARE_POOL_NEW_DELETE(TestPool);
	TestPoolUser()
	{
		printf("\nCalling TestPoolUser's constructor, OMG!\n");
	}
	~TestPoolUser()
	{
		printf("\nCalling TestPoolUser's destructor, OMG!\n");
	}
};

class PoolAllocatorTest : public TestFixture<PoolAllocatorTest>
{
public:
	TEST_FIXTURE_DESCRIBE(PoolAllocatorTest, "Testing Pool Allocator...")
	{
		//TEST_CASE_DESCRIBE(testStartUp, "Initialize/Deinitialize pools without crashing!");
		TEST_CASE_DESCRIBE(testAllocFree32, "Allocate/Free, Allocate/Free single 32 byte block");
		TEST_CASE_DESCRIBE(testOverflow, "If small pool is full, does it return 0? (SHOULD ASSERT IN DEBUG!)");
		TEST_CASE_DESCRIBE(testReuse, "Allocate/Free all 32 byte blocks 10 times in a row with no errors");
		TEST_CASE_DESCRIBE(testBoundaryCheck, "Allocate, Overwrite Boundary, Free (SHOULD ASSERT IN DEBUG!)");
		TEST_CASE_DESCRIBE(testReuseRandom, "Allocate all blocks/randomly free some, try to allocate again.");
		//TEST_CASE_DESCRIBE(testAlignment, "Make sure we get back a block that's 16-byte aligned.");
		//TEST_CASE_DESCRIBE(testPoolNewDelete, "Allocate for a class using overloaded new/delete");
	}
	void testStartUp()
	{
 		SmallPool::get().StartUp();
 		SmallPool::get().ShutDown();
	}
	void testAllocFree32()
	{
 		SmallPool::get().StartUp();
 		char* temp = reinterpret_cast<char*>(SmallPool::get().Allocate(32));
		ASSERT_TEST_MESSAGE(temp != 0, "Allocate returned 0 when there should be blocks left.");
		ASSERT_TEST_MESSAGE(SmallPool::get().GetNumBlocksFree() == 63, "Incorrect number of blocks remaining.");
 		SmallPool::get().Free(temp);
		ASSERT_TEST_MESSAGE(SmallPool::get().GetNumBlocksFree() == 64, "Incorrect number of blocks remaining.");
#ifdef _DEBUG
 		// Make sure when we call free, the data is overwritten
 		ASSERT_EQUALS((char)0xde, (*temp));
#endif
 		char* temp2 = reinterpret_cast<char*>(SmallPool::get().Allocate(32));
 		ASSERT_TEST_MESSAGE(temp == temp2, "Block not returned to front of free list on Free");
 		SmallPool::get().Free(temp2);
 		SmallPool::get().ShutDown();
	}
	void testOverflow()
	{
 		void* temp;
 		SmallPool::get().StartUp();
 		for (int i = 0; i < 64; i++)
 		{
			ASSERT_TEST_MESSAGE(SmallPool::get().GetNumBlocksFree() == (64 - i), "Incorrect number of blocks remaining.");
 			temp = SmallPool::get().Allocate(32);
 			ASSERT_TEST_MESSAGE(temp != 0, "Allocate returned 0 when there should be blocks left.");
			//printf("%d\n", sizeof(PoolBlock<32>));
			//printf("temp = 0x%x\n", temp);
 		}
 
 		// make sure we return 0 when full
 		temp = SmallPool::get().Allocate(32);
 		ASSERT_TEST_MESSAGE(temp == 0, "Didn't return 0 when absolutely no blocks left.");
 		SmallPool::get().ShutDown();
	}
	void testReuse()
	{
 		void* pointers[64];
 		void* pointers2[64];
 		//void* temp;
 		SmallPool::get().StartUp();
 		for (int i = 0; i < 5; i++)
 		{
 			for (int j = 0; j < 64; j++)
 			{
 				pointers[j] = SmallPool::get().Allocate(32);
 				ASSERT_TEST_MESSAGE(pointers[j] != 0, "Allocate returned 0 when there should be blocks left.");
 			}
 			//temp = SmallPool::get().Allocate(32);
 			//ASSERT_TEST_MESSAGE(temp == 0, "Allocate did not return 0 when full.");
 
 			for (int j = 0; j < 64; j++)
 			{
 				SmallPool::get().Free(pointers[64 - j - 1]);
 			}
 
 			for (int j = 0; j < 64; j++)
 			{
 				pointers2[j] = SmallPool::get().Allocate(32);
 				ASSERT_TEST_MESSAGE(pointers[j] == pointers2[j], "Blocks not returned to free list in expected order.")
 			}
 			//temp = SmallPool::get().Allocate(32);
 			//ASSERT_TEST_MESSAGE(temp == 0, "Allocate did not return 0 when full.");
 
 			for (int j = 0; j < 64; j++)
 			{
 				SmallPool::get().Free(pointers[64 - j - 1]);
 			}
 		}
 		SmallPool::get().ShutDown();
	}
	void testBoundaryCheck()
	{
 		SmallPool::get().StartUp();
 		char* temp = reinterpret_cast<char*>(SmallPool::get().Allocate(32));
		ASSERT_TEST_MESSAGE(temp != 0, "Allocate returned 0 when there should be blocks left.");
 		temp[32] = 'a';
 		SmallPool::get().Free(temp);
 		SmallPool::get().ShutDown();
	}
	void testReuseRandom()
	{
		SmallPool::get().StartUp();
		std::vector<void*> blocks;
		for (int i = 0; i < 64; i++)
		{
			blocks.push_back(SmallPool::get().Allocate(32));
		}

		std::srand(static_cast<unsigned>(std::time(0)));
		std::random_shuffle(blocks.begin(), blocks.end());
		for (int i = 0; i < 32; i++)
		{
			void* temp = blocks.back();
			SmallPool::get().Free(temp);
			blocks.pop_back();
		}

		for (int i = 0; i < 32; i++)
		{
			void* temp = SmallPool::get().Allocate(32);
			ASSERT_TEST_MESSAGE(std::find(blocks.begin(), blocks.end(), temp) == blocks.end(),
				"Allocate returned a pointer that's already in use.");
			blocks.push_back(temp);
		}
		SmallPool::get().ShutDown();
	}
	void testAlignment()
	{
		SmallPool::get().StartUp();
		char* temp = reinterpret_cast<char*>(SmallPool::get().Allocate(sizeof(FastVector3)));
		ASSERT_TEST_MESSAGE(temp != 0, "Allocate returned 0 when there should be blocks left.");
		printf("temp = 0x%x\n", temp);
		char* temp2 = reinterpret_cast<char*>(SmallPool::get().Allocate(sizeof(FastVector3)));
		printf("temp2 = 0x%x\n", temp2);
		FastVector3* vTemp = new(temp) FastVector3(1.0f, 2.0f, 3.0f);
		FastVector3* vTemp2 = new(temp2) FastVector3(2.0f, 4.0f, 6.0f);
		ASSERT_EQUALS_EPSILON(28.0f, vTemp->Dot(*vTemp2), 0.01f);
		printf("result = %f\n", vTemp->Dot(*vTemp2));
		SmallPool::get().ShutDown();
	}
	void testPoolNewDelete()
	{
		TestPool::get().StartUp();
		ASSERT_TEST_MESSAGE(TestPool::get().GetNumBlocksFree() == 16, "Incorrect number of blocks remaining.");
		TestPoolUser* pTemp = new TestPoolUser();
		ASSERT_TEST_MESSAGE(TestPool::get().GetNumBlocksFree() == 15, "Incorrect number of blocks remaining.");
		ASSERT_TEST_MESSAGE(pTemp->randomvar1 == 0xdededede, "Memory not cleared out, did it come from the pool?");
		delete pTemp;
		TestPool::get().ShutDown();
	}
};

REGISTER_FIXTURE(FastVector3Test);
REGISTER_FIXTURE(FastMatrix4Test);
//REGISTER_FIXTURE(FastQuaternionTest);
//REGISTER_FIXTURE(SlowVector3Test);
//REGISTER_FIXTURE(SlowMatrix4Test);
//REGISTER_FIXTURE(SlowQuaternionTest);
//REGISTER_FIXTURE(SingletonTest);
REGISTER_FIXTURE(PoolAllocatorTest);
} // namespace ITP485

#endif // _UNITTESTS_HPP_

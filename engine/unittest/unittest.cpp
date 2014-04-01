// unittest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "..\core\fastmath.h"
#include "..\core\slowmath.h"
#include "..\MiniCppUnit-2.5\MiniCppUnit.hxx"
#include "unittests.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace ITP485;

void test_speed_cross_product();
void test_speed_normalize();
void test_speed_matrix_mult();
void test_speed_create_rotation();
void test_speed_quat_to_matrix();

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Select a test suite to run:" << std::endl;
	std::cout << "1. Functionality tests" << std::endl;
	std::cout << "2. Speed tests (make sure you are running in RELEASE)" << std::endl;
	int choice;
	std::cin >> choice;
	
	if (choice == 1)
	{
		TestFixtureFactory::theInstance().runTests();
	}
	else if (choice == 2)
	{
		std::cout << "********************************************" << std::endl;
		test_speed_cross_product();
		//std::cout << "********************************************" << std::endl;
		//test_speed_normalize();
		std::cout << "********************************************" << std::endl;
		test_speed_matrix_mult();
		std::cout << "********************************************" << std::endl;
		//test_speed_create_rotation();
		//std::cout << "********************************************" << std::endl;
		//test_speed_quat_to_matrix();
		//std::cout << "********************************************" << std::endl;
	}

	while (getchar() != '\n'); // clear input buffer
	std::cout << "Press enter to continue..." << std::endl;
	getchar();
	return 0;
}

float output_duration(float freq_ms, float num_tests, LARGE_INTEGER perf_start, LARGE_INTEGER perf_end)
{
	float elapsed = (perf_end.QuadPart - perf_start.QuadPart) / freq_ms;
	std::cout << "Total duration for 10000 = " << elapsed << "ms" << std::endl;
	std::cout << "Average duration = " << elapsed / 10000.0f << "ms" << std::endl;

	return elapsed;
}

void output_results(float elapsed_slow, float elapsed_fast)
{
	if (elapsed_fast > elapsed_slow)
	{
		std::cout << "Fast implementation is slower :(" << std::endl;
	}
	else
	{
		std::cout << "Fast implementation is " << elapsed_slow/elapsed_fast << "x faster!! :)" << std::endl;
	}
}

void test_speed_cross_product()
{
	LARGE_INTEGER freq, perf_start, perf_end;
	float freqms, elapsed_slow, elapsed_fast;
	QueryPerformanceFrequency(&freq);
	freqms = freq.QuadPart / 1000.0f;

	std::cout << "Testing slow cross product..." << std::endl;
	QueryPerformanceCounter(&perf_start);
	SlowVector3 sv1(1.0f, 2.0f, 3.0f);
	SlowVector3 sv2(2.0f, 3.0f, 4.0f);
	for (int i = 0; i < 10000; i++)
	{
		sv1 = Cross(sv1, sv2);
	}
	QueryPerformanceCounter(&perf_end);
	std::cout << sv1.GetX() << std::endl;
	elapsed_slow = output_duration(freqms, 10000.0f, perf_start, perf_end);

	std::cout << std::endl;

	std::cout << "Testing fast cross product..." << std::endl;
	QueryPerformanceCounter(&perf_start);
	FastVector3 fv1(1.0f, 2.0f, 3.0f);
	FastVector3 fv2(2.0f, 3.0f, 4.0f);
	for (int i = 0; i < 10000; i++)
	{
		fv1 = Cross(fv1, fv2);
	}
	QueryPerformanceCounter(&perf_end);
	std::cout << fv1.GetX() << std::endl;
	elapsed_fast = output_duration(freqms, 10000.0f, perf_start, perf_end);

	std::cout << std::endl;
	output_results(elapsed_slow, elapsed_fast);
}

void test_speed_normalize()
{
	LARGE_INTEGER freq, perf_start, perf_end;
	float freqms, elapsed_slow, elapsed_fast;
	QueryPerformanceFrequency(&freq);
	freqms = freq.QuadPart / 1000.0f;

	std::cout << "Testing slow normalize..." << std::endl;
	QueryPerformanceCounter(&perf_start);
	SlowVector3 sv1(1.0f, 2.0f, 3.0f);
	for (int i = 0; i < 10000; i++)
	{
		sv1.Normalize();
	}
	QueryPerformanceCounter(&perf_end);
	std::cout << sv1.GetX() << std::endl;
	elapsed_slow = output_duration(freqms, 10000.0f, perf_start, perf_end);

	std::cout << std::endl;

	std::cout << "Testing fast normalize..." << std::endl;
	QueryPerformanceCounter(&perf_start);
	FastVector3 fv1(1.0f, 2.0f, 3.0f);
	for (int i = 0; i < 10000; i++)
	{
		fv1.Normalize();
	}
	QueryPerformanceCounter(&perf_end);
	std::cout << fv1.GetX() << std::endl;
	elapsed_fast = output_duration(freqms, 10000.0f, perf_start, perf_end);

	std::cout << std::endl;
	output_results(elapsed_slow, elapsed_fast);
}

void test_speed_matrix_mult()
{
	LARGE_INTEGER freq, perf_start, perf_end;
	float freqms, elapsed_slow, elapsed_fast;
	QueryPerformanceFrequency(&freq);
	freqms = freq.QuadPart / 1000.0f;
	
	float mat1[4][4] =
	{
		3.0f, 0.0f, 0.0f, -5.0f,
		0.0f, 3.0f, 0.0f, 5.0f,
		0.0f, 0.0f, 3.0f, 4.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	float mat2[4][4] =
	{
		1.0f, 0.0f, 0.0f, -3.0f,
		0.0f, 1.0f, 0.0f, 3.0f,
		0.0f, 0.0f, 1.0f, 2.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	std::cout << "Testing slow matrix multiplication..." << std::endl;
	QueryPerformanceCounter(&perf_start);
	SlowMatrix4 sm1(mat1);
	SlowMatrix4 sm2(mat2);
	for (int i = 0; i < 10000; i++)
	{
		sm2.Multiply(sm1);
	}
	QueryPerformanceCounter(&perf_end);
	elapsed_slow = output_duration(freqms, 10000.0f, perf_start, perf_end);

	std::cout << std::endl;

	std::cout << "Testing fast matrix multiplication..." << std::endl;
	QueryPerformanceCounter(&perf_start);
	FastMatrix4 fm1(mat1);
	FastMatrix4 fm2(mat2);
	for (int i = 0; i < 10000; i++)
	{
		fm2.Multiply(fm1);
	}
	QueryPerformanceCounter(&perf_end);
	elapsed_fast = output_duration(freqms, 10000.0f, perf_start, perf_end);

	std::cout << std::endl;
	output_results(elapsed_slow, elapsed_fast);
}

void test_speed_create_rotation()
{
	LARGE_INTEGER freq, perf_start, perf_end;
	float freqms, elapsed_slow, elapsed_fast;
	QueryPerformanceFrequency(&freq);
	freqms = freq.QuadPart / 1000.0f;

	std::cout << "Testing slow create rotation and transform..." << std::endl;
	QueryPerformanceCounter(&perf_start);
	SlowVector3 sv1(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < 10000; i++)
	{
		SlowMatrix4 sm1;
		sm1.CreateRotationX(PiOver2 * i);
		sv1.Transform(sm1);
	}
	QueryPerformanceCounter(&perf_end);
	std::cout << sv1.GetX() << std::endl;
	elapsed_slow = output_duration(freqms, 10000.0f, perf_start, perf_end);

	std::cout << std::endl;

	std::cout << "Testing fast create rotation and transform..." << std::endl;
	QueryPerformanceCounter(&perf_start);
	FastVector3 fv1(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < 10000; i++)
	{
		FastMatrix4 fm1;
		fm1.CreateRotationX(PiOver2 * i);
		fv1.Transform(fm1);
	}
	QueryPerformanceCounter(&perf_end);
	std::cout << fv1.GetX() << std::endl;
	elapsed_fast = output_duration(freqms, 10000.0f, perf_start, perf_end);

	std::cout << std::endl;
	output_results(elapsed_slow, elapsed_fast);
}

void test_speed_quat_to_matrix()
{
	LARGE_INTEGER freq, perf_start, perf_end;
	float freqms, elapsed_slow, elapsed_fast;
	QueryPerformanceFrequency(&freq);
	freqms = freq.QuadPart / 1000.0f;

	std::cout << "Testing slow quat to matrix and transform..." << std::endl;
	QueryPerformanceCounter(&perf_start);
	SlowVector3 sv1(0.0f, 1.0f, 0.0f);
	SlowVector3 saxis(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < 10000; i++)
	{
		SlowMatrix4 sm1;
		SlowQuaternion squat(saxis, PiOver2 * i);
		sm1.CreateFromQuaternion(squat);
		sv1.Transform(sm1);
	}
	QueryPerformanceCounter(&perf_end);
	std::cout << sv1.GetX() << std::endl;
	elapsed_slow = output_duration(freqms, 10000.0f, perf_start, perf_end);

	std::cout << std::endl;

	std::cout << "Testing fast quat to matrix and transform..." << std::endl;
	QueryPerformanceCounter(&perf_start);
	FastVector3 fv1(0.0f, 1.0f, 0.0f);
	FastVector3 faxis(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < 10000; i++)
	{
		FastMatrix4 fm1;
		FastQuaternion fquat(faxis, PiOver2 * i);
		fm1.CreateFromQuaternion(fquat);
		fv1.Transform(fm1);
	}
	QueryPerformanceCounter(&perf_end);
	std::cout << fv1.GetX() << std::endl;
	elapsed_fast = output_duration(freqms, 10000.0f, perf_start, perf_end);

	std::cout << std::endl;
	output_results(elapsed_slow, elapsed_fast);
}

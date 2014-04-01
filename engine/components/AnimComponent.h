// The anim component is used by game objects which have animation data
#pragma once

#include "../core/math.h"
#include "../core/poolalloc.h"
#include <string>

struct ID3DXEffect;

namespace ITP485
{

// Maximum joints
const int MAX_JOINTS = 64;

// Joint structure
struct Joint
{
	// inverse bind pose (global) matrix
	Matrix4 inv_bindPose;

	// local bind pose for this joint
	Matrix4 localPose;

	// Name of the joint
	std::string m_Name;

	// parent index or -1 if root
	short m_ParentIndex;

	// Overloads of array new/delete to ensure the array is 16-byte aligned
	void* operator new[] (size_t size)
	{
		return _aligned_malloc(size, 16);
	}
	void operator delete[] (void* ptr)
	{
		_aligned_free(ptr);
	}
};

// Skeleton structure
struct Skeleton
{
	// Array of joints
	Joint* m_pJoints;

	// Number of joints
	short m_iNumJoints;

	Skeleton()
	: m_pJoints(nullptr)
	{

	}
};

typedef PoolAllocator<80, 1024> KeyFramePool;

// Key Frame structure
struct KeyFrame
{
	// Local pose at this joint at this keyframe
	Matrix4 localPose;

	// Frame number where this occurs
	int m_FrameNum;

	// Next key frame (if any)
	KeyFrame* m_Next;

	KeyFrame()
	: m_Next(nullptr)
	{

	}

	DECLARE_POOL_NEW_DELETE(KeyFramePool);
};

// Animation structure
struct Animation
{
	// Name of this animation
	std::string m_Name;

	// Array of key frame pointers
	KeyFrame** m_pKeyFrames;

	// Length of this animation
	int m_NumFrames;

	// Frame we're currently on
	int m_CurrFrame;

	// Float frame (time) we're on
	float m_Time;

	Animation()
	: m_pKeyFrames(nullptr)
	, m_CurrFrame(0)
	, m_Time(0.0f)
	{

	}
};

// For calculating the current pose
struct JointPose
{
	// local pose for this joint
	Matrix4 localPose;

	// Overloads of array new/delete to ensure the array is 16-byte aligned
	void* operator new[] (size_t size)
	{
		return _aligned_malloc(size, 16);
	}
	void operator delete[] (void* ptr)
	{
		_aligned_free(ptr);
	}
};

// Skeleton's current pose
struct SkeletonPose
{
	// Array of joint poses
	JointPose* m_pPoses;

	SkeletonPose()
	: m_pPoses(nullptr)
	{

	}
};

class AnimComponent
{
public:
	// Constructor takes the name of the anim file
	AnimComponent(const char* szFileName);

	// Destructor
	~AnimComponent();

	// Update the active animation
	void Update(float fDelta);

	// Called by MeshComponent when it needs the matrix palette
	void StoreMatrixPalette(ID3DXEffect* pEffect);
private:
	// The skeleton for this anim component
	Skeleton m_Skeleton;

	// Current pose of this anim component
	SkeletonPose m_Pose;

	// The animation we're currently playing
	// We'd ideally change this to a pointer and load several animations
	// and be able to choose between them.
	// But right now the engine doesn't support this!
	Animation m_CurrAnimation;

	// Matrix palette (array) for this anim component
	Matrix4* m_Palette;

	// Initialize the animation data as needed
	void InitializeData();

	// Parses in the file information
	void Parse(const char* szFileName);
};

} // end namespace

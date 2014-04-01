#include "AnimComponent.h"
#include "../../ticpp/ticpp.h"
#include <sstream>
#include <d3d9.h>
#include <d3dx9math.h>

namespace ITP485
{

AnimComponent::AnimComponent( const char* szFileName )
: m_Palette(nullptr)
{
	Parse(szFileName);
	InitializeData();
}

void AnimComponent::InitializeData()
{
	
}

AnimComponent::~AnimComponent()
{
	// Delete stuff so we don't leak copious amounts of memory
	delete[] m_Skeleton.m_pJoints;
	for (int i = 0; i < m_Skeleton.m_iNumJoints; i++)
	{
		KeyFrame* curr = m_CurrAnimation.m_pKeyFrames[i];
		while (curr != nullptr)
		{
			KeyFrame* next = curr->m_Next;
			delete curr;
			curr = next;
		}
	}
	delete[] m_CurrAnimation.m_pKeyFrames;

	_aligned_free(m_Palette);
}

void AnimComponent::Update( float fDelta )
{

}

void AnimComponent::StoreMatrixPalette( ID3DXEffect* pEffect )
{

}


void AnimComponent::Parse( const char* szFileName )
{
	// Parse the itpanim file
	ticpp::Document doc(szFileName);
	doc.LoadFile();

	// This is some really ugly XML parsing code
	ticpp::Iterator<ticpp::Element> child;
	for(child = child.begin(doc.FirstChildElement()); child != child.end(); child++)
	{
		// The value of this child identifies the name of this element
		std::string strName;
		std::string strValue;
		std::string strText;

		child->GetValue(&strName);
		if (strName == "skeleton")
		{
			// Initialize the bones array
			strValue = child->GetAttribute("count");
			m_Skeleton.m_iNumJoints = atoi(strValue.c_str());
			m_Skeleton.m_pJoints = new Joint[m_Skeleton.m_iNumJoints];

			// Skeleton pose
			m_Pose.m_pPoses = new JointPose[m_Skeleton.m_iNumJoints];

			// Matrix palette
			void* buf = _aligned_malloc(sizeof(Matrix4) * m_Skeleton.m_iNumJoints, 16);
			m_Palette = new (buf) Matrix4[m_Skeleton.m_iNumJoints];

			// Now get every joint
			ticpp::Iterator<ticpp::Element> joint;
			for(joint = joint.begin(child.Get()); joint != joint.end(); joint++)
			{
				//float x, y, z, w;
				float mat[4][4];

				strValue = joint->GetAttribute("id");
				int index = atoi(strValue.c_str());
				
				m_Skeleton.m_pJoints[index].m_Name = joint->GetAttribute("name");
				
				strValue = joint->GetAttribute("parent");
				int parent = atoi(strValue.c_str());
				m_Skeleton.m_pJoints[index].m_ParentIndex = parent;

				// Get the translation/quat for this joint
				ticpp::Iterator<ticpp::Element> ele;
				for(ele = ele.begin(joint.Get()); ele != ele.end(); ele++)
				{
					ele->GetValue(&strName);
					/*if (strName == "pos")
					{
						sscanf_s(ele->GetText().c_str(), "%f,%f,%f", &x, &y, &z);
						m_Skeleton.m_pJoints[index].m_vBindTrans.Set(x, y, z);
					}
					else if (strName == "quat")
					{
						sscanf_s(ele->GetText().c_str(), "%f,%f,%f,%f", &x, &y, &z, &w);
						m_Skeleton.m_pJoints[index].m_qBindQuat.Set(x, y, z, w);
					}
					else */if (strName == "mat")
					{
						sscanf_s(ele->GetText().c_str(), "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
							&mat[0][0], &mat[0][1], &mat[0][2], &mat[0][3],
							&mat[1][0], &mat[1][1], &mat[1][2], &mat[1][3],
							&mat[2][0], &mat[2][1], &mat[2][2], &mat[2][3],
							&mat[3][0], &mat[3][1], &mat[3][2], &mat[3][3]);
						m_Skeleton.m_pJoints[index].localPose.Set(mat);
					}
				}
			}
		}
		else if (strName == "animations")
		{
			// Now get every animation
			ticpp::Iterator<ticpp::Element> anim;
			for(anim = anim.begin(child.Get()); anim != anim.end(); anim++)
			{
				//float x, y, z, w;
				float mat[4][4];

				// Get Name and length
				strValue = anim->GetAttribute("name");
				m_CurrAnimation.m_Name = strValue;

				strValue = anim->GetAttribute("length");
				m_CurrAnimation.m_NumFrames = atoi(strValue.c_str());
				m_CurrAnimation.m_CurrFrame = 0;

				// Initialize key frame array of pointers
				m_CurrAnimation.m_pKeyFrames = new KeyFrame*[m_Skeleton.m_iNumJoints];

				// Now loop through the tracks
				ticpp::Iterator<ticpp::Element> track;
				for(track = track.begin(anim.Get()); track != track.end(); track++)
				{
					strValue = track->GetAttribute("id");
					int index = atoi(strValue.c_str());

					KeyFrame* PrevKey = nullptr;
					// Go through every key frame for this track
					ticpp::Iterator<ticpp::Element> key;
					for(key = key.begin(track.Get()); key != key.end(); key++)
					{
						// Instantiate this key frame
						KeyFrame* CurrKey = new KeyFrame;

						// Set previous key's ptr if one exists
						if (PrevKey != nullptr)
						{
							PrevKey->m_Next = CurrKey;
						}
						else // otherwise this one is the first
						{
							m_CurrAnimation.m_pKeyFrames[index] = CurrKey;
						}

						// Grab frame number
						strValue = key->GetAttribute("num");
						CurrKey->m_FrameNum = atoi(strValue.c_str());

						// Now grab the data for this key frame
						ticpp::Iterator<ticpp::Element> ele;
						for(ele = ele.begin(key.Get()); ele != ele.end(); ele++)
						{
							ele->GetValue(&strName);
							/*if (strName == "pos")
							{
								sscanf_s(ele->GetText().c_str(), "%f,%f,%f", &x, &y, &z);
								CurrKey->m_vTrans.Set(x, y, z);
							}
							else if (strName == "quat")
							{
								sscanf_s(ele->GetText().c_str(), "%f,%f,%f,%f", &x, &y, &z, &w);
								CurrKey->m_qRot.Set(x, y, z, w);
							}
							else*/ if (strName == "mat")
							{
								sscanf_s(ele->GetText().c_str(), "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
									&mat[0][0], &mat[0][1], &mat[0][2], &mat[0][3],
									&mat[1][0], &mat[1][1], &mat[1][2], &mat[1][3],
									&mat[2][0], &mat[2][1], &mat[2][2], &mat[2][3],
									&mat[3][0], &mat[3][1], &mat[3][2], &mat[3][3]);
								CurrKey->localPose.Set(mat);
							}
						}

						PrevKey = CurrKey;
					}
				}
			}
		}
	}
}

} // end namespace

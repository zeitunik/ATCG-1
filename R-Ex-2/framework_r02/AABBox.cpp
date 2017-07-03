//=============================================================================
//
//       Copyright (C) 2017 by Computer Graphics Group, University of Bonn
//                           cg.cs.uni-bonn.de
//
//-----------------------------------------------------------------------------
//
//	part of ATCG1, Excercise R02
//
//=============================================================================

//=============================================================================
//
//  CLASS CAABBox - IMPLEMENTATION
//
//=============================================================================

#define RAYTRACER_AABBOX_CPP


//== INCLUDES =================================================================
#include "AABBox.h"

//== NAMESPACE =============================================================== 

namespace RAYTRACER {
   
//== IMPLEMENTATION ========================================================== 

void
CAABBox::ExtendBy(const VectorType3& v3Point)
{
	if (v3Point[0] < m_v3Min[0]) m_v3Min[0] = v3Point[0];
	if (v3Point[1] < m_v3Min[1]) m_v3Min[1] = v3Point[1];
	if (v3Point[2] < m_v3Min[2]) m_v3Min[2] = v3Point[2];

	if (v3Point[0] > m_v3Max[0]) m_v3Max[0] = v3Point[0];
	if (v3Point[1] > m_v3Max[1]) m_v3Max[1] = v3Point[1];
	if (v3Point[2] > m_v3Max[2]) m_v3Max[2] = v3Point[2];
};

//-----------------------------------------------------------------------------

void
CAABBox::ExtendBy(const CAABBox& clBox)
{
	ExtendBy( clBox.GetMin() );
	ExtendBy( clBox.GetMax() );
};

//-----------------------------------------------------------------------------

bool
CAABBox::Intersect(const CRay& clRay) const
{
	const VectorType3& v3_o   = clRay.GetOrigin();
	const VectorType3& v3_dir = clRay.GetDir();
	VectorType3 v3_near;
	VectorType3 v3_far;

	if (v3_dir[0] > 0)
	{
		v3_near[0] = (m_v3Min[0] - v3_o[0])/v3_dir[0];
		v3_far[0] = (m_v3Max[0] - v3_o[0])/v3_dir[0];
	}
	else if (v3_dir[0] < 0)
	{
		v3_near[0] = (m_v3Max[0] - v3_o[0])/v3_dir[0];
		v3_far[0] = (m_v3Min[0] - v3_o[0])/v3_dir[0];
	}else{
		v3_near[0] = -FLT_MAX;
		v3_far[0] = FLT_MAX;
	}
	if (v3_dir[1] > 0)
	{
		v3_near[1] = (m_v3Min[1] - v3_o[1])/v3_dir[1];
		v3_far[1] = (m_v3Max[1] - v3_o[1])/v3_dir[1];
	}
	else if ( v3_dir[1] < 0 )
	{
		v3_near[1] = (m_v3Max[1] - v3_o[1])/v3_dir[1];
		v3_far[1] = (m_v3Min[1] - v3_o[1])/v3_dir[1];
	}else{
		v3_near[1] = -FLT_MAX;
		v3_far[1] = FLT_MAX;
	}
	if (v3_dir[2] > 0)
	{
		v3_near[2] = (m_v3Min[2] - v3_o[2])/v3_dir[2];
		v3_far[2] = (m_v3Max[2] - v3_o[2])/v3_dir[2];
	}
	else if ( v3_dir[2] < 0 )
	{
		v3_near[2] = (m_v3Max[2] - v3_o[2])/v3_dir[2];
		v3_far[2] = (m_v3Min[2] - v3_o[2])/v3_dir[2];
	}else{
		v3_near[2] = -FLT_MAX;
		v3_far[2] = FLT_MAX;
	}

	RealType t_0 = std::max(v3_near[0], std::max(v3_near[1], v3_near[2]));
	RealType t_1 = std::min(v3_far[0], std::min(v3_far[1], v3_far[2]));
	
	return t_0<t_1;
}

//=============================================================================
}; // namespace RAYTRACER
//=============================================================================



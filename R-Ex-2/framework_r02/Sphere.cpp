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
//  CLASS CSphere - IMPLEMENTATION
//
//=============================================================================

#define RAYTRACER_SPHERE_CPP
#define _USE_MATH_DEFINES

//== INCLUDES =================================================================
#include <cmath>
#include "Sphere.h"

//== NAMESPACE ===============================================================

namespace RAYTRACER {

//== IMPLEMENTATION ==========================================================

VectorType2
CSphere::Cartesian2Polar(VectorType3& v) const
{
	RealType z = (v[2]>1) ? 1 : (v[2]<-1) ? -1 : v[2];
	RealType theta = (RealType) acos(z);
	RealType phi =   (RealType) atan2(v[1], v[0]);
	if (phi<0) phi = phi+2*(RealType)M_PI;
	return VectorType2( theta, phi );
}

//-----------------------------------------------------------------------------

bool
CSphere::Intersect(const CRay& clRay, RealType t0, RealType t1, TTracingContext& tContext ) const
{
	/*
		Implement ray-sphere intersection.
		You must set the following member of the TTracingContext struct:
		t          - ray parameter of intersection point
		v3Normal   - Normal of surface at intersection point
		v3HitPoint - Coordinate of intersection point
		v2TexCoord - 2D-Texture coordinate of intersection point (use polar coordinates (method Cartesian2Polar), not needed currently)
		pclShader  - Material of sphere
	*/

	VectorType3 d = clRay.GetDir();
	VectorType3 o = clRay.GetOrigin();
	VectorType3 diff = o - m_v3Center;
	
	RealType A = dot(d, d);
	RealType B = 2*dot(d, diff);
	RealType C = dot(diff, diff) - m_rRadius * m_rRadius;
	RealType D = B*B - 4*A*C;

	RealType t;
	if ( D >= 0)
	{
		t = (-B - sqrt(B*B - 4*A*C)) / (2*A); 
		if(t < t0 || t >= t1) return false;

		tContext.t = t;
		VectorType3 i = clRay.Evaluate(t);
		VectorType3 i_c = i - m_v3Center;
		
		tContext.v3Normal = i_c/m_rRadius;
		tContext.v3HitPoint = i;
		tContext.v2TexCoord = Cartesian2Polar(i_c);
		tContext.pclShader = GetShader();

		return true;
	}

	return false;
}

//=============================================================================
}; // namespace RAYTRACER
//=============================================================================



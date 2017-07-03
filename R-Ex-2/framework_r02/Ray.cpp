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
//  CLASS CRay - IMPLEMENTATION
//
//=============================================================================

#define RAYTRACER_RAY_CPP


//== INCLUDES =================================================================
#include "Ray.h"

//== NAMESPACE ===============================================================

namespace RAYTRACER {

//== IMPLEMENTATION ==========================================================

CRay
CRay::ReflectedRay(const VectorType3& v3Point,  const VectorType3& v3Normal) const
{
	/*
		Implement the construction of the reflected ray at point v3Point and with normal vector v3Normal
	*/

	CRay result;
	// r = d − 2 <n | d> n

	result = m_v3Dir - 2 * dot(v3Normal, m_v3Dir) * v3Normal;

	return result;
};


//-----------------------------------------------------------------------------

bool
CRay::RefractedRay(const VectorType3& v3Point,
			       const VectorType3& v3Normal,
				   RealType          rRefractiveIndex,
				   CRay&             clRefractedRay ) const
{
	/*
		Implement the calculation of the refracted ray at point v3Point, with normal vector v3Normal and
		refractive index of the material rRefractiveIndex.
		Return false in case of total internal reflection.

		Hint: We assume that the refraction occurs between a material with the given refractive index and air (refractive index ~1).
	*/

	//t = c_n n + c_d d
	RealType etta; 	
	RealType d = dot(v3Normal, m_v3Dir);
	
	if (d < 0 ) 
		 etta = rRefractiveIndex;
	else 
		etta = 1/rRefractiveIndex;

	RealType c = 1 / etta;
	RealType value = 1 - c*c * (1 - d*d);
	if (value < 0)	// total reflection
		return false;

	RealType c_n = -c * d - sqrt(value);
	RealType c_c = c;
	
	clRefractedRay = c_n * v3Normal + c_d * m_v3Dir;

	return true;
};


//=============================================================================
}; // namespace RAYTRACER
//=============================================================================



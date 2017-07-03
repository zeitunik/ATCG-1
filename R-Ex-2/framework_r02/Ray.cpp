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

	return true;
};


//=============================================================================
}; // namespace RAYTRACER
//=============================================================================



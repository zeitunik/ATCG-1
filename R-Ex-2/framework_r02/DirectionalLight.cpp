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
//  CLASS CDirectionalLight - IMPLEMENTATION
//
//=============================================================================

#define RAYTRACER_POINTLIGHT_CPP


//== INCLUDES =================================================================
#include <cfloat>
#include "DirectionalLight.h"

//== NAMESPACE =============================================================== 

namespace RAYTRACER {
   
//== IMPLEMENTATION ========================================================== 

/*!

*/
void 
CDirectionalLight::ShadowFeeler( unsigned int      uiNumSample,
		                         CRay&             clRay,
				                 RealType&         rRayDomain,
				                 ColorType&        colIntensity) const
{
	clRay.SetDir( m_v3Direction );
	rRayDomain = FLT_MAX;
	colIntensity = m_colIntensity;
};


//=============================================================================
}; // namespace RAYTRACER
//=============================================================================



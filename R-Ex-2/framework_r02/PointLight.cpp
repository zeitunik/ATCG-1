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
//  CLASS CPointLight - IMPLEMENTATION
//
//=============================================================================

#define RAYTRACER_POINTLIGHT_CPP


//== INCLUDES =================================================================
#include "PointLight.h"

//== NAMESPACE =============================================================== 

namespace RAYTRACER {
   
//== IMPLEMENTATION ========================================================== 

/*!

*/
void 
CPointLight::ShadowFeeler( unsigned int      uiNumSample,
		                   CRay&             clRay,
				           RealType&         rRayDomain,
				           ColorType&        colIntensity) const
{
	clRay.SetDir( m_v3Position - clRay.GetOrigin() );
	rRayDomain = 1;
	
	//attenuation
	colIntensity = m_colIntensity * (1/clRay.GetDir().length());	
};


//=============================================================================
}; // namespace RAYTRACER
//=============================================================================



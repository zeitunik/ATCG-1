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
//  CLASS CLambertShader - IMPLEMENTATION
//
//=============================================================================

#define RAYTRACER_LAMBERTSHADER_CPP


//== INCLUDES =================================================================
#include "LambertShader.h"

//== NAMESPACE =============================================================== 

namespace RAYTRACER {
   
//== IMPLEMENTATION ========================================================== 

ColorType
CLambertShader::Shade(const TTracingContext& tContext) const
{
	const VectorType3& v3_normal = tContext.v3Normal;
	RealType r_cos_theta = v3_normal | tContext.v3Incoming;
	if (r_cos_theta<0) return ColorType(0,0,0);

	return tContext.colLightIntensity*m_colDiffuse*r_cos_theta;
};

//=============================================================================
}; // namespace RAYTRACER
//=============================================================================



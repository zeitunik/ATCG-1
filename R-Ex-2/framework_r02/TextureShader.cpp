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
//  CLASS CTextureShader - IMPLEMENTATION
//
//=============================================================================

#define RAYTRACER_TEXTURESHADER_CPP


//== INCLUDES =================================================================
#include "TextureShader.h"

//== NAMESPACE =============================================================== 

namespace RAYTRACER {
   
//== IMPLEMENTATION ========================================================== 

ColorType
CTextureShader::Shade(const TTracingContext& rtContext) const
{
	//get texcoords
	double tmp;
	RealType f_x = modf(rtContext.v2TexCoord[0]*m_v2Scale[0], &tmp);
	RealType f_y = modf(rtContext.v2TexCoord[1]*m_v2Scale[1], &tmp);
	if (f_x<0) f_x+=1;
	if (f_y<0) f_y+=1;

	//No interpolation between texels
	int i_x = (int)(f_x * m_width);
	int i_y = (int)(f_y * m_height);
	
	ColorType col;
	col[0] = m_clTexture[i_y * m_width + i_x][0];
	col[1] = m_clTexture[i_y * m_width + i_x][1];
	col[2] = m_clTexture[i_y * m_width + i_x][2];

	//evaluate lambertian diffuse term
	const VectorType3& v3_normal = rtContext.v3Normal;
	RealType f_cos_theta = v3_normal | rtContext.v3Incoming;
	if (f_cos_theta<0) f_cos_theta = 0;
	
	  	  
	return (rtContext.colLightIntensity * col) * f_cos_theta;
};

//-----------------------------------------------------------------------------

void
CTextureShader::LoadTexture(const std::string& sFilename)
{
	//m_clTexture.load( sFilename.c_str() );
}


//=============================================================================
}; // namespace RAYTRACER
//=============================================================================



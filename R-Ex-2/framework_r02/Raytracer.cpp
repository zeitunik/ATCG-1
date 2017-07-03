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
//  CLASS CWhitted - IMPLEMENTATION
//
//=============================================================================

#define RAYTRACER_WHITTED_CPP


//== INCLUDES =================================================================

#include <omp.h>
#include "Raytracer.h"

//== NAMESPACE ===============================================================

namespace RAYTRACER {

//== IMPLEMENTATION ==========================================================

ColorType
CWhitted::TraceRay(const CSurface*   pclScene,
				   const CRay&	     crclRay,
				   unsigned int      uiDepth )
{
	ColorType col(0.);
	if (0 == uiDepth) return col;
	--uiDepth;


	/*
		Implement:

		(i)  compute the nearest intersection point of the ray with the scene
		(ii) evaluate the local light model at the intersection point (method Shade())
		(iii)if surface hit by the ray is specular (CShader.IsSpecular()), compute and trace the reflected ray
		(iv) if surface is transparent (CShader.IsTransparent()), compute and trace the refracted ray
		(v)  add the weighted (CShader.GetSpecularColor(), CShader.GetRefractiveColor())
		     radiances of the secondary rays to the result of the local light model
        (vi) if no surface is hit, return the background color

		Hint: You will need an instance of the structure TTracingContext, which contains all necessary arguments for tracing rays
	*/

	TTracingContext tContext;
	if ( pclScene->Intersect(crclRay, 0., 100., tContext) )		// we have intersection with the scene
	{
		ColorType c = Shade(pclScene, tContext);
		ColorType refl = 0., refr = 0.;

		/*if(tContext.pclShader->IsSpecular())
		{
			CRay reflected = crclRay.ReflectedRay(tContext.v3HitPoint, tContext.v3Normal);
			refl = TraceRay(pclScene, reflected, uiDepth);
		}
		if(tContext.pclShader->IsTransparent())
		{
			CRay refracted;
			if(RefractedRay(tContext.v3HitPoint, tContext.v3Normal, tContext.pclShader->GetRefractiveIndex(), refracted))
			{
				refr = TraceRay(pclScene, refracted, uiDepth);
			}
		}*/
	}



	return ColorType(0.);

}


//-----------------------------------------------------------------------------

ColorType
CWhitted::Shade( const CSurface*   pclScene,
				 TTracingContext&  tContext )
{
	/*
		Implement:

		set ambient part of the color
		for all light sources
			compute shadow ray and intensity
			intersect shadow ray with scene
			if no intersected surface between surface and light source
				evaluate local light model (CShader.Shade())
				add result to final color

		Hints:

		The local light model is defined by the classes derived from CShader.
		The methods ShadeAmbient() and Shade() should be used here. The current shader
		can be taken from argument tContext.
		The light sources can be found in the STL-container m_LightList.
		Light sources are derived from class CLight and implement the method ShadowFeeler()
		which can be used to compute ray direction, domain and intensity.
		Use these to initialize the variables tContext.v3Incoming and tContext.colLightIntensity
		before evaluating the local light model using Shade()
	*/

	ColorType color(0.);

	color = tContext.pclShader->ShadeAmbient(1.0);

	for (int i = 0; i < m_LightList.size(); ++i)
	{
		LightListType::iterator it = m_LightList.begin();
		std::advance(it, i);

		CRay shadow_ray;
		shadow_ray.SetOrigin(tContext.v3HitPoint);
		RealType domain;
		ColorType col;
		(*it)->ShadowFeeler(1, shadow_ray, domain, col);
		TTracingContext tContext_shadow;
		
		if(!pclScene->Intersect(shadow_ray, 0., 100., tContext_shadow))	// if no intersection with scene ...
		{
			tContext.v3Incoming = shadow_ray.GetDir();
			tContext.colLightIntensity = col;
			color += tContext.pclShader->Shade(tContext);
		}
	}

	return color;
}

//-----------------------------------------------------------------------------

void
CWhitted::RemoveAllLights()
{
	if (m_bOwnsLights)
	{
		LightListType::iterator it     = m_LightList.begin();
		LightListType::iterator it_end = m_LightList.end();
		for( ;it != it_end; ++it)
		{
			delete (*it);
		}
	}
	m_LightList.clear();
}

//=============================================================================
}; // namespace RAYTRACER
//=============================================================================



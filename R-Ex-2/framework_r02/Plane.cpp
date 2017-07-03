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
//  CLASS CSurfacePlane - IMPLEMENTATION
//
//=============================================================================

#define RAYTRACER_SURFACEPLANE_CPP

//== INCLUDES =================================================================
#include "Plane.h"

//== NAMESPACE =============================================================== 

namespace RAYTRACER {
   
//== IMPLEMENTATION ========================================================== 

void
CPlane::ConstructONB(const VectorType3& v, VectorType3& x, VectorType3& y, VectorType3& z ) const
{
	z = v;
	z.normalize();
	
	//construct an orthogonal vector
	if ( fabs(z[0]) < fabs(z[1]) && fabs(z[0]) < fabs(z[2]) )
	{
		x[0] = 0; x[1] = z[2]; x[2] =-z[1];
	}
	else if (fabs(z[1]) < fabs(z[2]))
	{
		x[0] = z[2]; 
		x[1] = 0;
		x[2] = -z[0];
	}
	else
	{
		x[0] = z[1];
		x[1] =-z[0];
		x[2] = 0;
	}
	x.normalize();
	y = x%z;
}

//-----------------------------------------------------------------------------

bool 
CPlane::Intersect(const CRay& clRay, RealType t0, RealType t1, TTracingContext& tContext ) const
{
	RealType f_denominator = m_v3Normal | clRay.GetDir();
		
	if ( fabs( f_denominator ) < FLT_EPSILON ) return false;
	RealType t = (m_v3Normal | (m_v3Point - clRay.GetOrigin())) / f_denominator;
	if (t > t0 && t < t1)
	{
		tContext.t            = t;
		tContext.v3HitPoint   = clRay.Evaluate(t);
		tContext.v3Normal     = m_v3Normal;
		
		VectorType3 v3_in_plane = tContext.v3HitPoint - m_v3Point;
		tContext.v2TexCoord    = VectorType2(m_v3X|v3_in_plane, m_v3Y|v3_in_plane);

		tContext.pclShader    = GetShader();
		return true;
	 }
	return false;
}


//=============================================================================
}; // namespace RAYTRACER
//=============================================================================



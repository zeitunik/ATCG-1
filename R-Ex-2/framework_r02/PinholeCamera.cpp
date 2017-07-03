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
//  CLASS CPinholeCamera - IMPLEMENTATION
//
//=============================================================================

#define RAYTRACER_PINHOLECAMERA_CPP

//== INCLUDES =================================================================
#include "PinholeCamera.h"

//== NAMESPACE ===============================================================

namespace RAYTRACER {


//== IMPLEMENTATION ==========================================================

void
CPinholeCamera::MakeONB()
{
  VectorType3 w = (m_v3LookAt-m_v3Eye).normalize();
  VectorType3 u = (w%m_v3Up).normalize();
  VectorType3 v =  u%w;

  m_m33ONB.SetRow(0, u);
  m_m33ONB.SetRow(1, v);
  m_m33ONB.SetRow(2, w);
}

//-----------------------------------------------------------------------------

CRay
CPinholeCamera::GenerateRay(RealType x, RealType y, RealType rWidth, RealType rHeight) const
{
	/*
	You have to implement the generation of a ray throuh the eye point and the center of the given pixel (x,y) on an image plane of size (rWidth,rHeight)
	You can use the 3x3 matrix m_m33ONB which contains an orthonormal basis of the camera coordinate system with axes (right, up, view).
	m_rFocalLength contains the distance from the eye point to the image plane and (m_rLeft, m_rBottom, m_rRight, m_rTop) the size of
	the image on the image plane in the orthonormal basis.
	*/

	CRay ray;
	VectorType3 eye = GetEye();
	ray.SetOrigin(eye);
	
	RealType left, bottom, right, top;
	GetWindow(left, bottom, right, top);

	RealType x_step = (right - left) / rWidth;
	RealType y_step = (top - bottom) / rHeight;

	// VectorType3 look_dir = (GetLookAt()-eye).normalize();

	VectorType3 dir(left + x*x_step + x_step/2., bottom + y*y_step + y_step/2., GetFocalLength());
	dir = m_m33ONB.Transpose()*dir + eye;		// transform to world coordinates
	// std::cout << x << "\t" << y << "\t" << dir << std::endl;
	
	dir -= eye;
	dir.normalize();

	ray.SetDir(dir);

	return ray;
};



//=============================================================================
}; // namespace RAYTRACER
//=============================================================================

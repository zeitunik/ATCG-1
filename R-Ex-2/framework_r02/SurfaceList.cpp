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
//  CLASS CSurfaceList - IMPLEMENTATION
//
//=============================================================================

#define RAYTRACER_SURFACELIST_CPP


//== INCLUDES =================================================================
#include "SurfaceList.h"

//== NAMESPACE =============================================================== 

namespace RAYTRACER {
   
//== IMPLEMENTATION ========================================================== 


void
CSurfaceList::RemoveAllSurfaces()
{
	if (m_bOwnsSurfaces)
	{
		SurfaceContainerType::iterator it = m_SurfaceContainer.begin();
		SurfaceContainerType::iterator it_end = m_SurfaceContainer.end();
		for( ;it != it_end; ++it)
		{
			delete (*it);
		}		
	}
	m_SurfaceContainer.clear();
}

//-----------------------------------------------------------------------------

bool  
CSurfaceList::Intersect( const CRay&       clRay, 
	   			         const RealType    t0, 
				         const RealType    t1, 
					     TTracingContext&  tContext ) const
{
	if ( !GetAABB().Intersect(clRay) ) return false;
	bool b_ret = false;

	tContext.t = t1;
	SurfaceContainerType::const_iterator it = m_SurfaceContainer.begin();
	SurfaceContainerType::const_iterator it_end = m_SurfaceContainer.end();
	for( ;it != it_end; ++it)
	{
		//The last ray-intersection parameter restricts the next intersection
		if ( (*it)->Intersect(clRay, t0, tContext.t, tContext) )
		{			
			b_ret = true;
		}
	}
	return b_ret;
}


//=============================================================================
}; // namespace RAYTRACER
//=============================================================================



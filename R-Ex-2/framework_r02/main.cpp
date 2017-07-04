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
//  This file contains two scene definitions (call Scene0() or Scene1()),
//  as well as the main rendering loop.
//
//=============================================================================

#include <iostream>
#include <thread>

#include "Block.h"
#include "GUI.h"
#include "Timer.h"
#include "Types.h"

#include "PointLight.h"
#include "DirectionalLight.h"
#include "Plane.h"
#include "Sphere.h"

using namespace RAYTRACER;

static const int I_IMAGE_SIZE_X = 512;
static const int I_IMAGE_SIZE_Y = 512;
// select which scene to render (can be 0 or 1)
static const int I_SCENE = 1;
// maximum ray depth
static const int I_MAX_DEPTH = 10;
// block size used for parallelization
static const int I_BLOCK_SIZE = 32;

// Raytracer
CWhitted		m_clWhittedRaytracer;
CSurfaceList	m_clScene;

CPhongShader*	m_pclSphereShader0;
CGlassShader*	m_pclSphereShader2;
CLambertShader*	m_pclSphereShader1;
CTextureShader*	m_pclPlaneShader;

//Checkerboard plane with metallic spheres
void Scene0() {
	m_clWhittedRaytracer.GetCamera().SetEye( VectorType3(0, 0.5, 2.5) );
//Lights
	CDirectionalLight* pcl_dir_light0 = new CDirectionalLight( VectorType3(1, 1, 2), ColorType(0.5) );
	CDirectionalLight* pcl_dir_light1 = new CDirectionalLight( VectorType3(-1, 1, 1), ColorType(0.5) );
	m_clWhittedRaytracer.AddLight( pcl_dir_light0 );
	m_clWhittedRaytracer.AddLight( pcl_dir_light1 );

//Shader
	//Metal
	m_pclSphereShader0 = new CPhongShader( ColorType(0.035, 0.0325, 0.025), ColorType(0.8), ColorType(0.), 1, ColorType(0.5, 0.45, 0.35), ColorType(1.), 120 );
	m_pclSphereShader1 = 0;
	//Checkerboard pattern
	m_pclPlaneShader  = new CTextureShader( ColorType(0.1), ColorType(0.25), ColorType(0.), 1, VectorType2(1., 1.));
	std::vector<ColorType> cl_texture(4);
	ColorType col_plane(1, 0.75, 0.33);
	cl_texture[0] = col_plane;
	cl_texture[1] = ColorType(1.);
	cl_texture[2] = ColorType(1.);
	cl_texture[3] = col_plane;
	m_pclPlaneShader->SetTexture(cl_texture, 2, 2);

//Spheres
	CSphere* pcl_sphere = new CSphere( VectorType3(0.272166, 0.272166, 0.544331), 0.166667 );
	pcl_sphere->SetShader(m_pclSphereShader0);
	m_clScene.AddSurface( pcl_sphere );

	pcl_sphere = new CSphere( VectorType3(0.643951, 0.172546, 0), 0.166667 );
	pcl_sphere->SetShader(m_pclSphereShader0);
	m_clScene.AddSurface( pcl_sphere );

	pcl_sphere = new CSphere( VectorType3(0.172546, 0.643951, 0 ), 0.166667 );
	pcl_sphere->SetShader(m_pclSphereShader0);
	m_clScene.AddSurface( pcl_sphere );

	pcl_sphere = new CSphere( VectorType3(-0.371785, 0.0996195, 0.544331), 0.166667 );
	pcl_sphere->SetShader(m_pclSphereShader0);
	m_clScene.AddSurface( pcl_sphere );

	pcl_sphere = new CSphere( VectorType3(-0.471405, 0.471405, 0 ), 0.166667 );
	pcl_sphere->SetShader(m_pclSphereShader0);
	m_clScene.AddSurface( pcl_sphere );

	pcl_sphere = new CSphere( VectorType3(-0.643951, -0.172546, 0 ), 0.166667 );
	pcl_sphere->SetShader(m_pclSphereShader0);
	m_clScene.AddSurface( pcl_sphere );

	pcl_sphere = new CSphere( VectorType3(0.0996195, -0.371785, 0.544331 ), 0.166667 );
	pcl_sphere->SetShader(m_pclSphereShader0);
	m_clScene.AddSurface( pcl_sphere );

	pcl_sphere = new CSphere( VectorType3(-0.172546, -0.643951, 0 ), 0.166667 );
	pcl_sphere->SetShader(m_pclSphereShader0);
	m_clScene.AddSurface( pcl_sphere );

	pcl_sphere = new CSphere( VectorType3(0.471405, -0.471405, 0 ), 0.166667 );
	pcl_sphere->SetShader(m_pclSphereShader0);
	m_clScene.AddSurface( pcl_sphere );

	pcl_sphere = new CSphere( VectorType3(0, 0, 0), 0.5 );
	pcl_sphere->SetShader(m_pclSphereShader0);
	m_clScene.AddSurface( pcl_sphere );

//Plane
	CPlane* pcl_plane = new CPlane( VectorType3(0, 1, 0), VectorType3( 0, -0.8, 0) );
	pcl_plane->SetShader( m_pclPlaneShader );
	m_clScene.AddSurface( pcl_plane );
}


//-----------------------------------------------------------------------------

//Checkerboard plane with refractive sphere
void Scene1() {
	m_clWhittedRaytracer.GetCamera().SetEye( VectorType3(0, 0, 4) );
//Lights
	CDirectionalLight* pcl_dir_light0 = new CDirectionalLight( VectorType3(1., 1., 2.), ColorType(0.75, 0.75, 0.75) );
	m_clWhittedRaytracer.AddLight( pcl_dir_light0 );

// Shader
	//Glass
	m_pclSphereShader2 = new CGlassShader();
	//diffuse red plastic
	m_pclSphereShader1 = new CLambertShader( ColorType(0.35, 0.032, 0.025), ColorType(0.8, 0.05, 0.06) );
	//Checkerboard
	m_pclPlaneShader  = new CTextureShader( ColorType(0.1, 0.1, 0.1), ColorType(0.25, 0.25, 0.25), ColorType(0., 0., 0.), 1, VectorType2(1., 1.));
	std::vector<ColorType> cl_texture(4);
	ColorType col_plane(1, 0.75, 0.33);
	cl_texture[0] = col_plane;
	cl_texture[1] = ColorType(1.);
	cl_texture[2] = ColorType(1.);
	cl_texture[3] = col_plane;
	m_pclPlaneShader->SetTexture(cl_texture, 2, 2);

//Spheres
	CSphere* pcl_sphere = new CSphere( VectorType3(0, 0, 1.2), 0.5);
	pcl_sphere->SetShader(m_pclSphereShader2);
	m_clScene.AddSurface( pcl_sphere );

	pcl_sphere = new CSphere( VectorType3(-0.75, 0.5, 0.2), 0.5);
	pcl_sphere->SetShader(m_pclSphereShader1);
	m_clScene.AddSurface( pcl_sphere );

//Plane
	CPlane* pcl_plane = new CPlane( VectorType3(0, 0, 1), VectorType3( 0, 0, -0.5) );
	pcl_plane->SetShader( m_pclPlaneShader );
	m_clScene.AddSurface( pcl_plane );
}


static void renderBlock(ImageBlock &block) {
	CPinholeCamera camera = m_clWhittedRaytracer.GetCamera();

	Point2i offset = block.getOffset();
	Vector2i size  = block.getSize();

	// clear block contents
	block.clear();

	// for each pixel and pixel sample sample
	for (int y=0; y<size.y(); ++y) {
		for (int x=0; x<size.x(); ++x) {
			RealType px = (RealType) (x + offset.x());
			RealType py = (RealType) (y + offset.y());

			// sample ray from camera
			CRay ray = camera.GenerateRay(px, py, I_IMAGE_SIZE_X, I_IMAGE_SIZE_Y);

			// we should use a reconstruction filter here, however, for simplicity we only average the pixel samples
			ColorType col = m_clWhittedRaytracer.TraceRay(&m_clScene, ray, I_MAX_DEPTH);

			// store in image block
			block.put(x, y, col);
		}
	}
}

int main( int argc, char **argv ) {
	//Global settings
	m_clWhittedRaytracer.GetCamera().Reset();
	m_clWhittedRaytracer.SetBackgroundColor( ColorType(	0.078, 0.361, 0.753) );
	m_clWhittedRaytracer.SetAmbientColor( ColorType(0.5, 0.5, 0.5) );

	//Load scene
	switch (I_SCENE) {
	case 0: Scene0();
			break;
	case 1: Scene1();
			break;
	default: Scene0();
			break;
	}

	unsigned nproc = std::thread::hardware_concurrency();
	if (!nproc) {
		nproc = 1;
	}

	Vector2i outputSize = Vector2i(I_IMAGE_SIZE_X, I_IMAGE_SIZE_Y);

	/* Create a block generator (i.e. a work scheduler) */
	BlockGenerator blockGenerator(outputSize, I_BLOCK_SIZE);

	/* Allocate memory for the entire output image and clear it */
	ImageBlock result(outputSize);
	result.clear();

	/* Create a window that visualizes the partially rendered result */
	nanogui::init();
	gui *screen = new gui(result);

	/* Do the following in parallel and asynchronously */
	std::thread render_thread([&] {
		std::cout << "Rendering .. ";
		std::cout.flush();
		Timer timer;

		auto map = [&]() {
			/* Allocate memory for a small image block to be rendered
			   by the current thread */
			ImageBlock block(Vector2i(I_BLOCK_SIZE, I_BLOCK_SIZE));

			/* Request an image block from the block generator */
			while (blockGenerator.next(block)) {
				/* Render all contained pixels */
				renderBlock(block);

				/* The image block has been processed. Now add it to
				   the "big" block that represents the entire image */
				result.put(block);
			}
		};

		/// Uncomment the following line for single threaded rendering
		//map(range);

		/// Default: parallel rendering
		std::vector<std::thread> render_threads;
		for (int i = 0; i < nproc; ++i) {
			render_threads.emplace_back(std::bind(map));
		}

		for (auto &thread : render_threads) {
			thread.join();
		}

		std::cout << "done. (took " << timer.elapsedString() << ")" << std::endl;
	});

	/* Enter the application main loop */
	nanogui::mainloop();

	/* Shut down the user interface */
	render_thread.join();

	delete screen;
	nanogui::shutdown();

	return 0;
}

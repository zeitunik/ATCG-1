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
//  GUI - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include <iostream>

#include "GUI.h"
#include <nanogui/button.h>
#include <nanogui/label.h>
#include <nanogui/layout.h>
#include <nanogui/slider.h>
#include <nanogui/glutil.h>
#include <nanogui/textbox.h>
#include <nanogui/widget.h>
#include <nanogui/checkbox.h>

#include "CImg.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Plane.h"
#include "Sphere.h"

namespace RAYTRACER {

#define UI_OFFSET 150
#define UI_BORDER 10

gui::gui(ImageBlock& block)
 : nanogui::Screen(block.getSize() + Vector2i(UI_OFFSET, 0), "ATCG1 R02", true), 
   m_block(block) {
	
    using namespace nanogui;

	// add some UI elements
    Widget *panel = new Widget(this);
	panel->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, UI_BORDER, UI_BORDER));

	// very basic tone mapping
    new Label(panel, "Exposure value: ", "sans-bold");
    m_slider = new Slider(panel);
    m_slider->setValue(0.5f);
	m_slider->setFixedWidth(UI_OFFSET - UI_BORDER * 2);
    m_slider->setCallback(
        [&](float value) {
            m_scale = std::pow(2.f, (value - 0.5f) * 20);
        }
	);

	m_checkbox_sRGB = new CheckBox(panel, "sRGB");
	m_checkbox_sRGB->setChecked(m_sRGB);
	m_checkbox_sRGB->setCallback(
		[&](bool value) {
			m_sRGB = value;
		}
	);

	// save output
	new Label(panel, "Export: ", "sans-bold");
	m_text_filename = new TextBox(panel, "rendering.bmp");
	m_text_filename->setFontSize(16);
	m_text_filename->setEditable(true);

	m_button_save = new Button(panel, "save");
	m_button_save->setFontSize(16);
	m_button_save->setCallback(
				[&]() {
		int width = block.getSize().x();
		int height = block.getSize().y();
		std::vector<float> tmp(width * height * 3);
		for (int i = 0; i < width * height; i++) {
			tmp[i + 0 * width * height] = (uint8_t) 255. * block.coeff(i)[0];
			tmp[i + 1 * width * height] = (uint8_t) 255. * block.coeff(i)[1];
			tmp[i + 2 * width * height] = (uint8_t) 255. * block.coeff(i)[2];
		}

		cimg_library::CImg<uint8_t> img(&tmp[0], width, height, 1, 3);
		img.save(m_text_filename->value().c_str());
	});

	panel->setSize(block.getSize());
    performLayout(mNVGContext);
	
	panel->setPosition(Vector2i(0, 0));

    // simple gamma tonemapper as a GLSL shader
    m_shader = new GLShader();
	m_shader->init(
        "Tonemapper",

        /* Vertex shader */
        "#version 330\n"
        "in vec2 position;\n"
        "out vec2 uv;\n"
        "void main() {\n"
        "    gl_Position = vec4(position.x*2-1, position.y*2-1, 0.0, 1.0);\n"
        "    uv = vec2(position.x, 1-position.y);\n"
        "}",

        /* Fragment shader */
        "#version 330\n"
        "uniform sampler2D source;\n"
		"uniform int srgb;\n"
        "uniform float scale;\n"
        "in vec2 uv;\n"
        "out vec4 out_color;\n"
        "float toSRGB(float value) {\n"
        "    if (value < 0.0031308)\n"
        "        return 12.92 * value;\n"
        "    return 1.055 * pow(value, 0.41666) - 0.055;\n"
        "}\n"
        "void main() {\n"
        "    vec4 color = texture(source, uv);\n"
        "    color *= scale / color.w;\n"
		"    if (srgb == 1) {\n"
		"        out_color = vec4(toSRGB(color.r), toSRGB(color.g), toSRGB(color.b), 1);\n"
		"    } else {\n"
		"        out_color = vec4(color.rgb, 1);\n"
		"    }\n"
        "}"
    );

    MatrixXu indices(3, 2); // draw 2 triangles
    indices.col(0) << 0, 1, 2;
    indices.col(1) << 2, 3, 0;

    MatrixXf positions(2, 4);
    positions.col(0) << 0, 0;
    positions.col(1) << 1, 0;
    positions.col(2) << 1, 1;
    positions.col(3) << 0, 1;

    m_shader->bind();
    m_shader->uploadIndices(indices);
    m_shader->uploadAttrib("position", positions);

    // allocate texture memory for the rendered image
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    drawAll();
	setVisible(true);
}

gui::~gui() {
    glDeleteTextures(1, &m_texture);
    delete m_shader;
}

void gui::drawContents() {
	m_block.lock();
	const Vector2i size = m_block.getSize();

	// convert block to float for GPU upload
	int nc = sizeof(ColorType) / sizeof(RealType);
	std::vector<float> pixels(size.x() * size.y() * nc);
	for (int i = 0; i < size.x() * size.y(); i++) {
		for (int c = 0; c < nc; c++) {
			pixels[nc * i + c] = (float) m_block.coeff(i)[c];
		}
	}
	int borderSize = m_block.getBorderSize();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, (GLint) m_block.cols());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, size.x(), size.y(),
			0, GL_RGB, GL_FLOAT, (uint8_t *) &pixels[0] +
			(borderSize * m_block.cols() + borderSize) * sizeof(ColorType));
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	m_block.unlock();

	glViewport(GLsizei(UI_OFFSET * mPixelRatio), 0, GLsizei(mPixelRatio * size[0]),
		 GLsizei(mPixelRatio * size[1]));
    m_shader->bind();
	m_shader->setUniform("scale", m_scale);
	m_shader->setUniform("srgb", (int) m_sRGB);
    m_shader->setUniform("source", 0);
    m_shader->drawIndexed(GL_TRIANGLES, 0, 2);
    glViewport(0, 0, mFBSize[0], mFBSize[1]);
}

}

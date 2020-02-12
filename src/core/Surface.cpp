//
//  Surface.cpp
//  PixEngine
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "Surface.hpp"
#include "OpenGlUtils.h"
#include "png.h"
#include "PixEngine.hpp"
#include "Shader.hpp"

using namespace rgl;

constexpr float Surface::vertices[32] ;
constexpr unsigned int Surface::indices[6];

Surface::Surface(int width, int height, std::string name):nWidth(width), nHeight(height) {
	pShader = new Shader(name);
	pActiveDrawable = new Drawable(width, height);
}

Surface::~Surface() {
	delete pShader;
	delete pActiveDrawable;
	pShader = nullptr;
	pActiveDrawable = nullptr;
}

bool Surface::init_opengl() {
	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	
	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Surface::vertices), Surface::vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Surface::indices), Surface::indices, GL_STATIC_DRAW);
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	glClearColor(0.0, 0.0, 0.0, 1);
	return true;
}

void Surface::init_texture() {
	
	glGenTextures(1, &glBuffer);
	glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_2D, glBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
#if !defined __APPLE__ && !defined ANDROID
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
#endif
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, pActiveDrawable->getData());
	
	pShader->use();
	pShader->setInt("glbuffer", 0);
}

void Surface::reset_texture() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, glBuffer);
}

void Surface::tick() {
	
	// TODO: This is a bit slow (especially in debug, but 100x faster in release mode???)
	// Copy pixel array into texture
	glActiveTexture(GL_TEXTURE0);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nWidth, nHeight, GL_RGBA, GL_UNSIGNED_BYTE, pActiveDrawable->getData());
	glBindTexture(GL_TEXTURE_2D, glBuffer);
	pShader->use();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Surface::deinit() {
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

Drawable *Surface::buffer() { return pActiveDrawable; }

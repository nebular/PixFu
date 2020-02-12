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
#include "Texture2D.hpp"

using namespace rgl;

constexpr float Surface::vertices[32] ;
constexpr unsigned int Surface::indices[6];

Surface::Surface(int width, int height, std::string name):nWidth(width), nHeight(height) {
	pShader = new Shader(name);
	pActiveTexture = new Texture2D(width, height);
}

Surface::~Surface() {
	delete pShader;
	delete pActiveTexture;
	pShader = nullptr;
	pActiveTexture = nullptr;
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
	pActiveTexture->upload();
	pShader->use();
	pShader->setInt("glbuffer", pActiveTexture->unit());
}

void Surface::tick() {
	// Copy pixel array into texture
	pActiveTexture->update();
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

Drawable *Surface::buffer() { return pActiveTexture->buffer(); }

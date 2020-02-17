#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
//
//  Surface.cpp
//  PixFu
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "png.h"
#include "PixFu.hpp"
#include "Utils.hpp"
#include "Shader.hpp"
#include "Layer.hpp"
#include "Texture2D.hpp"
#include "OpenGlUtils.h"

using namespace rgl;

const std::string Layer::TAG = "Layer";


Layer::~Layer() {
	if (DBG) LogV(TAG, "Layer destroyed");
}

void Layer::init(float *vertices, int numvertices, unsigned int *indices, int numindices ) {
	pVertices = vertices;
	nVertices = numvertices;
	pIndices = indices;
	nIndices = numindices;
	initLayer();
}

void Layer::initLayer() {
	
	if (DBG) LogV(TAG, SF("Init OpenGL, %d vertices, %d indices", nVertices, nIndices));
	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	
	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, nVertices, pVertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices, pIndices, GL_STATIC_DRAW);
	
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
	glEnableVertexAttribArray(0);
	
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  (void *) (6 * sizeof(float)));
	
	glEnableVertexAttribArray(2);
	
	//	glEnable(GL_DEPTH_TEST);
	//	glDepthFunc(GL_LESS);
	
	if (DBG) OpenGlUtils::glError("surface initopengl");
}

void Layer::draw() {
	// Copy pixel array into texture
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Layer::deinit() {
	
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	
	if (DBG) LogV(TAG, "deinit");
	if (DBG) OpenGlUtils::glError("surface initopengl");
}



#pragma clang diagnostic pop

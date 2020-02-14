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
#include "Surface.hpp"
#include "Texture2D.hpp"
#include "OpenGlUtils.h"

using namespace rgl;

const std::string Surface::TAG = "Surface";

constexpr float Surface::VERTICES[32];
constexpr unsigned int Surface::INDICES[6];

Surface::Surface(int width, int height, const std::string &samplerName,
				 const std::string &shaderName)
		: nWidth(width), nHeight(height), sSamplerName(samplerName) {

	pShader = new Shader(shaderName);
	pActiveTexture = new Texture2D(width, height);

	if (DBG) LogV(TAG, SF("Creating %dx%d, sampler %s, shader %s", width, height, samplerName.c_str(),
				 shaderName.c_str()));

}

Surface::~Surface() {
	delete pShader;
	delete pActiveTexture;
	pShader = nullptr;
	pActiveTexture = nullptr;
	if (DBG) LogV(TAG, "Surface destroyed");
}

void Surface::init_opengl() {

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Surface::VERTICES), Surface::VERTICES, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Surface::INDICES), Surface::INDICES,
				 GL_STATIC_DRAW);
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

	glClearColor(0.0, 0.0, 0.0, 1);

	if (DBG) OpenGlUtils::glError("surface initopengl");
}

void Surface::init_texture() {
	pActiveTexture->upload();
	pShader->use();
	pShader->setInt(sSamplerName, pActiveTexture->unit());
}

bool Surface::init() {
	init_opengl();
	init_texture();
	return pShader != nullptr && pActiveTexture != nullptr;
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

	if (DBG) LogV(TAG, "deinit");
	if (DBG) OpenGlUtils::glError("surface initopengl");
}

Drawable *Surface::buffer() { return pActiveTexture->buffer(); }

#pragma clang diagnostic pop
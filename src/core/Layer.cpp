#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"

//
//  Layer.cpp
//  PixFu
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "PixFu.hpp"
#include "Utils.hpp"
#include "Layer.hpp"
#include "Texture2D.hpp"
#include "OpenGlUtils.h"

namespace rgl {

	const std::string Layer::TAG = "Layer";

	Layer::~Layer() {
		if (DBG) LogV(TAG, "Layer destroyed");
	}

	void Layer::setup(std::vector<Vertex> &vertices, std::vector<unsigned> &indices) {
		setup(
				(float *) &vertices[0],
				vertices.size(),
				&indices[0],
				indices.size()
		);
	}

	void Layer::setup(float *vertices, unsigned numvertices, unsigned *indices, unsigned numindices) {
		pVertices = vertices;
		nVertices = numvertices;
		pIndices = indices;
		nIndices = numindices;
		init();
	}

	void Layer::init() {

		LogE("GL", SF("glGenVertexArrays VAO + genVBO + genEBO + bindbuffer + vertexAttribPointer"));

		// generate and bind VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// generate vbo
		glGenBuffers(1, &vbo);
		// bind and fill vbo
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, nVertices * 8 * sizeof(float), pVertices, GL_STATIC_DRAW);

		// generate ebo
		glGenBuffers(1, &ebo);
		if (DBG) OpenGlUtils::glError("surface initopengl1");

		// store data in attribute list
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(unsigned int), pIndices, GL_STATIC_DRAW);


		// setup vertex attributes

		// pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);

		// normals attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));

		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));


		glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		if (DBG) OpenGlUtils::glError("surface initopengl2");
		if (DBG) LogV(TAG, SF("Created buffers, %d vertices, %d indices, VAO %d", nVertices, nIndices, vao));

		glBindVertexArray(0);

	}

	void Layer::bind() {
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		if (DBG) OpenGlUtils::glError("bind");

	}

	void Layer::draw(bool bindBuffers) {

		// optimization:
		// if drawing objects with the same VAO we will bind just once
		// then just loop drawElements

		if (bindBuffers) bind();
		glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
		if (bindBuffers) unbind();
	}

	void Layer::unbind() {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
		if (DBG) OpenGlUtils::glError("unbind");
	}

	void Layer::deinit() {
		unbind();

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);

		if (DBG) LogV(TAG, "deinit");
		if (DBG) OpenGlUtils::glError("surface initopengl");
	}

}
#pragma clang diagnostic pop

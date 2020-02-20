#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"

//
//  Layer.cpp
//  PixFu
//
//  A VAO Layer, abstracts drawing a mesh from an array of vertexes and indices.
//  Vertexes are interleaved <POS - NORM - TEXCOORDS> so it uses only one buffer
//  The VAO buffers are constructed by calling the init() method, and from then on
//  draw() to draw the mesh.
//
//  This class does not know about shaders, derived classes are expected to take care
//  of that as needed.
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "PixFu.hpp"
#include "Utils.hpp"
#include "LayerVao.hpp"
#include "Texture2D.hpp"
#include "OpenGlUtils.h"

namespace rgl {

	const std::string LayerVao::TAG = "LayerVao";

	LayerVao::~LayerVao() {
		if (DBG) LogV(TAG, "Layer destroyed");
	}

	void LayerVao::setup(std::vector<Vertex_t> &vertices, std::vector<unsigned> &indices) {
		setup(
				(float *) &vertices[0],
				(unsigned) vertices.size(),
				&indices[0],
				(unsigned) indices.size()
		);
	}

	void LayerVao::setup(float *vertices, unsigned numvertices, unsigned *indices, unsigned numindices) {
		pVertices = vertices;
		nVertices = numvertices;
		pIndices = indices;
		nIndices = numindices;
		init();
	}

	void LayerVao::init() {

		LogE("GL", SF("glGenVertexArrays VAO + genVBO + genEBO + bindbuffer + vertexAttribPointer"));

		// generate and bind VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// generate vbo
		glGenBuffers(1, &vbo);
		// bind and fill vbo
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(nVertices * 8 * sizeof(float)), pVertices, GL_STATIC_DRAW);

		// generate ebo
		glGenBuffers(1, &ebo);
		if (DBG) OpenGlUtils::glError("surface initopengl1");

		// store data in attribute list
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(nIndices * sizeof(unsigned int)), pIndices, GL_STATIC_DRAW);


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

	void LayerVao::bind() {
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		if (DBG) OpenGlUtils::glError("bind");

	}

	void LayerVao::draw(bool bindBuffers) {

		// optimization:
		// if drawing objects with the same VAO we will bind just once
		// then just loop drawElements

		if (bindBuffers) bind();
		glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
		if (bindBuffers) unbind();
	}

	void LayerVao::unbind() {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
		if (DBG) OpenGlUtils::glError("unbind");
	}

	void LayerVao::deinit() {
		unbind();

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);

		if (DBG) LogV(TAG, "deinit");
		if (DBG) OpenGlUtils::glError("surface initopengl");
	}

}
#pragma clang diagnostic pop

//
//  Layer.cpp
//  PixFu
//
//  A VAO Layer, abstracts drawing a group of meshes from  arrays of vertexes and indices.
//  Vertexes are interleaved <POS - NORM - TEXCOORDS> so it uses only one VAO buffer per mesh
//  You can add meshes calling the add() method, init them all callint init(), and from then on
//  just draw(i) to draw the mesh
//
//  This class does not know about shaders, derived classes are expected to take care
//  of that as needed. @see Surface.cpp, Terrain.cpp or ObjectCluster.cpp
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
#pragma ide diagnostic ignored "RedundantCast"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"

#include "Fu.hpp"
#include "Utils.hpp"
#include "LayerVao.hpp"
#include "Texture2D.hpp"
#include "OpenGlUtils.h"

namespace Pix {

	const std::string LayerVao::TAG = "LayerVao";

	// debug mode may change it to LINES
	GLenum LayerVao::DRAWMODE = GL_TRIANGLES;

	LayerVao::~LayerVao() {
		if (DBG) LogV(TAG, "Layer destroyed");
	}

	unsigned LayerVao::add(std::vector<Vertex_t> &vertices, std::vector<unsigned> &indices) {
		return add(
				(float *) &vertices[0],
				(unsigned) vertices.size(),
				&indices[0],
				(unsigned) indices.size()
		);
	}

	unsigned LayerVao::add(float *vertices, unsigned numvertices, unsigned *indices, unsigned numindices) {
		Mesh_t mesh = {vertices, numvertices, indices, numindices};
		init(mesh);
		vMeshes.push_back(mesh);
		return (unsigned) vMeshes.size();
	}

	void LayerVao::init(Mesh_t &mesh) {

		// generate and bind VAO
		glGenVertexArrays(1, &mesh.vao);
		glBindVertexArray(mesh.vao);

		// generate vbo
		glGenBuffers(1, &mesh.vbo);

		// bind and fill vbo
		glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(mesh.nVertices * 8 * sizeof(float)), mesh.pVertices, GL_STATIC_DRAW);

		// generate ebo
		glGenBuffers(1, &mesh.ebo);
		if (DBG) OpenGlUtils::glError("LayerVAO init");

		// store data in attribute list
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(mesh.nIndices * sizeof(unsigned int)), mesh.pIndices, GL_STATIC_DRAW);


		// setup vertex attributes

		// pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);

		// normals attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));

		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));


		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (DBG) OpenGlUtils::glError("LayerVao Attribs");
		if (DBG) LogV(TAG, SF("Mesh setup, %d vertices, %d indices, VAO %d", mesh.nVertices, mesh.nIndices, mesh.vao));

		glBindVertexArray(0);

	}

	void LayerVao::bind(int index) {
		Mesh_t &mesh = vMeshes[index];
		glBindVertexArray(mesh.vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
//		glEnable(GL_CULL_FACE);
		if (DBG) OpenGlUtils::glError("bind mesh");

	}

	void LayerVao::draw(int index, bool bindBuffers) {

		Mesh_t &mesh = vMeshes[index];
		if (bindBuffers) bind(index);
		glDrawElements(DRAWMODE, mesh.nIndices, GL_UNSIGNED_INT, 0);
		if (bindBuffers) unbind();
	}

	void LayerVao::unbind() {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
//		glDisable(GL_CULL_FACE);
		if (DBG) OpenGlUtils::glError("unbind");
	}

	void LayerVao::deinit() {

		unbind();

		for (Mesh_t &mesh:vMeshes) {
			glDeleteVertexArrays(1, &mesh.vao);
			glDeleteBuffers(1, &mesh.vbo);
			glDeleteBuffers(1, &mesh.ebo);
		}

		if (DBG) LogV(TAG, "deinit");
		if (DBG) OpenGlUtils::glError("LayerVao deinit");
	}

}

#pragma clang diagnostic pop

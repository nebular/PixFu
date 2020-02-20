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

#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma once

#include "PixFuExtension.hpp"
#include <string>
#include <vector>
#include "glm.hpp"

namespace rgl {

	// The Vertex contains the position, the normal and the texture cooordinates

	typedef struct Vertex {
		glm::vec3 vertice;
		glm::vec3 normal = {0, 1, 0};
		glm::vec2 tex = {0, 0};
	} Vertex_t;

	/*
	 *  A VAO Layer, abstracts drawing a mesh from an array of vertexes and indices.
 	 *  Vertexes are interleaved <POS - NORM - TEXCOORDS> so it uses only one buffer
 	 *  The VAO buffers are constructed by calling the init() method, and from then on
 	 *  draw() to draw the mesh.
 	 */

	class LayerVao {

		static const std::string TAG;

		unsigned nIndices = 0;
		unsigned nVertices = 0;
		unsigned *pIndices = nullptr;
		float *pVertices = nullptr;

		unsigned vbo;
		unsigned ebo;

		void init();

	protected:

		unsigned int vao = (unsigned)-1;

		void setup(float *vertices, unsigned numvertices,
				   unsigned *indices, unsigned numindices);

		void setup(std::vector<Vertex_t> &vertices, std::vector<unsigned> &indices);

		void bind();

		void unbind();

	public:

		virtual ~LayerVao();

		// called by the loop to update the surface
		void draw(bool bind = true);

		// called by the loop to finish the surface
		void deinit();

	};


	class VaoLayer : public LayerVao {

	protected:
		std::vector<Vertex_t> vVertices;
		std::vector<unsigned> vIndices;

	public:
		void build();
	};

	inline void VaoLayer::build() {
		setup(vVertices, vIndices);
	}

}

#pragma clang diagnostic pop
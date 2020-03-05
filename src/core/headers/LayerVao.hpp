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

#include "FuExtension.hpp"
#include <string>
#include <vector>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace Pix {

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

	typedef struct sMesh {
		float *pVertices = nullptr;
		unsigned nVertices = 0;
		unsigned *pIndices = nullptr;
		unsigned nIndices = 0;
		unsigned vao = (unsigned) -1;
		unsigned vbo = (unsigned) -1;
		unsigned ebo = (unsigned) -1;
	} Mesh_t;

	class LayerVao {

		static const std::string TAG;
		
		void init(Mesh_t &mesh);

	protected:

		std::vector<Mesh_t> vMeshes;

		/**
		 * Add a new mesh to render
		 * @param vertices  Vertices, must be PPP/NNN/TT
		 * @param numvertices  Number of vertices
		 * @param indices Indices
		 * @param numindices  umber of indices.
		 * @return The mesh ID
		 */
		unsigned add(float *vertices, unsigned numvertices,
					 unsigned *indices, unsigned numindices);

		/**
		 * Add a nes mesh to render
		 * @param vertices Vector of Vertexes
		 * @param indices Vector of indices
		 * @return  The mesh ID
		 */

		unsigned add(std::vector<Vertex_t> &vertices, std::vector<unsigned> &indices);

		/**
		 * Binds a mesh for GL-drawing
		 * @param index The mesh id
		 */
		void bind(int index = 0);

		/**
		 * Unbinds bound mesh
		 */

		void unbind();

	public:

		static GLenum DRAWMODE;

		virtual ~LayerVao();

		// called by the loop to update the surface
		void draw(int index = 0, bool bind = true);

		// called by the loop to finish the surface
		void deinit();

	};

}

#pragma clang diagnostic pop

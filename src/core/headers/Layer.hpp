//
//  Surface.hpp
//  PixFu
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#include "PixFuExtension.hpp"
#include "glm.hpp"
#include <string>
#include "vector"

namespace rgl {

struct Vertex {
	glm::vec3 vertice;
	glm::vec3 normal = {0,1,0};
	glm::vec2 tex = {0,0};
};

/**
 *
 *  A surface is a screen-size memory sprite that is uploaded as a opengl texture and refreshed
 *  every iteration. Surface only provides the buffer, but no methods to draw to it. This is the
 *  job of specialized classes like Font or Canvas2D, that provides lines, circles, etc ...
 *
 */

class Layer {
	
	static const std::string TAG;
	
	unsigned int nIndices = 0;
	unsigned int nVertices = 0;
	unsigned int *pIndices = nullptr;
	float *pVertices = nullptr;
	
	unsigned int vbo;
	unsigned int ebo;
	
	void init();
	
protected:
	
	unsigned int vao = -1;
	
	void setup(float *vertices, unsigned numvertices,
			   unsigned *indices, unsigned numindices);
	
	void setup(std::vector<Vertex> &vertices, std::vector<unsigned> &indices);
	
	void bind();
	
	void unbind();
	
public:
	
	// called by the loop to update the surface
	void draw(bool bind = true);
	
	~Layer();
	
	// called by the loop to finish the surface
	void deinit();
	
};


class VaoLayer : public Layer {
	
protected:
	std::vector<Vertex> vVertices;
	std::vector<unsigned> vIndices;
public:
	void build();
};

inline void VaoLayer::build() {
	setup(vVertices, vIndices);
}

}


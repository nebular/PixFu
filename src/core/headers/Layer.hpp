//
//  Surface.hpp
//  PixFu
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef LAyer_hpp
#define LAyer_hpp

#include "OpenGL.h"
#include "Drawable.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"

#include "PixFuExtension.hpp"

namespace rgl {

/**
 *
 *  A surface is a screen-size memory sprite that is uploaded as a opengl texture and refreshed
 *  every iteration. Surface only provides the buffer, but no methods to draw to it. This is the
 *  job of specialized classes like Font or Canvas2D, that provides lines, circles, etc ...
 *
 */

class Layer : public PixFuExtension {
	
	static const std::string TAG;
	
	int nIndices=0, nVertices=0;
	unsigned int *pIndices=nullptr;
	float *pVertices=nullptr;
	
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	void initLayer();

protected:
	Shader *pShader;				// shader to use

	void init(float *vertices, int numvertices, unsigned int *indices, int numindices);

	// called by the loop to update the surface
	void draw();
	

public:
		
	~Layer();
	
	
	// called by the loop to finish the surface
	void deinit();
	
	// get the backing memory buffer
	Drawable *buffer();
	
	Shader *shader();
	
};

inline Shader *Layer::shader() {
	return pShader;
}


}

#endif /* Surface_hpp */

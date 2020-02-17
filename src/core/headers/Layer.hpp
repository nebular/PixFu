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

	class Layer {

		static const std::string TAG;

		unsigned int nIndices = 0;
		unsigned int nVertices = 0;
		unsigned int *pIndices = nullptr;
		float *pVertices = nullptr;

		unsigned int vao;
		unsigned int vbo;
		unsigned int ebo;

		void initLayer();

	protected:

		void setup(float *vertices, unsigned int numvertices, unsigned int *indices,
				   unsigned int numindices);

		// called by the loop to update the surface
		void draw();


	public:

		~Layer();


		// called by the loop to finish the surface
		void deinit();

	};


}

#endif /* Surface_hpp */

//
//  Surface.hpp
//  PixFu
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef Surface_hpp
#define Surface_hpp

#include "OpenGL.h"
#include "Drawable.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"

namespace rgl {

/**
 *
 *  A surface is a screen-size memory sprite that is uploaded as a opengl texture and refreshed
 *  every iteration. Surface only provides the buffer, but no methods to draw to it. This is the
 *  job of specialized classes like Font or Canvas2D, that provides lines, circles, etc ...
 *
 */

	class Surface {

		static const std::string TAG;
		static constexpr float VERTICES[32]{
				// positions          // colors           // texture coords
				1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
				1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
				-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
		};

		static constexpr unsigned int INDICES[6]{
				0, 1, 3, // first triangle
				1, 2, 3  // second triangle
		};

		const int nWidth, nHeight;		// texture size

		Shader *pShader;				// shader to use
		Texture2D *pActiveTexture;		// opengl texture
		std::string sSamplerName;		// name of the uniform sampler in the shader

		unsigned int vao;
		unsigned int vbo;
		unsigned int ebo;

		void init_opengl();

		void init_texture();

	public:

		Surface(int width, int height, const std::string &samplerName = "glbuffer",
				const std::string &shaderName = "default");

		~Surface();

		// called by the loop to init the surface
		bool init();

		// called by the loop to update the surface
		void tick();

		// called by the loop to finish the surface
		void deinit();

		// get the backing memory buffer
		Drawable *buffer();

	};


}
#endif /* Surface_hpp */

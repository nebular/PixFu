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
 * A surface is memory bitmap rendered into an openGL texture.
 *
 */

class Surface {

	static const std::string TAG;
	static constexpr float VERTICES[32] {
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

	const int nWidth, nHeight;

	Shader *pShader;
	Texture2D *pActiveTexture;
	std::string sSamplerName;

	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	void init_opengl();
	void init_texture();

public:

	Surface(int width, int height, std::string samplerName="glbuffer", std::string shaderName="default");
	~Surface();

	bool init();
	void tick();
	void deinit();
	
	Drawable *buffer();

};


}
#endif /* Surface_hpp */

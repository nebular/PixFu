//
//  Surface.hpp
//  PixEngine
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef Surface_hpp
#define Surface_hpp

#include "OpenGL.h"
#include "Drawable.hpp"
#include "Shader.hpp"

namespace rgl {


class Surface {

	static constexpr float vertices[32] {
			// positions          // colors           // texture coords
			1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
			1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
			-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
	};
	
	static constexpr unsigned int indices[6]{
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
	};

	const int nWidth, nHeight;

	GLuint glBuffer;
	Shader *pShader;
	Drawable *pActiveDrawable;

	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

public:

	Surface(int width, int height, std::string shader);
	~Surface();

	bool init_opengl();
	void init_texture();
	void reset_texture();
	void tick();
	void deinit();
	
	Drawable *buffer();

};


}
#endif /* Surface_hpp */

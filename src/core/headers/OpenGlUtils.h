//
//  OpenGlUtils.h
//  LoneKart
//
//  Created by rodo on 20/01/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef OpenGlUtils_h
#define OpenGlUtils_h
#define NO_TEXTURE -1

#include "OpenGL.h"

#include <cstdint>
#include <string>
#include <iostream>
#include <streambuf>

namespace rgl {
	class Drawable;
}

class OpenGlUtils {

public:

	static std::string TAG;
	static std::string VERSION;

	static std::string loadShaderFile(const std::string &sFile);

	static unsigned int compileShader(unsigned int type, const std::string &source);

	static unsigned int
	loadShader(const std::string &vertexShader, const std::string &fragementShader);

	static unsigned int loadShader(const std::string &filename);

	static void glError(const std::string &tag);

	static GLuint getGlTexture(GLuint glChannel);

	static GLuint loadTexture(rgl::Drawable *img, GLuint texId = NO_TEXTURE, bool repeat = false);
};

#endif /* OpenGlUtils_h */

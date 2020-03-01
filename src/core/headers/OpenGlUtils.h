//
//  OpenGlUtils.h
//  LoneKart
//
//  Created by rodo on 20/01/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#include "OpenGL.h"

#include <cstdint>
#include <string>
#include <iostream>
#include <streambuf>

namespace Pix {

	constexpr unsigned NO_TEXTURE = (unsigned)-1;

	class Drawable;

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

		static GLuint loadTexture(Pix::Drawable *img, GLuint texId = NO_TEXTURE, bool repeat = false);
	};

}

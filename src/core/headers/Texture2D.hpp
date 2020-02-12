//
//  Texture2D.hpp
//  PixEngine
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef Texture2D_h
#define Texture2D_h

#include "OpenGL.h"
#include "OpenGlUtils.h"
#include "Drawable.hpp"

namespace rgl {

class Texture2D {
	
	Drawable *pBuffer;
	GLuint glChannel = -1;
	GLuint glUnit = -1;
	
public:
	Texture2D(std::string filename, GLuint texUnit);
	bool onUserCreate();
	int width();
	int height();
	GLuint id();
	GLuint unit();
	void bind();

};

inline GLuint Texture2D::id() { return glChannel; }
inline GLuint Texture2D::unit() { return glChannel - 1; }

inline void Texture2D::bind() {
	glActiveTexture(openglutils::getGlTexture(glChannel));
    glBindTexture(GL_TEXTURE_2D, glChannel);
}

inline int Texture2D::width() {
	return pBuffer->width;
}
inline int Texture2D::height() {
	return pBuffer->height;
}
inline bool Texture2D::onUserCreate() {

	if (pBuffer != nullptr) {
		glChannel = openglutils::loadTexture(pBuffer, NO_TEXTURE);
		return glChannel >= 0;
	}
	return false;

}

inline Texture2D::Texture2D(std::string filename, GLuint texUnit):glUnit(texUnit) {
	pBuffer=Drawable::fromFile(filename);
	if (pBuffer == nullptr) throw 404;
}

}
#endif /* Texture2D_h */

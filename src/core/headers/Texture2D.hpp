//
//  Texture2D.hpp
//  PixFu
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef Texture2D_h
#define Texture2D_h

#include "OpenGL.h"
#include "Drawable.hpp"

namespace rgl {

	class Texture2D {

		Drawable *pBuffer;
		GLuint glChannel = -1;

	public:

		~Texture2D();

		// New Texture (buffer)
		Texture2D(Drawable *buffer);

		// New Texture (blank)
		Texture2D(int width, int height);

		// New Texture (from file)
		Texture2D(std::string filename);

		bool upload();    // Upload texture to graphics card
		GLuint id();    // Get texture id
		GLuint unit();    // Get hardwired texture unit
		int width();    // Texture width
		int height();    // Texture Height


		void bind();    // Binds and activates texture
		void update();    // re-uploads changed buffer

		Drawable *buffer();
	};

	inline Drawable *Texture2D::buffer() { return pBuffer; }

	inline GLuint Texture2D::id() { return glChannel; }

	inline GLuint Texture2D::unit() { return glChannel - 1; }

	inline int Texture2D::width() { return pBuffer->width; }

	inline int Texture2D::height() { return pBuffer->height; }

}
#endif /* Texture2D_h */

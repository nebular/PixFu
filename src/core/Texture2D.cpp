//
//  Texture2D.cpp
//  PixFu
//
//  Created by rodo on 12/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "Texture2D.hpp"
#include "OpenGlUtils.h"

namespace Pix {

	Texture2D::Texture2D(Drawable *buffer, bool repeat)
	: pBuffer(buffer), REPEATMODE(repeat) {}

	Texture2D::Texture2D(int width, int height, bool repeat)
	: REPEATMODE(repeat) {
		pBuffer = new Drawable(width, height);
	}

	Texture2D::Texture2D(std::string filename, bool repeat)
	: REPEATMODE(repeat) {
		std::string copy = filename;
		pBuffer = Drawable::fromFile(std::move(filename));
		if (pBuffer == nullptr) throw std::runtime_error("File not found " + copy);
	}

	Texture2D::~Texture2D() {
		delete pBuffer;
		pBuffer = nullptr;
	}

	bool Texture2D::upload() {
		if (pBuffer != nullptr) {
			glChannel = OpenGlUtils::loadTexture(pBuffer, NO_TEXTURE, REPEATMODE);
			return glChannel >= 0;
		}
		return false;
	}

	void Texture2D::bind() {
		glActiveTexture(OpenGlUtils::getGlTexture(glChannel));
		glBindTexture(GL_TEXTURE_2D, glChannel);
	}

	void Texture2D::update() {
		bind();
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pBuffer->width, pBuffer->height, GL_RGBA,
						GL_UNSIGNED_BYTE, pBuffer->getData());
	}

}

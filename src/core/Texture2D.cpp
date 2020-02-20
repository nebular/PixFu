//
//  Texture2D.cpp
//  PixFu
//
//  Created by rodo on 12/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "Texture2D.hpp"
#include "OpenGlUtils.h"

namespace rgl {

	Texture2D::Texture2D(Drawable *buffer) : pBuffer(buffer) {}


	Texture2D::Texture2D(int width, int height) {
		pBuffer = new Drawable(width, height);
	}

	Texture2D::Texture2D(std::string filename) {
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
			glChannel = OpenGlUtils::loadTexture(pBuffer, NO_TEXTURE);
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

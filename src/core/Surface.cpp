//
//  Surface.cpp
//  PixEngine
//
//  Created by rodo on 17/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "Surface.hpp"
#include "Utils.hpp"
#include "Font.hpp"
#include "Canvas2D.hpp"

namespace rgl {

	std::string Surface::TAG = "Surface";

	constexpr float Surface::VERTICES[32];
	constexpr unsigned int Surface::INDICES[6];

	Surface::Surface(int width, int height, const std::string &shaderName,
					 const std::string &samplerName)
			: nWidth(width),
			  nHeight(height),
			  pShader(new Shader(shaderName)),
			  sSamplerName(samplerName) {

		if (DBG) LogV(TAG, SF("Creating, %dx%d, shader %s", width, height, shaderName.c_str()));
		pActiveTexture = new Texture2D(width, height);

	}

	Surface::~Surface() {
		delete pActiveTexture;
		delete pShader;
		pShader = nullptr;
		pActiveTexture = nullptr;
	}

	bool Surface::init(PixFu *engine) {

		if (pActiveTexture == nullptr) return false;
		
		Layer::setup((float *) VERTICES, sizeof(VERTICES), (unsigned int *) INDICES,
					 sizeof(INDICES));

		pActiveTexture->upload();
		
		pShader->use();
		pShader->textureUnit(sSamplerName, pActiveTexture);
		pShader->setVec2("iResolution", nWidth, nHeight);

		pCanvas = new Canvas2D(pActiveTexture->buffer(), new Font());
		return true;

	}

	void Surface::tick(PixFu *engine, float fElapsedTime) {
		pActiveTexture->update();
		pShader->use();

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		draw();
		glDisable(GL_BLEND);
	}

}


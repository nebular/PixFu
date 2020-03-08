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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
#pragma clang diagnostic ignored "-Wunused-parameter"

namespace Pix {

	std::string Surface::TAG = "Surface";

	std::vector<Vertex_t> Surface::VERTICES={
			// positions       // colors/norms     // texture coords
		{{ 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, // top right
		{{ 1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}, // bottom right
		{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // bottom left
		{{-1.0f,  1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}  // top left
	};

	std::vector<unsigned> Surface::INDICES={
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
	};

	Surface::Surface(int width, int height, const FontInfo_t fontInfo, const std::string shaderName, const std::string samplerName, bool blend)
			: nWidth(width),
			  nHeight(height),
			  bBlend(blend),
			  pShader(new Shader(std::move(shaderName))),
			  sSamplerName(std::move(samplerName)) {

		if (DBG) LogV(TAG, SF("Creating, %dx%d, shader %s", width, height, shaderName.c_str()));
		pActiveTexture = new Texture2D(width, height);
		pCanvas = new Canvas2D(pActiveTexture->buffer(), new Font(fontInfo));
	}

	Surface::~Surface() {
		delete pActiveTexture;
		delete pShader;
		pShader = nullptr;
		pActiveTexture = nullptr;
	}

	bool Surface::init(Fu *engine) {

		if (pActiveTexture == nullptr) return false;

		LayerVao::add(VERTICES, INDICES);

		pActiveTexture->upload();

		pShader->use();
		pShader->textureUnit(sSamplerName, pActiveTexture);
		pShader->setVec2("iResolution", nWidth, nHeight);
		pShader->stop();

		return true;

	}

	void Surface::tick(Fu *engine, float fElapsedTime) {

		// only update texture if it has changed
		if (buffer()->clearDirty()) pActiveTexture->update();

		pShader->use();

		if (bBlend) {
			// blend the surface with back layers
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		}

		draw();

		if (bBlend) {
			glDisable(GL_BLEND);
		}

		pShader->stop();

	}

}


#pragma clang diagnostic pop

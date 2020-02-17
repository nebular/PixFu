//
//  PrimarySurface.cpp
//  PixEngine
//
//  Created by rodo on 17/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "PrimarySurface.hpp"
#include "Utils.hpp"

namespace rgl {

std::string PrimarySurface::TAG = "PrimarySurface";
constexpr float PrimarySurface::VERTICES[32];
constexpr unsigned int PrimarySurface::INDICES[6];

PrimarySurface::PrimarySurface(int width, int height, const std::string &samplerName, const std::string &shaderName)
: nWidth(width), nHeight(height), sSamplerName(samplerName), pShader(new Shader(shaderName)) {
	if (DBG) LogV(TAG, SF("Creating, %dx%d, shader %s", width, height, shaderName.c_str()));
	pActiveTexture = new Texture2D(width, height);
}

PrimarySurface::~PrimarySurface() {
	delete pActiveTexture;
	delete pShader;
	pShader = nullptr;
	pActiveTexture=nullptr;
}

bool PrimarySurface::init(PixFu *engine) {
	
	if (pActiveTexture == nullptr) return false;
	Layer::init((float*)VERTICES, sizeof(VERTICES), (unsigned int *)INDICES, sizeof(INDICES));

	pActiveTexture->upload();
	pShader->use();
	pShader->textureUnit(sSamplerName, pActiveTexture);
	pShader->setVec2("iResolution", nWidth, nHeight);
	return true;
	
}

void PrimarySurface::tick(PixFu *engine, float fElapsedTime) {
	pActiveTexture->update();
	pShader->use();
	draw();
}

Drawable *PrimarySurface::buffer() { return pActiveTexture->buffer(); }

}


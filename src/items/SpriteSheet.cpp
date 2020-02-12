//
//  SpriteRenderer.cpp
//  LoneKart
//
//  Created by rodo on 10/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "SpriteSheet.hpp"
#include "OpenGL.h"
#include "glm.hpp"
#include "ext.hpp"
#include "Texture2D.hpp"
#include "PixEngine.hpp"
#include "Utils.hpp"

namespace rgl {

SpriteSheet::SpriteSheet(PixEngine *engine, std::string spriteAtlas, int numx, int numy, std::string shader)
:NUMX(numx),NUMY(numy) {
	pShader = new Shader(shader);
	pTexture = new Texture2D(spriteAtlas);
	SPRSIZE = {pTexture->width()/NUMX, pTexture->height()/NUMY};
	mProjection = glm::ortho(0.0f, (float)engine->WIDTH, (float)engine->HEIGHT, 0.0f, -1.0f, 1.0f);
}

SpriteSheet::~SpriteSheet() {
	glDeleteVertexArrays(1, &this->quadVAO);
	delete pShader;
	delete pTexture;
	pShader = nullptr;
	pTexture = nullptr;
}

void SpriteSheet::init() {
	// Configure VAO/VBO
	GLuint VBO;
	GLfloat vertices[] = {
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindVertexArray(quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

glm::vec4 SpriteSheet::getTinter(TintMode_t tintMode, Pixel replacement) {
	return { (float)replacement.r/255, (float)replacement.g/255, (float)replacement.b/255, tintMode};
}

bool SpriteSheet::remove(int spriteId) {
	return mSprites.erase(spriteId) > 0;
}

int SpriteSheet::create(
						int sheetIndex,
						glm::vec2 position,
						float scale,
						float rotation,
						float height,
						int totalx,
						int totaly) {
	
	glm::vec4 spritePos = { position.x, position.y, scale, rotation };
	glm::vec4 spriteDef = { sheetIndex, height, totalx, totaly };
	glm::vec4 spriteFx = getTinter(NO_TINT, Pixel(0,0,0,0));
	SpriteMeta_t meta = { spritePos, spriteDef, spriteFx };
	
	mSprites[nIdCounter]=meta;
	
	return nIdCounter++;
}

void SpriteSheet::tint(int spriteId, TintMode_t tintMode, Pixel color) {
	SpriteMeta_t &meta = mSprites.at(spriteId);
	meta.fx = getTinter(tintMode, color);
}

void SpriteSheet::update(
						 int spriteId,
						 glm::vec2 position,
						 int spriteIndex,
						 float scale,
						 float rotation,
						 float height) {
	
	SpriteMeta_t &meta = mSprites.at(spriteId);
	
	if (spriteIndex>=0) meta.def.x = spriteIndex;
	meta.pos.x = position.x;
	meta.pos.y = position.y;
	meta.pos.z = scale;
	meta.pos.w = rotation;
	meta.def.y = height;
}

void SpriteSheet::drawSprite(SpriteMeta_t &meta) {
	
	float scale = meta.pos.z;
	float rotation = meta.pos.w;
	
	glm::mat4 model = glm::identity<glm::mat4>();
	
	glm::vec2 size = SPRSIZE * scale;
	
	model = glm::translate(model, glm::vec3(meta.pos.x, meta.pos.y, 0.0f));
	
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	
	model = glm::scale(model, glm::vec3(size, 1.0f));
	
	pShader->setMat4("model", &model[0][0]);
	pShader->setVec4("iSpriteDef", meta.def.x, meta.def.y, meta.def.z, meta.def.w);
	pShader->setVec4("iSpriteFx", meta.fx.x, meta.fx.y, meta.fx.z, meta.fx.w);
	
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	
}

bool SpriteSheet::onUserCreate(PixEngine *engine) {
	
	if (pTexture == nullptr) return false;
	
	init();
	lStartTime = nowms();
	
	bool success = pTexture->upload();
	
	if (success) {
		// the spritesheet (all sprites in one image)
		// aqui no va pero deberia ir
		//		pShader->setInt("sampler", 1); // RLP
		//		pShader->setVec4("iSpriteSheet", pTexture->width(), pTexture->height(), NUMX, NUMY);
		//		pShader->setMat4("projection", &mProjection[0][0]);
		return true;
	} else {
		std::cerr << "Spritesheet not found" << std::endl;
		return false;
	}
	
}

void SpriteSheet::onUserUpdate(PixEngine *engine, float fElapsedTime) {
	
	long ms = nowms();
	
	pShader->use();
	
	// all these things are common, ideally should be sent in onUserCreate
	// but I can't get it working !
	
	// texture sampler
	pShader->setInt("sampler", pTexture->unit());
	// projecyion matrix
	pShader->setMat4("projection", &mProjection[0][0]);
	// spritesheet metrics
	pShader->setVec4("iSpriteSheet", pTexture->width(), pTexture->height(), NUMX, NUMY);
	// chroma key
	pShader->setVec4("iColorKey", (float)oChromaKey.r/255, (float)oChromaKey.g/255, (float)oChromaKey.b/255, (float)oChromaKey.a/255);
	
	// send time
	pShader->setFloat("iTime", ((float)( ms - lStartTime))/1000 );
	
	pTexture->bind();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	
	std::map<int,SpriteMeta_t>::iterator it;
	for ( it=mSprites.begin(); it!=mSprites.end(); ++it)
		drawSprite(it->second);
	
	glDisable(GL_BLEND);
}

}

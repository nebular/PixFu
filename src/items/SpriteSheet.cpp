#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#pragma ide diagnostic ignored "readability-magic-numbers"
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

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
#include "PixFu.hpp"
#include "Utils.hpp"
#include "SpriteSheets.hpp"


namespace rgl {

	std::string SpriteSheet::TAG = "SpriteSheet";

	SpriteSheet::SpriteSheet(PixFu *engine, SpriteSheetInfo_t info) : sInfo(std::move(info)) {
		pShader = new Shader(sInfo.shader);
		pTexture = new Texture2D(sInfo.filename);
		SPRSIZE = {pTexture->width() / sInfo.numX, pTexture->height() / sInfo.numY};
		sInfo.spriteWidth = SPRSIZE.x;
		sInfo.spriteHeight = SPRSIZE.y;
		mProjection = glm::ortho(0.0F, (float) engine->screenWidth(),
								 (float) engine->screenHeight(), 0.0F, -1.0F, 1.0F);
		nId = SpriteSheets::add(this);
		if (DBG) LogV(TAG, SF("Created spritesheet %d", nId));
	}

	SpriteSheet::~SpriteSheet() {
		if (DBG) LogV(TAG, SF("Destroying spritesheet %d", nId));
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
				0.0F, 1.0F, 0.0F, 1.0F,
				1.0F, 0.0F, 1.0F, 0.0F,
				0.0F, 0.0F, 0.0F, 0.0F,

				0.0F, 1.0F, 0.0F, 1.0F,
				1.0F, 1.0F, 1.0F, 1.0F,
				1.0F, 0.0F, 1.0F, 0.0F
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(quadVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *) nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	glm::vec4 SpriteSheet::getTinter(TintMode_t tintMode, Pixel replacement) {
		return {(float) replacement.r / 255, (float) replacement.g / 255,
				(float) replacement.b / 255, tintMode};
	}

	bool SpriteSheet::remove(int spriteId) {
		return mSprites.erase(spriteId) > 0;
	}

	int SpriteSheet::create(
			int sheetIndex,
			int totalx,
			int totaly,

			glm::vec2 position,        // position in screen coords
			float scale,            // scale
			float rotation,            // rotation
			float height            // height
	) {

		glm::vec4 spritePos = {position.x, position.y, scale, rotation};
		glm::vec4 spriteDef = {sheetIndex, height, totalx, totaly};
		glm::vec4 spriteFx = getTinter(NO_TINT, Pixel(0, 0, 0, 0));
		SpriteMeta_t meta = {spritePos, spriteDef, spriteFx};

		mSprites[nIdCounter] = meta;

		return nIdCounter++;
	}

	void SpriteSheet::tint(int spriteId, TintMode_t tintMode, Pixel color) {
		SpriteMeta_t &meta = mSprites.at(spriteId);
		meta.fx = getTinter(tintMode, color);
	}

	void SpriteSheet::tint(int spriteId, TintMode_t tintMode) {
		SpriteMeta_t &meta = mSprites.at(spriteId);
		meta.fx.w = tintMode;
	}

	void SpriteSheet::hide(int spriteId) {
		SpriteMeta_t &meta = mSprites.at(spriteId);
		meta.def.x = -1;
	}

	void SpriteSheet::update(
			int spriteId,
			glm::vec2 position,
			int spriteIndex,
			float scale,
			float rotation,
			glm::vec4 raw) {

		SpriteMeta_t &meta = mSprites.at(spriteId);

		if (spriteIndex >= 0) meta.def.x = spriteIndex;
		meta.pos.x = position.x;
		meta.pos.y = position.y;
		meta.pos.z = scale;
		meta.pos.w = rotation;
		meta.def.y = raw.z;
		meta.raw.x = raw.x;
		meta.raw.y = raw.y;
		meta.raw.z = raw.z;
	}

	void SpriteSheet::drawSprite(SpriteMeta_t &meta) {

		// sprite can be hidden
		if (meta.def.x == -1) return;

		float scale = meta.pos.z;
		float rotation = meta.pos.w;


		glm::vec2 size = glm::vec2(SPRSIZE.x * scale, SPRSIZE.y * scale);

		// account for the number of sprites width & height
		size.x *= meta.def.z;
		size.y *= meta.def.w;

		auto model = glm::identity<glm::mat4>();

		model = glm::translate(model,
							   glm::vec3(meta.pos.x - size.x / 2, meta.pos.y - size.y / 2, 0.0F));

		model = glm::translate(model, {0.5F * size.x, 0.5F * size.y, 0.0F});
#pragma clang diagnostic push
#pragma ide diagnostic ignored "err_ovl_no_viable_member_function_in_call"
		model = glm::rotate(model, rotation, {0.0f, 0.0f, 1.0f});
#pragma clang diagnostic pop
		model = glm::translate(model, {-0.5F * size.x, -0.5F * size.y, 0.0F});

		model = glm::scale(model, {size.x, size.y, 1.0});

		pShader->setMat4("model", (float *) &model);
		pShader->setVec4("iSpriteRaw", meta.raw.x, meta.raw.y, meta.raw.z, meta.pos.w);
		pShader->setVec4("iSpriteDef", meta.def.x, meta.def.y, meta.def.z, meta.def.w);
		pShader->setVec4("iSpriteFx", meta.fx.x, meta.fx.y, meta.fx.z, meta.fx.w);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

	}

	void SpriteSheet::clear() {
		mSprites.clear();
	}

	bool SpriteSheet::init(PixFu *engine) {

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

	void SpriteSheet::tick(PixFu *engine, float fElapsedTime) {

		long ms = nowms();

		pShader->use();

		// all these things are common, ideally should be sent in onUserCreate
		// but I can't get it working !


		// projecyion matrix
		pShader->setMat4("projection", (float *) &mProjection);
		// spritesheet metrics
		pShader->setVec4("iSpriteSheet", pTexture->width(), pTexture->height(), getNumX(),
						 getNumY());
		// chroma key
		pShader->setVec4("iColorKey", (float) oChromaKey.r / 255, (float) oChromaKey.g / 255,
						 (float) oChromaKey.b / 255, (float) oChromaKey.a / 255);

		// send time
		pShader->setFloat("iTime", ((float) (ms - lStartTime)) / 1000);

		// ground sampler
		if (SpriteSheets::pGroundTexture != nullptr) {
			Texture2D *g = SpriteSheets::pGroundTexture;
			pShader->setVec2("iMapSize", g->width(), g->height());
			pShader->setInt("ground", SpriteSheets::pGroundTexture->unit());
			SpriteSheets::pGroundTexture->bind();
		}

		// texture sampler
		pShader->setInt("sampler", pTexture->unit());
		pTexture->bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		std::map<int, SpriteMeta_t>::iterator it;
		for (it = mSprites.begin(); it != mSprites.end(); ++it)
			drawSprite(it->second);

		glDisable(GL_BLEND);
	}

}


#pragma clang diagnostic pop
//
//  SpriteRenderer.hpp
//  LoneKart
//
//  Created by rodo on 10/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#ifndef SpriteRenderer_hpp
#define SpriteRenderer_hpp

#include "Shader.hpp"
#include "glm.hpp"
#include "Drawable.hpp"
#include "Texture2D.hpp"
#include "PixFu.hpp"
#include <vector>
#include <map>

namespace rgl {

	typedef enum eTintMode {
		NO_TINT = 0, TINT_CHROMA = 1, TINT_FULL = 2
	} TintMode_t;

	typedef struct sSpriteMeta {
		glm::vec4 pos, def, fx;
	} SpriteMeta_t;

	class PixFu;

	class SpriteSheet : public PixFuExtension {

		const int NUMX, NUMY;
		int nIdCounter = 0;


	public:

		SpriteSheet(PixFu *engine, std::string spriteAtlas, int numX, int numY,
					std::string shader = "sprites");

		~SpriteSheet();

		bool init(PixFu *engine);

		void tick(PixFu *engine, float fElapsedTime);

		/**
		 * Adds a new sprite into the screen
		 * returns the spriteId
		 */
		int create(
				int sheetIndex,            // sprite index in the sheet
				glm::vec2 position,        // position in screen coordinates
				float scale = 1,            // scale
				float rotation = 0,        // rotation in radians
				float height = 0,            // height
				int totalx = 1,            // number of sprites wide
				int totaly = 1            // number of sprites high
		);

		bool remove(int spriteId);

		/**
		 * Tints existing sprite
		 */
		void tint(
				int spriteId,
				TintMode_t tintMode,
				Pixel color
		);

		/**
		 * Updates existing sprite properties
		 */
		void update(
				int spriteId,            // sprite ID
				glm::vec2 position,        // position in screen coords
				int spriteIndex = -1,    // new sprite index in sheet or -1 to keep
				float scale = 1.0,        // scale
				float rotation = 0.0,    // rotation
				float height = 0.0        // height
		);


	private:
		Pixel oChromaKey = Pixel(255, 0, 0, 30);

		Shader *pShader;
		GLuint quadVAO;
		Texture2D *pTexture;
		glm::vec2 SPRSIZE;
		glm::mat4 mProjection;

		long lStartTime;

		std::map<int, SpriteMeta_t> mSprites;

		void init();

		void drawSprite(SpriteMeta_t &spriteMeta);

		glm::vec4 getTinter(TintMode_t tintMode, Pixel replacement);
	};

};


#endif /* SpriteRenderer_hpp */

#pragma clang diagnostic pop
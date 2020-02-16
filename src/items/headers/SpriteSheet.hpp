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


// a handy way to refer to a sprite: spritesheet + spriteindex
typedef struct SpriteLocator {
	int spriteSheet;
	int spriteIndex;
	int numX = 1;
	int numY = 1;
} SpriteLocator_t;

// todo
typedef struct SpriteSheetInfo {
	std::string filename;				// png filename
	int numX, numY;						// number of sprites x,y
	std::string shader = "default";
	float spriteRadiusConstant = 1.0;	// sphere rdius vs full radius width
	int width, height;					// calculated size of spritesheet png
	int spriteWidth, spriteHeight;		// calculated size of each sprite
} SpriteSheetInfo_t;



typedef enum eTintMode {
	NO_TINT = 0, TINT_CHROMA = 1, TINT_FULL = 2
} TintMode_t;

typedef struct sSpriteMeta {
	glm::vec4 pos, def, fx, raw;
} SpriteMeta_t;

class PixFu;

class SpriteSheet : public PixFuExtension {
	
	static std::string TAG;
	
	int nIdCounter = 0;
	int nId;

	SpriteSheetInfo_t sInfo;
	
	
public:
	
	SpriteSheet(PixFu *engine, SpriteSheetInfo_t info);
	
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
			   int totalx = 1,            // number of sprites wide
			   int totaly = 1,            // number of sprites high
			   
			   glm::vec2 position = {0,0},        // position in screen coords
			   float scale = 1.0,        // scale
			   float rotation = 0.0,    // rotation
			   float height = 0.0        // height
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
	
	void tint(
	int spriteId,
	TintMode_t tintMode
	);
	
	/**
	 * Hides/shows existing sprite
	 */
	void hide(int spriteId);
	
	/**
	 * Updates existing sprite properties
	 */
	void update(
				int spriteId,             // sprite ID
				glm::vec2 position,       // position in screen coords
				int spriteIndex = -1,     // new sprite index in sheet or -1 to keep
				float scale = 1.0,        // scale
				float rotation = 0.0,     // rotation
				glm::vec4 raw = {0,0,0,0}
	);
	
	void clear();
	
	int getId();
	int getNumX();
	int getNumY();
	int getWidth();
	int getHeight();
	int getSpriteWidth();
	int getSpriteHeight();;
	float getSpriteRadius();
	
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
	
	// getters
	glm::vec4 getTinter(TintMode_t tintMode, Pixel replacement);
	
};

// query spritesheet metadata
inline int SpriteSheet::getId() 		 { return nId; }
inline int SpriteSheet::getNumX() 		 { return sInfo.numX; }
inline int SpriteSheet::getNumY() 		 { return sInfo.numY; }
inline int SpriteSheet::getWidth() 		 { return sInfo.width; }
inline int SpriteSheet::getHeight() 	 { return sInfo.height; }
inline int SpriteSheet::getSpriteWidth()  { return SPRSIZE.x; }
inline int SpriteSheet::getSpriteHeight() { return SPRSIZE.y; }
inline float SpriteSheet::getSpriteRadius() { return sInfo.spriteRadiusConstant * SPRSIZE.x / 2; }
};


#endif /* SpriteRenderer_hpp */

#pragma clang diagnostic pop

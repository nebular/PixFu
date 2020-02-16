//
//  SpriteSheets.cpp
//  LoneKart
//
//  Created by rodo on 14/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "SpriteSheets.hpp"
#include "Utils.hpp"

namespace rgl {

int SpriteSheets::instanceCounter = 0;
std::string SpriteSheets::TAG = "SpriteSheets";
Texture2D *SpriteSheets::pGroundTexture = nullptr;

std::map<int,SpriteSheet*> SpriteSheets::mSpriteSheet;

int SpriteSheets::add(SpriteSheet *spriteSheet) {
	if (DBG) LogV(TAG, SF("Adding spritesheet %d", instanceCounter));
	mSpriteSheet[instanceCounter] = spriteSheet;
	return instanceCounter++;
}

void SpriteSheets::clear() {
	if (DBG) LogV(TAG, "Clearing sprites in all sheets");
	std::map<int,SpriteSheet*>::iterator it;
	for ( it=mSpriteSheet.begin(); it!=mSpriteSheet.end(); ++it) {
		it->second->clear();
	}
}

bool SpriteSheets::remove(int spriteSheedId) {
	if (DBG) LogV(TAG, SF("Removing spritesheet %d", spriteSheedId));
	return mSpriteSheet.erase(spriteSheedId) > 0;
}

bool SpriteSheets::remove(SpriteSheet *spriteSheet) {
	std::map<int,SpriteSheet*>::iterator it;
	for ( it=mSpriteSheet.begin(); it!=mSpriteSheet.end(); ++it) {
		if (it->second == spriteSheet) {
			return remove(it->first);
		}
	}
	return false;
}

void SpriteSheets::unload() {
	if (DBG) LogV(TAG, "Unloading all spritesheets");
	std::map<int,SpriteSheet*>::iterator it;
	for ( it=mSpriteSheet.begin(); it!=mSpriteSheet.end(); ++it) {
		delete it->second;
	}
	mSpriteSheet.clear();
	instanceCounter = 0;
}

}

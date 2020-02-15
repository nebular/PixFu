//
//  SpriteSheets.hpp
//  LoneKart
//
//  Created by rodo on 14/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef SpriteSheets_hpp
#define SpriteSheets_hpp

#include "SpriteSheet.hpp"

namespace rgl {

class SpriteSheets {

	static std::string TAG;
	static int instanceCounter;
	static std::map<int,SpriteSheet*> mSpriteSheet;		// Loaded spritesheets

public:
	static int add(SpriteSheet *spriteSheet);
	static bool remove(int spriteSheedId);
	static bool remove(SpriteSheet *spriteSheet);
	static SpriteSheet *get(int spriteSheetId);
	static void clear();
	static void unload();
};

// Gets a loaded Spritesheet. It is through this object that you interact with sprites.
inline SpriteSheet *SpriteSheets::get(int spriteSheetId) { return mSpriteSheet.at(spriteSheetId); }

}
#endif /* SpriteSheets_hpp */

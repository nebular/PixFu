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

	static std::vector<SpriteSheet*> vSpriteSheet;		// Loaded spritesheets

public:
	static void add(SpriteSheet *spriteSheet);
	static SpriteSheet *get(int index);
	static void clear();
};

// Gets a loaded Spritesheet. It is through this object that you interact with sprites.
inline SpriteSheet *SpriteSheets::get(int index) { return vSpriteSheet.at(index); }

}
#endif /* SpriteSheets_hpp */

//
//  SpriteSheets.cpp
//  LoneKart
//
//  Created by rodo on 14/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "SpriteSheets.hpp"

namespace rgl {
std::vector<SpriteSheet*> SpriteSheets::vSpriteSheet;

void SpriteSheets::add(SpriteSheet *spriteSheet) {
	vSpriteSheet.push_back(spriteSheet);
}

void SpriteSheets::clear() {
	for (SpriteSheet *s:vSpriteSheet)
		s->clear();
}
}

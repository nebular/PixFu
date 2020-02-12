//
//  Font.hpp
//  LoneKart
//
//  Created by rodo on 17/01/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef Font_hpp
#define Font_hpp
#include "PixEngine.hpp"

namespace rgl {
class Font {

	Drawable *pTarget;
	Drawable *pFontSprite;
	int nWidth, nHeight;

public:
	Font(std::string filename, int charWidth=8, int charHeight=8);
	void drawString(Drawable *target, int32_t x, int32_t y, std::string sText, rgl::Pixel col, uint32_t scale);
};
}
#endif /* Font_hpp */

//
//  Font.hpp
//  LoneKart
//
//  Created by rodo on 17/01/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef Font_hpp
#define Font_hpp
#include "Drawable.hpp"

namespace rgl {
	class Font {

		Drawable *pFontSprite;
		int nWidth, nHeight;

	public:
		Font(std::string filename = "fonts/default.png", int charWidth = 8, int charHeight = 8);

		~Font();

		void
		drawString(Drawable *target, int32_t x, int32_t y, const std::string &text, Pixel col,
				   uint32_t scale = 1);
	};
}
#endif /* Font_hpp */

//
//  Font.hpp
//  A helper class to draw strings using a Font. Fonts are supplied as PNG files with a
//  fixed grid.
//
//  Code to draw string originally from onelonecoder.com ´s Pixel Game Engine
//
//  Created by rodo on 17/01/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once
#include "Drawable.hpp"
#include <string_view>

namespace Pix {

	typedef struct sFontInfo {
		const std::string_view name = "default";
		const int charWidth = 8;
		const int charHeight = 8;
		const int firstChar = 32;
	} FontInfo_t;

	static FontInfo_t FONT_DEFAULT = {};
	static FontInfo_t FONT_BIASAM = { "Bisasam16x16.png",16,16};
	static FontInfo_t FONT_BIASAM_STROKE = { "Bisasam16x16_stroke.png", 16,16};

	class Font {

		Drawable *pFontSprite;
		const FontInfo_t INFO;
		
	public:

		/**
		 * Creates a new font, loading its bitmap.
		 * )@param fontInfo The font descriptor
		 */

		Font(FontInfo_t fontInfo={});
		
		~Font();

		void
		drawString(Drawable *target, int32_t x, int32_t y, const std::string &text, Pixel col,
				   uint32_t scale = 1);
	};
}

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

	/** Defines a font */
	typedef struct sFontInfo {
		/** Font name, maps to assets /fonts/<name>.png */
		const std::string_view name = "default";
		/** Character Width */
		const int charWidth = 8;
		/** Character Height */
		const int charHeight = 8;
		/** First ascii char */
		const int firstChar = 32;
		/** Total chars (only needed if multicolot multiversion fonts) */
		const int totalChars = 96;
		/** Whether the font is multicolor or monochrome (that will be colored) */
		const bool multiColor = false;
		/** Number of sequential versions of the font with different colors or styles */
		const int colorVersions = 1;

	} FontInfo_t;

	// Font constants

	static FontInfo_t FONT_DEFAULT = {};
	static FontInfo_t FONT_BISASAM = { "Bisasam_16x16",16,16};
	static FontInfo_t FONT_BISASAM_STROKE = { "Bisasam_16x16_stroke", 16,16};
	static FontInfo_t FONT_1943 = { "1943", 8,8, 32, 95, true, 10 };
	static FontInfo_t FONT_DANGER = { "Dangerous Seed (Namco)", 8,8, 32, 95, true, 11 };
	static FontInfo_t FONT_RAIDEN = { "Raiden Fighters (Seibu)", 16,8, 32, 95, true, 10 };

	class Font {

		Drawable *pFontSprite;
		
	public:

		const FontInfo_t INFO;

		/**
		 * Creates a new font, loading its bitmap.
		 * @param fontInfo The font descriptor
		 */

		Font(FontInfo_t fontInfo={});
		
		~Font();

		void
		drawString(Drawable *target, int32_t x, int32_t y, const std::string &text, Pixel col,
				   uint32_t scale = 1);
	};
}

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

namespace Pix {

	class Font {

		Drawable *pFontSprite;
		int nWidth, nHeight;

	public:

		/**
		 Creates a new font, loading its bitmap.
		 @param fontName The font name, the font bitmap is expected at /fonts/<name>.png in the
						 application assets
		 @param charWidth Character Width in px
		 @param charHeight Character Height in px
		 */

		Font(std::string fontName = "default", int charWidth = 8, int charHeight = 8);

		~Font();

		void
		drawString(Drawable *target, int32_t x, int32_t y, const std::string &text, Pixel col,
				   uint32_t scale = 1);
	};
}

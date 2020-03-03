//
//  Font.cpp
//  LoneKart
//
//  Font routines Copyright 2018 - 2019 OneLoneCoder.com
//
//  Created by rodo on 17/01/2020.
//  Copyright © 2020 rodo. All rights reserved.

#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
#pragma ide diagnostic ignored "readability-magic-numbers"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"


#include "Font.hpp"


namespace Pix {

	Font::Font(FontInfo_t fontInfo):INFO(std::move(fontInfo)) {
		std::string fontName = "fonts/" + std::string(INFO.name) + ".png";
		pFontSprite = Drawable::fromFile(fontName);
		if (pFontSprite == nullptr) throw new std::runtime_error("Font " + fontName + " not found.");
	}

	Font::~Font() {
		delete pFontSprite;
		pFontSprite = nullptr;
	}

	// code from onelonecoder.com
	void Font::drawString(Drawable *target, int32_t x, int32_t y, const std::string &sText,
						  Pix::Pixel col,
						  uint32_t scale) {
		int sx = 0;
		int sy = 0;

		for (auto c : sText) {
			if (c == '\n') {
				sx = 0;
				sy += INFO.charHeight * scale;
			} else {
				const int glyphsPerWidth = pFontSprite->width/INFO.charWidth;

				int32_t ox = (c - INFO.firstChar) % glyphsPerWidth;
				int32_t oy = (c - INFO.firstChar) / glyphsPerWidth;

				if (scale > 1) {
					for (uint32_t i = 0; i < INFO.charWidth; i++)
						for (uint32_t j = 0; j < INFO.charHeight; j++) {
							Pix::Pixel pix = pFontSprite->getPixel(i + ox * INFO.charWidth,
																   j + oy * INFO.charHeight);
							if (pix.a != 0) {
								for (uint32_t is = 0; is < scale; is++)
									for (uint32_t js = 0; js < scale; js++)
										target->setPixel(x + sx + (i * scale) + is,
														 y + sy + (j * scale) + js,
														 pix.r + pix.b + pix.g == 0 ? pix : col);
							}
						}
				} else {
					for (uint32_t i = 0; i < INFO.charWidth; i++)
						for (uint32_t j = 0; j < INFO.charWidth; j++) {
							Pixel pix = pFontSprite->getPixel(i + ox * INFO.charWidth, j + oy * INFO.charHeight);
							if (pix.a != 0) {
								target->setPixel(x + sx + i, y + sy + j,
												 pix.r + pix.b + pix.g == 0 ? pix : col);
							}
						}
				}
				sx += INFO.charWidth * scale;
			}
		}
	}
}

#pragma clang diagnostic pop

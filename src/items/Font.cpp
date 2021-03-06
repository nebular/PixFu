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

	// code from onelonecoder.com, tweaked for multicolor and color maps
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

				bool isZero = col.r==0 && col.g==0 && col.b==0;
				int code = c;

				if (INFO.multiColor && isZero && col.a < INFO.colorVersions)
					code+=(col.a-1)*INFO.totalChars;
				
				int32_t ox = (code - INFO.firstChar) % glyphsPerWidth;
				int32_t oy = (code - INFO.firstChar) / glyphsPerWidth;
					
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
														 INFO.multiColor ? pix : ( isZero ? col : pix));
							}
						}
				} else {
					for (uint32_t i = 0; i < INFO.charWidth; i++)
						for (uint32_t j = 0; j < INFO.charWidth; j++) {
							Pixel pix = pFontSprite->getPixel(i + ox * INFO.charWidth, j + oy * INFO.charHeight);
							if (pix.a != 0) {
								target->setPixel(x + sx + i, y + sy + j,
												 INFO.multiColor ? pix : (isZero ? col : pix));
							}
						}
				}
				sx += INFO.charWidth * scale;
			}
		}
	}
}

#pragma clang diagnostic pop

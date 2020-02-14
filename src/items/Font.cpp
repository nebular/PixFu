#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
#pragma ide diagnostic ignored "readability-magic-numbers"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

//
//  Font.cpp
//  LoneKart
//
//  Created by rodo on 17/01/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "Font.hpp"


namespace rgl {

	Font::Font(std::string filename, int w, int h) {
		pFontSprite = Drawable::fromFile(std::move(filename));
		nWidth = w;
		nHeight = h;
	}

	Font::~Font() {
		delete pFontSprite;
		pFontSprite = nullptr;
	}


	void Font::drawString(Drawable *target, int32_t x, int32_t y, const std::string &sText,
						  rgl::Pixel col,
						  uint32_t scale) {
		int sx = 0;
		int sy = 0;

		for (auto c : sText) {
			if (c == '\n') {
				sx = 0;
				sy += nHeight * scale;
			} else {
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;

				if (scale > 1) {
					for (uint32_t i = 0; i < nWidth; i++)
						for (uint32_t j = 0; j < nHeight; j++) {
							rgl::Pixel pix = pFontSprite->getPixel(i + ox * nWidth,
																   j + oy * nHeight);
							if (pix.a != 0) {
								for (uint32_t is = 0; is < scale; is++)
									for (uint32_t js = 0; js < scale; js++)
										target->setPixel(x + sx + (i * scale) + is,
														 y + sy + (j * scale) + js,
														 pix.r + pix.b + pix.g == 0 ? pix : col);
							}
						}
				} else {
					for (uint32_t i = 0; i < nWidth; i++)
						for (uint32_t j = 0; j < nWidth; j++) {
							Pixel pix = pFontSprite->getPixel(i + ox * nWidth, j + oy * nHeight);
							if (pix.a != 0) {
								target->setPixel(x + sx + i, y + sy + j,
												 pix.r + pix.b + pix.g == 0 ? pix : col);
							}
						}
				}
				sx += nWidth * scale;
			}
		}
	}
}

#pragma clang diagnostic pop
#pragma clang diagnostic pop
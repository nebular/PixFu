//
//  Drawable.hpp
//  PixFu
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#include <string>

namespace rgl {

	struct Pixel {

		union {
			const uint32_t n = 0xFF000000;
			struct {
				const uint8_t r;
				const uint8_t g;
				const uint8_t b;
				const uint8_t a;
			};
		};

		constexpr Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);

		constexpr Pixel(uint32_t p = 0);

		constexpr Pixel scale(float mult) const;
	};

	inline constexpr Pixel::Pixel(uint32_t p) : n(p) {}

	inline constexpr Pixel::Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : r(red), g(green), b(blue), a(alpha) {}

	inline constexpr Pixel Pixel::scale(float mult) const {
		return {
				static_cast<uint8_t>(r * mult),
				static_cast<uint8_t>(g * mult),
				static_cast<uint8_t>(b * mult),
				a
		};
	}


	struct Colors {

		static constexpr Pixel

				BLANK = {0, 0, 0, 0},
				BLACK = {0, 0, 0},
				RED = {255, 0, 0},
				ORANGE = {255, 128, 0},
				GREEN = {0, 255, 0},
				BLUE = {0, 0, 255},
				CYAN = {0, 255, 25},
				LIGHTBLUE = {0, 128, 25},
				LIGHTGREEN = {0, 255, 12},
				MAGENTA = {255, 0, 25},
				VIOLET = {128, 0, 25},
				PINK = {255, 128, 255},
				YELLOW = {255, 255, 0},
				GREY = {128, 128, 128},
				WHITE = {255, 255, 255};

	};


	class Drawable {

		static std::string TAG;
		Pixel *pData;

	public:

		const int width, height;

		Drawable(int w, int h);

		~Drawable();

		Pixel *getData();

		void setPixel(int x, int y, Pixel pix);

		Pixel getPixel(int x, int y);

		Pixel sample(float x, float y);

		void clear(Pixel color);

		void blank(char ch);

		static Drawable *fromFile(std::string name);

	};

	inline Pixel *Drawable::getData() { return pData; }

	inline void Drawable::setPixel(int x, int y, rgl::Pixel pix) {
		if (x < width && y < height && x >= 0 && y >= 0) pData[y * width + x] = pix;
	}

	inline Pixel Drawable::getPixel(int x, int y) {
		return pData[y * width + x];
	}

};

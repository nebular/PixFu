//
//  Drawable.hpp
//  PixFu
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef Drawable_hpp
#define Drawable_hpp

#include <string>

namespace rgl {

	struct Pixel {

		union {
			uint32_t n = 0xFF000000;
			struct {
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a;
			};
		};

		Pixel();

		Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);

		Pixel(uint32_t p);

		Pixel scale(float mult);
	};

	class Colors {
	public:
		static Pixel RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, BLACK, GREY, WHITE,
		ORANGE, PINK, VIOLET, LIGHTBLUE, LIGHTGREEN;
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
#endif /* Drawable_hpp */

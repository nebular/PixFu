//
//  Drawable.hpp
//  PixFu
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "err_uninitialized_member_in_ctor"

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
		bool dirty;

	public:

		const int width, height;

		/**
		 * Creates an empty drawable
		 * @param w Width in PX
		 * @param h height in PX
		 */
		Drawable(int w, int h);

		~Drawable();

		/**
		 * Gets supporting buffer
		 * @return The raw pixel buffer
		 */

		Pixel *getData();

		/**
		 * Sets a pixel
		 * @param x coord
		 * @param y coord
		 * @param pix color
		 */
		void setPixel(int x, int y, Pixel pix);

		/**
		 * Gets pixel at position
		 * @param x coord
		 * @param y coord
		 * @return Pixel
		 */

		Pixel getPixel(int x, int y);

		/**
		 * Samples with normalized coordinates
		 * @param x  normalized x
		 * @param y  normalized y
		 * @return The pixel
		 */

		Pixel sample(float x, float y);

		/**
		 * Clears the buffer to the provided pixel color
		 * @param color The color
		 */

		void clear(Pixel color);

		/**
		 * Clears the buffer using a fast memset. So all components must be the same. Useful mainly to
		 * efficientlt clear to transparent (0x0000000)
		 * @param ch
		 */

		void blank(char ch);

		/**
		 * Clears the dirty flag if set. Flag is set by setPixel(). Used by other classes to know
		 * if a drawable has changed.
		 * @return true if flag was cleared
		 */

		bool clearDirty();


		/**
		 * Creates a drawable from a PNG file
		 * @param name The filename
		 * @return a Drawable
		 */

		static Drawable *fromFile(std::string name);

	};

	inline Pixel *Drawable::getData() { return pData; }

	inline void Drawable::setPixel(int x, int y, rgl::Pixel pix) {
		dirty = true;
		if (x < width && y < height && x >= 0 && y >= 0)
			pData[y * width + x] = pix;
	}

	inline bool Drawable::clearDirty() {
		if (dirty) {
			dirty = false;
			return true;
		} else return false;
	}

	inline Pixel Drawable::getPixel(int x, int y) {
		if (x < 0 || y < 0 || x >= width || y >= height) return 0;
		return pData[y * width + x];
	}

};
#pragma clang diagnostic pop

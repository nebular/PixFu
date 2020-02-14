//
//  Drawable.cpp
//  PixFu
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "png.h"

#include "Drawable.hpp"
#include "PixFu.hpp"

using namespace rgl;

Pixel::Pixel() {
	r = 0;
	g = 0;
	b = 0;
	a = 255;
}

Pixel::Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

Pixel::Pixel(uint32_t p) {
	n = p;
}

Pixel Pixel::scale(float mult) {
	return Pixel(r * mult, g * mult, b * mult, a);
}

Pixel Colors::RED = Pixel(255, 0, 0);
Pixel Colors::GREEN = Pixel(0, 255, 0);
Pixel Colors::BLUE = Pixel(0, 0, 255);
Pixel Colors::CYAN = Pixel(0, 255, 255);
Pixel Colors::MAGENTA = Pixel(255, 0, 255);
Pixel Colors::YELLOW = Pixel(255, 255, 0);
Pixel Colors::GREY = Pixel(128, 128, 128);
Pixel Colors::WHITE = Pixel(255, 255, 255);
Pixel Colors::BLACK = Pixel(0, 0, 0);

std::string Drawable::TAG="Drawable";

Drawable::Drawable(int x, int y) : width(x), height(y) {
	pData = new rgl::Pixel[x * y];
	if (DBG) LogV(TAG, SF("New Drawable %dx%d (%dkb)", width, height, width*height/1000));
}

Drawable::~Drawable() {
	delete pData;
	pData = nullptr;
	if (DBG) LogV(TAG, SF("Free Drawable %dx%d (%dkb)", width, height, width*height/1000));
}

void Drawable::setPixel(int x, int y, rgl::Pixel pix) {
	if (x < width && y < height && x >= 0 && y >= 0)
		pData[y * width + x] = pix;
}

Pixel Drawable::getPixel(int x, int y) {
	return pData[y * width + x];
}

Drawable *Drawable::fromFile(std::string sImageFile) {

	int width, height;

	////////////////////////////////////////////////////////////////////////////
	// Use libpng, Thanks to Guillaume Cottenceau
	// https://gist.github.com/niw/5963798

	png_structp png;
	png_infop info;

	sImageFile = PixFuPlatform::getPath(sImageFile);

	FILE *f = fopen(sImageFile.c_str(), "rb");
	if (!f) return nullptr;

	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) return nullptr;

	info = png_create_info_struct(png);
	if (!info) return nullptr;

	if (setjmp(png_jmpbuf(png))) return nullptr;

	png_init_io(png, f);
	png_read_info(png, info);

	png_byte color_type;
	png_byte bit_depth;
	png_bytep *row_pointers;
	width = png_get_image_width(png, info);
	height = png_get_image_height(png, info);
	color_type = png_get_color_type(png, info);
	bit_depth = png_get_bit_depth(png, info);

	if (bit_depth == 16) png_set_strip_16(png);
	if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
	if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
	if (color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
	if (color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);

	png_read_update_info(png, info);
	row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * height);
	for (int y = 0; y < height; y++) {
		row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(png, info));
	}
	png_read_image(png, row_pointers);
	////////////////////////////////////////////////////////////////////////////

	Drawable *d = new Drawable(width, height);

	// Iterate through image rows, converting into sprite format
	for (int y = 0; y < height; y++) {
		png_bytep row = row_pointers[y];
		for (int x = 0; x < width; x++) {
			png_bytep px = &(row[x * 4]);
			d->setPixel(x, y, rgl::Pixel(px[0], px[1], px[2], px[3]));
		}
	}

	fclose(f);
	return d;

}

void Drawable::clear(Pixel color) {
	for (int i = 0, l = width * height; i < l; i++)
		pData[i] = color;
}

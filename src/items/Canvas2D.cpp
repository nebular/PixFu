//
//  Geometry2D.cpp
//  PixFu
//
//  A class that can draw geometric shapes, colors and text to a Drawable. The engine class
//  creates one, that you can access by engine->canvas().  The world extension also creates
//  one that you can use to draw on the 3D world. Or you can create them yourself to draw to
//  a buffer.
//
//  All routines Copyright 2018 - 2019 OneLoneCoder.com
//

#include "Canvas2D.hpp"
#include <vector>
#include <cmath>

#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "err_ovl_no_viable_member_function_in_call"

namespace Pix {

	Canvas2D::Canvas2D(Drawable *target, Font *defaultFont) : pTarget(target) {
		pFont = defaultFont;
	}

	void
	Canvas2D::drawString(int32_t x, int32_t y, std::string sText, Pix::Pixel col, uint32_t scale) {
		if (pFont != nullptr)
			pFont->drawString(pTarget, x, y, sText, col, scale);
	}


	void Canvas2D::drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p,
							uint32_t pattern) {
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1;
		dy = y2 - y1;

		auto rol = [&](void) {
			pattern = (pattern << 1) | (pattern >> 31);
			return pattern & 1;
		};

		// straight lines idea by gurkanctn
		if (dx == 0) // Line is vertical
		{
			if (y2 < y1) std::swap(y1, y2);
			for (y = y1; y <= y2; y++)
				if (rol()) pTarget->setPixel(x1, y, p);
			return;
		}

		if (dy == 0) // Line is horizontal
		{
			if (x2 < x1) std::swap(x1, x2);
			for (x = x1; x <= x2; x++)
				if (rol()) pTarget->setPixel(x, y1, p);
			return;
		}

		// Line is Funk-aye
		dx1 = abs(dx);
		dy1 = abs(dy);
		px = 2 * dy1 - dx1;
		py = 2 * dx1 - dy1;
		if (dy1 <= dx1) {
			if (dx >= 0) {
				x = x1;
				y = y1;
				xe = x2;
			} else {
				x = x2;
				y = y2;
				xe = x1;
			}

			if (rol()) pTarget->setPixel(x, y, p);

			for (i = 0; x < xe; i++) {
				x = x + 1;
				if (px < 0)
					px = px + 2 * dy1;
				else {
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				if (rol()) pTarget->setPixel(x, y, p);
			}
		} else {
			if (dy >= 0) {
				x = x1;
				y = y1;
				ye = y2;
			} else {
				x = x2;
				y = y2;
				ye = y1;
			}

			if (rol()) pTarget->setPixel(x, y, p);

			for (i = 0; y < ye; i++) {
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else {
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				if (rol()) pTarget->setPixel(x, y, p);
			}
		}
	}

	void Canvas2D::drawCircle(int32_t x, int32_t y, int32_t radius, Pixel p, uint8_t mask) {
		int x0 = 0;
		int y0 = radius;
		int d = 3 - 2 * radius;
		if (!radius) return;

		while (y0 >= x0) // only formulate 1/8 of circle
		{
			if (mask & 0x01) pTarget->setPixel(x + x0, y - y0, p);
			if (mask & 0x02) pTarget->setPixel(x + y0, y - x0, p);
			if (mask & 0x04) pTarget->setPixel(x + y0, y + x0, p);
			if (mask & 0x08) pTarget->setPixel(x + x0, y + y0, p);
			if (mask & 0x10) pTarget->setPixel(x - x0, y + y0, p);
			if (mask & 0x20) pTarget->setPixel(x - y0, y + x0, p);
			if (mask & 0x40) pTarget->setPixel(x - y0, y - x0, p);
			if (mask & 0x80) pTarget->setPixel(x - x0, y - y0, p);
			if (d < 0) d += 4 * x0++ + 6;
			else d += 4 * (x0++ - y0--) + 10;
		}
	}

	void Canvas2D::fillCircle(int32_t x, int32_t y, int32_t radius, Pixel p) {
		// Taken from wikipedia
		int x0 = 0;
		int y0 = radius;
		int d = 3 - 2 * radius;
		if (!radius) return;

		auto drawline = [&](int sx, int ex, int ny) {
			for (int i = sx; i <= ex; i++)
				pTarget->setPixel(i, ny, p);
		};

		while (y0 >= x0) {
			// Modified to draw scan-lines instead of edges
			drawline(x - x0, x + x0, y - y0);
			drawline(x - y0, x + y0, y - x0);
			drawline(x - x0, x + x0, y + y0);
			drawline(x - y0, x + y0, y + x0);
			if (d < 0) d += 4 * x0++ + 6;
			else d += 4 * (x0++ - y0--) + 10;
		}
	}

	void Canvas2D::drawRect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p) {
		drawLine(x, y, x + w, y, p);
		drawLine(x + w, y, x + w, y + h, p);
		drawLine(x + w, y + h, x, y + h, p);
		drawLine(x, y + h, x, y, p);
	}

	void Canvas2D::fillRect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p) {
		int32_t x2 = x + w;
		int32_t y2 = y + h;

		int wmax = pTarget->width, hmax = pTarget->height;

		if (x < 0) x = 0;
		if (x >= (int32_t) wmax) x = (int32_t) wmax;
		if (y < 0) y = 0;
		if (y >= (int32_t) hmax) y = (int32_t) hmax;

		if (x2 < 0) x2 = 0;
		if (x2 >= (int32_t) wmax) x2 = (int32_t) wmax;
		if (y2 < 0) y2 = 0;
		if (y2 >= (int32_t) hmax) y2 = (int32_t) hmax;

		for (int i = x; i < x2; i++)
			for (int j = y; j < y2; j++)
				pTarget->setPixel(i, j, p);
	}

	void
	Canvas2D::drawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
						   Pixel p) {
		drawLine(x1, y1, x2, y2, p);
		drawLine(x2, y2, x3, y3, p);
		drawLine(x3, y3, x1, y1, p);
	}

// https://www.avrfreaks.net/sites/default/files/triangles.c
	void Canvas2D::fillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3,
								int32_t y3, Pixel p) {
		auto SWAP = [](int &x, int &y) {
			int t = x;
			x = y;
			y = t;
		};
		auto drawline = [&](int sx, int ex, int ny) {
			for (int i = sx; i <= ex; i++)
				pTarget->setPixel(i, ny, p);
		};

		int t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;
		// Sort vertices
		if (y1 > y2) {
			SWAP(y1, y2);
			SWAP(x1, x2);
		}
		if (y1 > y3) {
			SWAP(y1, y3);
			SWAP(x1, x3);
		}
		if (y2 > y3) {
			SWAP(y2, y3);
			SWAP(x2, x3);
		}

		t1x = t2x = x1;
		y = y1;   // Starting points
		dx1 = (int) (x2 - x1);
		if (dx1 < 0) {
			dx1 = -dx1;
			signx1 = -1;
		} else signx1 = 1;
		dy1 = (int) (y2 - y1);

		dx2 = (int) (x3 - x1);
		if (dx2 < 0) {
			dx2 = -dx2;
			signx2 = -1;
		} else signx2 = 1;
		dy2 = (int) (y3 - y1);

		if (dy1 > dx1) {   // swap values
			SWAP(dx1, dy1);
			changed1 = true;
		}
		if (dy2 > dx2) {   // swap values
			SWAP(dy2, dx2);
			changed2 = true;
		}

		e2 = dx2 >> 1;
		// Flat top, just process the second half
		if (y1 == y2) goto next;
		e1 = dx1 >> 1;

		for (int i = 0; i < dx1;) {
			t1xp = 0;
			t2xp = 0;
			if (t1x < t2x) {
				minx = t1x;
				maxx = t2x;
			} else {
				minx = t2x;
				maxx = t1x;
			}
			// process first line until y value is about to change
			while (i < dx1) {
				i++;
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) t1xp = signx1;//t1x += signx1;
					else goto next1;
				}
				if (changed1) break;
				else t1x += signx1;
			}
			// Move line
			next1:
			// process second line until y value is about to change
			while (1) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;//t2x += signx2;
					else goto next2;
				}
				if (changed2) break;
				else t2x += signx2;
			}
			next2:
			if (minx > t1x) minx = t1x;
			if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x;
			if (maxx < t2x) maxx = t2x;
			drawline(minx, maxx, y);    // Draw line from min to max points found on the y
			// Now increase y
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y == y2) break;

		}
		next:
		// Second half
		dx1 = (int) (x3 - x2);
		if (dx1 < 0) {
			dx1 = -dx1;
			signx1 = -1;
		} else signx1 = 1;
		dy1 = (int) (y3 - y2);
		t1x = x2;

		if (dy1 > dx1) {   // swap values
			SWAP(dy1, dx1);
			changed1 = true;
		} else changed1 = false;

		e1 = dx1 >> 1;

		for (int i = 0; i <= dx1; i++) {
			t1xp = 0;
			t2xp = 0;
			if (t1x < t2x) {
				minx = t1x;
				maxx = t2x;
			} else {
				minx = t2x;
				maxx = t1x;
			}
			// process first line until y value is about to change
			while (i < dx1) {
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) {
						t1xp = signx1;
						break;
					}//t1x += signx1;
					else goto next3;
				}
				if (changed1) break;
				else t1x += signx1;
				if (i < dx1) i++;
			}
			next3:
			// process second line until y value is about to change
			while (t2x != x3) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;
					else goto next4;
				}
				if (changed2) break;
				else t2x += signx2;
			}
			next4:

			if (minx > t1x) minx = t1x;
			if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x;
			if (maxx < t2x) maxx = t2x;
			drawline(minx, maxx, y);
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y > y3) return;
		}
	}

	void
	Canvas2D::drawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates,
								 float x, float y, float r, float s, std::vector<Pix::Pixel> col) {
		// pair.first = x coordinate
		// pair.second = y coordinate

		// Create translated model vector of coordinate pairs
		std::vector<std::pair<float, float>> vecTransformedCoordinates;
		unsigned int verts = (int) vecModelCoordinates.size();
		vecTransformedCoordinates.resize(verts);

		// Rotate
		for (int i = 0; i < verts; i++) {
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) -
												 vecModelCoordinates[i].second * sinf(r);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) +
												  vecModelCoordinates[i].second * cosf(r);

			// Scale
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;

			// Translate
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
		}

		// Draw Closed Polygon
		for (int i = 0; i < verts + 1; i++) {
			int j = (i + 1);
			drawLine((int) vecTransformedCoordinates[i % verts].first,
					 (int) vecTransformedCoordinates[i % verts].second,
					 (int) vecTransformedCoordinates[j % verts].first,
					 (int) vecTransformedCoordinates[j % verts].second, col[i % col.size()]);
		}
	}

	void Canvas2D::drawSprite(int32_t x, int32_t y, Pix::Drawable *sprite, uint32_t sampleWidth) {
		if (sprite == nullptr) return;

		if (sampleWidth != 0) {
			for (int32_t i = 0, l1 = sprite->width >> sampleWidth; i < l1; i++)
				for (int32_t j = 0, l2 = sprite->height >> sampleWidth; j < l2; j++)
					setPixel(x + i, y + j, sprite->getPixel(i << sampleWidth, j << sampleWidth));
		} else {
			for (int32_t i = 0; i < sprite->width; i++)
				for (int32_t j = 0; j < sprite->height; j++)
					setPixel(x + i, y + j, sprite->getPixel(i, j));
		}
	}


}

#pragma clang diagnostic pop

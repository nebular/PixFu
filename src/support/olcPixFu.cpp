//
//  olcPixFu.cpp
//  PixEngine
//
//  Created by rodo on 14/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include <arch/android/plugins/lonescreenkey.h>

#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "olcPixFu.hpp"

namespace rgl {

	olcPixFu::olcPixFu(std::string appName, std::string shaderName)
			: PixFu(appName, shaderName) {};

	HwButton_t olcPixFu::GetMouse(int button) {
		Mouse *m = Mouse::instance();
		return {m->isPressed(button), m->isHeld(button), m->isReleased(button)};
	}

	HwButton_t olcPixFu::GetKey(Keys key) {
		Keyboard *k = Keyboard::instance();
		return {k->isPressed(key), k->isHeld(key), k->isReleased(key)};
	}

	int olcPixFu::GetMouseX() {
		return Mouse::x();
	}

	int olcPixFu::GetMouseY() {
		return Mouse::y();
	}

	int olcPixFu::ScreenWidth() {
		return screenWidth();
	}

	int olcPixFu::ScreenHeight() {
		return screenHeight();
	}

	bool olcPixFu::onUserCreate(bool restarted) {
		bRestarted = restarted;
		return OnUserCreate();
	}

	bool olcPixFu::isRestarted() { return bRestarted; }

	bool olcPixFu::onUserUpdate(float fElapsedTime) {

		bool result = OnUserUpdate(fElapsedTime);

		LoneScreenKey *l = rgl::LoneScreenKey::currentInstance;
		if (l != nullptr) l->DrawSelf(canvas(), rgl::Colors::WHITE, true);

		return result;

	}

	void olcPixFu::Draw(uint32_t x, uint32_t y, rgl::Pixel color) {
		canvas()->setPixel(x, y, color);
	}

	void olcPixFu::Clear(rgl::Pixel color) {
		canvas()->clear(color);
	}

	void olcPixFu::DrawString(int32_t x, int32_t y, std::string sText, rgl::Pixel col,
							  uint32_t scale) {
		canvas()->drawString(x, y, sText, col, scale);
	}

	void
	olcPixFu::DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates,
								 float x,
								 float y, float r, float s, std::vector<rgl::Pixel> col) {
		canvas()->drawWireFrameModel(vecModelCoordinates, x, y, r, s, col);
	}

	void olcPixFu::DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, rgl::Pixel p,
							uint32_t pattern) {
		canvas()->drawLine(x1, y1, x2, y2, p, pattern);
	}


	void olcPixFu::DrawCircle(int32_t x, int32_t y, int32_t radius, rgl::Pixel p, uint8_t mask) {
		canvas()->drawCircle(x, y, radius, p, mask);
	}


	void olcPixFu::FillCircle(int32_t x, int32_t y, int32_t radius, rgl::Pixel p) {
		canvas()->fillCircle(x, y, radius, p);
	}

	void olcPixFu::DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, rgl::Pixel p) {
		canvas()->drawRect(x, y, w, h, p);
	}

	void olcPixFu::FillRect(int32_t x, int32_t y, int32_t w, int32_t h, rgl::Pixel p) {
		canvas()->fillRect(x, y, w, h, p);
	}

	void
	olcPixFu::DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
						   rgl::Pixel p) {
		canvas()->drawTriangle(x1, y1, x2, y2, x3, y3, p);
	}

	void
	olcPixFu::FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
						   rgl::Pixel p) {
		canvas()->fillTriangle(x1, y1, x2, y2, x3, y3, p);
	}

	void olcPixFu::DrawSprite(int32_t x, int32_t y, rgl::Drawable *sprite, uint32_t sampleWidth) {
		canvas()->drawSprite(x, y, sprite, sampleWidth);
	}

}
#pragma clang diagnostic pop

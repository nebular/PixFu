//
//  FuOLC.cpp
//  PixEngine
//
//  Created by rodo on 14/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

// #include <arch/android/plugins/lonescreenkey.h>

#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "FuOLC.hpp"

namespace Pix {

	FuOLC::FuOLC(std::string appName, std::string shaderName)
			: Fu(appName, shaderName) {};

	HwButton_t FuOLC::GetMouse(int button) {
		Mouse *m = Mouse::instance();
		return {m->isPressed(button), m->isHeld(button), m->isReleased(button)};
	}

	HwButton_t FuOLC::GetKey(Keys key) {
		Keyboard *k = Keyboard::instance();
		return {k->isPressed(key), k->isHeld(key), k->isReleased(key)};
	}

	int FuOLC::GetMouseX() {
		return Mouse::x();
	}

	int FuOLC::GetMouseY() {
		return Mouse::y();
	}

	int FuOLC::ScreenWidth() {
		return screenWidth();
	}

	int FuOLC::ScreenHeight() {
		return screenHeight();
	}

	bool FuOLC::onUserCreate(bool restarted) {
		bRestarted = restarted;
		return OnUserCreate();
	}

	bool FuOLC::isRestarted() { return bRestarted; }

	bool FuOLC::onUserUpdate(float fElapsedTime) {

		bool result = OnUserUpdate(fElapsedTime);

//		LoneScreenKey *l = Pix::LoneScreenKey::currentInstance;
//		if (l != nullptr) l->DrawSelf(canvas(), Pix::Colors::WHITE, true);

		return result;

	}

	void FuOLC::Draw(uint32_t x, uint32_t y, Pix::Pixel color) {
		canvas()->setPixel(x, y, color);
	}

	void FuOLC::Clear(Pix::Pixel color) {
		canvas()->clear(color);
	}

	void FuOLC::DrawString(int32_t x, int32_t y, std::string sText, Pix::Pixel col,
						   uint32_t scale) {
		canvas()->drawString(x, y, sText, col, scale);
	}

	void
	FuOLC::DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates,
							  float x,
							  float y, float r, float s, std::vector<Pix::Pixel> col) {
		canvas()->drawWireFrameModel(vecModelCoordinates, x, y, r, s, col);
	}

	void FuOLC::DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pix::Pixel p,
						 uint32_t pattern) {
		canvas()->drawLine(x1, y1, x2, y2, p, pattern);
	}


	void FuOLC::DrawCircle(int32_t x, int32_t y, int32_t radius, Pix::Pixel p, uint8_t mask) {
		canvas()->drawCircle(x, y, radius, p, mask);
	}


	void FuOLC::FillCircle(int32_t x, int32_t y, int32_t radius, Pix::Pixel p) {
		canvas()->fillCircle(x, y, radius, p);
	}

	void FuOLC::DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, Pix::Pixel p) {
		canvas()->drawRect(x, y, w, h, p);
	}

	void FuOLC::FillRect(int32_t x, int32_t y, int32_t w, int32_t h, Pix::Pixel p) {
		canvas()->fillRect(x, y, w, h, p);
	}

	void
	FuOLC::DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
						Pix::Pixel p) {
		canvas()->drawTriangle(x1, y1, x2, y2, x3, y3, p);
	}

	void
	FuOLC::FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
						Pix::Pixel p) {
		canvas()->fillTriangle(x1, y1, x2, y2, x3, y3, p);
	}

	void FuOLC::DrawSprite(int32_t x, int32_t y, Pix::Drawable *sprite, uint32_t sampleWidth) {
		canvas()->drawSprite(x, y, sprite, sampleWidth);
	}

}
#pragma clang diagnostic pop

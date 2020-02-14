//
//  olcPixFu.hpp
//  PixEngine
//
//  This is a base engine that emulates OneLoneCoder PGE interface
//  You can run PGE classes extending olcPixfu rather than olcPixelEngine
//
//  Created by rodo on 14/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef olcPixFu_hpp
#define olcPixFu_hpp

#include "PixFu.hpp"
#include "Canvas2D.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"

namespace rgl {

typedef struct HwButton {
	bool bPressed, bHeld, bReleased;
} HwButton_t;


class olcPixFu : public PixFu {
	
	Canvas2D *pCanvas;
	
public:
	
	HwButton_t GetMouse(int button);
	HwButton_t GetKey(Keys key);
	
	
	int GetMouseX();
	
	int GetMouseY();
	
	int ScreenWidth();
	
	int ScreenHeight();
	
	bool onUserCreate(bool restarted) override;
	
	bool onUserUpdate(float fElapsedTime) override;
	
	virtual bool OnUserUpdate(float fElapsedTime) = 0;
	virtual bool OnUserCreate() = 0;
	
	void Clear(rgl::Pixel color = rgl::Pixel(0, 0, 0, 0));
	
	void DrawString(int32_t x, int32_t y, std::string sText, rgl::Pixel col,
					uint32_t scale = 1);
	
	void
	DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x,
					   float y, float r, float s, std::vector<rgl::Pixel> col);
	
	void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, rgl::Pixel p,
				  uint32_t pattern = 0xFFFFFFFF);
	
	
	void DrawCircle(int32_t x, int32_t y, int32_t radius, rgl::Pixel p, uint8_t mask = 0xff);
	
	void FillCircle(int32_t x, int32_t y, int32_t radius, rgl::Pixel p);
	
	void DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, rgl::Pixel p);
	
	void FillRect(int32_t x, int32_t y, int32_t w, int32_t h, rgl::Pixel p);
	
	void DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
					  rgl::Pixel p);
	
	void FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
					  rgl::Pixel p);
	
};



HwButton_t olcPixFu::GetMouse(int button) {
	Mouse *m = Mouse::instance();
	return {m->isPressed(button), m->isHeld(button), m->isReleased(button)};
}

HwButton_t olcPixFu::GetKey(Keys key) {
	Keyboard *m = Keyboard::instance();
	return {m->isPressed(key), m->isHeld(key), m->isReleased(key)};
}

int  olcPixFu::GetMouseX() {
	return Mouse::x();
}

int  olcPixFu::GetMouseY() {
	return Mouse::y();
}

int  olcPixFu::ScreenWidth() {
	return screenWidth();
}

int  olcPixFu::ScreenHeight() {
	return screenHeight();
}
bool olcPixFu::onUserCreate(bool restarted)  {
	pCanvas = new Canvas2D(buffer(), new Font());
	return OnUserCreate();
}

bool olcPixFu::onUserUpdate(float fElapsedTime)  {
	return OnUserUpdate(fElapsedTime);
}

void olcPixFu::Clear(rgl::Pixel color) {
	pCanvas->clear(color);
}

void olcPixFu::DrawString(int32_t x, int32_t y, std::string sText, rgl::Pixel col,
						  uint32_t scale) {
	pCanvas->drawString(x, y, sText, col, 1);
}

void
olcPixFu::DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x,
							 float y, float r, float s, std::vector<rgl::Pixel> col) {
	pCanvas->drawWireFrameModel(vecModelCoordinates, x, y, r, s, col);
}

void olcPixFu::DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, rgl::Pixel p,
						uint32_t pattern) {
	pCanvas->drawLine(x1, y1, x2, y2, p, pattern);
}


void olcPixFu::DrawCircle(int32_t x, int32_t y, int32_t radius, rgl::Pixel p, uint8_t mask) {
	pCanvas->drawCircle(x,y,radius, p, mask);
}


void olcPixFu::FillCircle(int32_t x, int32_t y, int32_t radius, rgl::Pixel p) {
	pCanvas->fillCircle(x, y, radius, p);
}

void olcPixFu::DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, rgl::Pixel p) {
	pCanvas->drawRect(x, y, w, h, p);
}

void olcPixFu::FillRect(int32_t x, int32_t y, int32_t w, int32_t h, rgl::Pixel p) {
	pCanvas->fillRect(x, y, w, h, p);
}

void olcPixFu::DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
							rgl::Pixel p) {
	pCanvas->drawTriangle(x1, y1, x2, y2, x3, y3, p);
}

void olcPixFu::FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
							rgl::Pixel p) {
	pCanvas->fillTriangle(x1, y1, x2, y2, x3, y3, p);
}


}
#endif /* olcPixFu_hpp */

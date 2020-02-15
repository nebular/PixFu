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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

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
	bool bRestarted = false;

protected:
	bool isRestarted();

public:

	olcPixFu(std::string appName, std::string shaderName);
	
	HwButton_t GetMouse(int button);
	HwButton_t GetKey(Keys key);
	
	
	Canvas2D *canvas();
	
	int GetMouseX();
	
	int GetMouseY();
	
	int ScreenWidth();
	
	int ScreenHeight();
	
	virtual bool onUserCreate(bool restarted) override;
	
	bool onUserUpdate(float fElapsedTime) override;
	
	virtual bool OnUserUpdate(float fElapsedTime) = 0;
	virtual bool OnUserCreate() = 0;
	
	void Clear(rgl::Pixel color = rgl::Pixel(0, 0, 0, 0));
	
	void DrawString(int32_t x, int32_t y, std::string sText, rgl::Pixel col,
					uint32_t scale = 1);
	
	void Draw(uint32_t x, uint32_t y, rgl::Pixel color);
	
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
	
	void DrawSprite(int32_t x, int32_t y, rgl::Drawable *sprite, uint32_t sampleWidth = 1);

};

inline Canvas2D *olcPixFu::canvas() {
	return pCanvas;
}

}
#endif /* olcPixFu_hpp */

#pragma clang diagnostic pop

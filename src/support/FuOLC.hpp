//
//  FuOLC.hpp
//  PixEngine
//
//  This is a base engine that emulates OneLoneCoder PGE interface
//  You can run PGE classes extending olcPixfu rather than olcPixelEngine
//
//  Created by rodo on 14/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#ifndef olcPixFu_hpp
#define olcPixFu_hpp

#include "Fu.hpp"
#include "Canvas2D.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"

namespace Pix {

	typedef struct HwButton {
		bool bPressed, bHeld, bReleased;
	} HwButton_t;


	class FuOLC : public Fu {

		bool bRestarted = false;

	protected:

		bool isRestarted();

	public:

		FuOLC(std::string appName = "olcPixFuApp", FuConfig_t configuration = {});

		HwButton_t GetMouse(int button);

		HwButton_t GetKey(Keys key);

		int GetMouseX();

		int GetMouseY();

		int ScreenWidth();

		int ScreenHeight();

		virtual bool onUserCreate(bool restarted) override;

		bool onUserUpdate(float fElapsedTime) override;

		virtual bool OnUserUpdate(float fElapsedTime) = 0;

		virtual bool OnUserCreate() = 0;

		void Clear(Pix::Pixel color = Pix::Pixel(0, 0, 0, 0));

		void DrawString(int32_t x, int32_t y, std::string sText, Pix::Pixel col,
						uint32_t scale = 1);

		void Draw(uint32_t x, uint32_t y, Pix::Pixel color);

		void
		DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x,
						   float y, float r, float s, std::vector<Pix::Pixel> col);

		void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pix::Pixel p,
					  uint32_t pattern = 0xFFFFFFFF);


		void DrawCircle(int32_t x, int32_t y, int32_t radius, Pix::Pixel p, uint8_t mask = 0xff);

		void FillCircle(int32_t x, int32_t y, int32_t radius, Pix::Pixel p);

		void DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, Pix::Pixel p);

		void FillRect(int32_t x, int32_t y, int32_t w, int32_t h, Pix::Pixel p);

		void DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
						  Pix::Pixel p);

		void FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
						  Pix::Pixel p);

		void DrawSprite(int32_t x, int32_t y, Pix::Drawable *sprite, uint32_t sampleWidth = 1);

	};

}
#endif /* olcPixFu_hpp */

#pragma clang diagnostic pop

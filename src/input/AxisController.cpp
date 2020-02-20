#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "readability-isolate-declaration"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

//
// Created by rodo on 2020-02-10.
//

#include "AxisController.hpp"
#include "Canvas2D.hpp"
#include <cmath>


namespace rgl {

	AxisController::AxisController(int axisXlen, int axisYlen)
			: fAxisX(0),
			  fAxisY(0),
			  fNextAxisX(0),
			  fNextAxisY(0),
			  fCurrentX(0),
			  fCurrentY(0),
			  nAxisXLength(axisXlen),
			  nAxisYLength(axisYlen) {}

	AxisController::~AxisController() = default;

	void AxisController::drawSelf(Canvas2D *canvas, Pixel color) {


		int
				SW2 = canvas->width() / 2,
				SH2 = canvas->height() / 2;

		int
				MARGINV = 20,
				MARGINH = 20,
				XSIZE = nAxisXLength,
				YSIZE = nAxisYLength;

		// y axis
		int
				HX = 2 * SW2 - MARGINV,
				HS2 = YSIZE / 2;

		// pitch vertical
		canvas->drawLine(HX, SH2 - HS2, HX, SH2 + HS2, color);
		canvas->fillCircle(HX, static_cast<int32_t>(SH2 + HS2 * fAxisY), static_cast<int32_t>(5.0f), color);

		// x axis
		int
				HY = 2 * SH2 - MARGINH,
				WS2 = XSIZE / 2;

		// azimuth horizontal
		canvas->drawLine(SW2 - WS2, HY, SW2 + WS2, HY, color);
		canvas->fillCircle(static_cast<int32_t>(SW2 + WS2 * fAxisX), HY, static_cast<int32_t>(5.0f), color);

	}


	void AxisController::inputNormalized(float xAxis, float yAxis) {
		fNextAxisX = xAxis;
		fNextAxisY = yAxis;
	}

	void AxisController::inputGyroscope(float radAzimuth, float radPitch) {
		float xAxis = sin(radAzimuth);
		float yAxis = sin(-radPitch);
		inputNormalized(xAxis, yAxis);
	}

	void AxisController::sync(float fElapsedTime) {

		// dump new state into old
		fAxisX = fNextAxisX;
		fAxisY = fNextAxisY;

		// process interpolation

		constexpr float THR = 0.001;
		const float STEP = fElapsedTime / 4;

		if (fCurrentX < fAxisX) fCurrentX += STEP;
		else if (fCurrentX > fAxisX) fCurrentX -= STEP;
		if (fCurrentY < fAxisY) fCurrentY += STEP;
		else if (fCurrentY > fAxisY) fCurrentY -= STEP;

		float dx = fabs(fCurrentX - fAxisX);
		float dy = fabs(fCurrentY - fAxisY);

		if (dx < THR) fCurrentX = fAxisX;
		if (dy < THR) fCurrentY = fAxisY;
	}

}
#pragma clang diagnostic pop
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCSimplifyInspection"
#pragma ide diagnostic ignored "readability-magic-numbers"
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
#pragma ide diagnostic ignored "OCDFAInspection"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "readability-isolate-declaration"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

//
// Created by rodo on 2020-02-10.
//

#include "AxisController.hpp"
#include "Canvas2D.hpp"
#include "Utils.hpp"
#include <cmath>


namespace Pix {

	GenericAxisController *GenericAxisController::pInstance = nullptr;

	GenericAxisController::GenericAxisController(AxisControllerConfig_t config)
			: AxisController::AxisController(config) {}

	AxisController::AxisController(AxisControllerConfig_t config)
			: fAxisX(0),
			  fAxisY(0),
			  fNextAxisX(0),
			  fNextAxisY(0),
			  fCurrentX(0),
			  fCurrentY(0),
			  CONFIG(config)
			  {}

	AxisController::~AxisController() = default;

	void AxisController::drawSelf(Canvas2D *canvas, Pixel color) {

		int
				SW2 = canvas->width() / 2,
				SH2 = canvas->height() / 2;

		int
				MARGINV = 20,
				MARGINH = 20,
				XSIZE = (int) (SW2 * 0.7F),
				YSIZE = (int) (SW2 * 0.7F);

		// y axis
		int
				HX = 2 * SW2 - MARGINV,
				HS2 = YSIZE / 2;

		float pos = CONFIG.INVY ? -fAxisY : fAxisY;

		// pitch vertical
		canvas->drawLine(HX, SH2 - HS2, HX, SH2 + HS2, color);
		canvas->fillCircle(HX, static_cast<int32_t>(SH2 + HS2 * pos), static_cast<int32_t>(5.0F), color);

		// x axis
		int
				HY = 2 * SH2 - MARGINH,
				WS2 = XSIZE / 2;

		// azimuth horizontal
		canvas->drawLine(SW2 - WS2, HY, SW2 + WS2, HY, color);
		canvas->fillCircle(static_cast<int32_t>(SW2 + WS2 * fAxisX), HY, static_cast<int32_t>(5.0F), color);

	}

	void AxisController::inputIncremental(float xdelta, float ydelta) {
		if (xdelta != 0) nInputCounterX += 2;
		if (nInputCounterX > 6) nInputCounterX = 6;
		if (ydelta != 0) nInputCounterY += 2;
		if (nInputCounterY > 6) nInputCounterY = 6;
		inputNormalized(fNextAxisX + xdelta, fNextAxisY + ydelta);
	}

	void AxisController::inputNormalized(float xAxis, float yAxis) {

		if (xAxis > CONFIG.XMAX) xAxis = CONFIG.XMAX;
		if (xAxis < CONFIG.XMIN) xAxis = CONFIG.XMIN;

		if (yAxis > CONFIG.YMAX) yAxis = CONFIG.YMAX;
		if (yAxis < CONFIG.YMIN) yAxis = CONFIG.YMIN;

		fNextAxisX = xAxis;
		fNextAxisY = yAxis;

		if (DBG)
			LogV(TAG, SF("axis input %f %f", xAxis, yAxis));

	}

	void AxisController::inputGyroscope(float radAzimuth, float radPitch) {
		float xAxis = sin(radAzimuth);
		float yAxis = sin(-radPitch);
		inputNormalized(xAxis, yAxis);
	}

	void AxisController::sync(float fElapsedTime) {

		fAxisX = fNextAxisX;
		fAxisY = fNextAxisY;

		if (nInputCounterX > 0) nInputCounterX--;
		if (nInputCounterY > 0) nInputCounterY--;

		// process interpolation

		constexpr float THR = 0.001;

		// auto center constants
		if (nInputCounterX == 0) fAxisX *= CONFIG.AUTOX;
		if (nInputCounterY == 0) fAxisY *= CONFIG.AUTOY;

		float lerp = 4;

		fCurrentX += (fAxisX - fCurrentX) * lerp * fElapsedTime;
		fCurrentY += (fAxisY - fCurrentY) * lerp * fElapsedTime;

		float dx = fabs(fCurrentX - fAxisX);
		float dy = fabs(fCurrentY - fAxisY);

		if (dx < THR) fCurrentX = fAxisX;
		if (dy < THR) fCurrentY = fAxisY;

		fNextAxisX = fAxisX;
		fNextAxisY = fAxisY;

	}

}
#pragma clang diagnostic pop

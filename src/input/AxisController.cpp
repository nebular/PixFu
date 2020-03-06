#pragma clang diagnostic push
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
			  CONFIG(std::move(config))
			  {}

	AxisController::~AxisController() = default;

	void AxisController::drawSelf(Canvas2D *canvas, Pixel color) {

		int
				SW2 = canvas->width() / 2,
				SH2 = canvas->height() / 2;

		int
				MARGINV = 20,
				MARGINH = 20,
				XSIZE = (int) (SW2 * 0.7),
				YSIZE = (int) (SW2 * 0.7);

		// y axis
		int
				HX = 2 * SW2 - MARGINV,
				HS2 = YSIZE / 2;

		float pos = CONFIG.INVY ? -fAxisY : fAxisY;

		// pitch vertical
		canvas->drawLine(HX, SH2 - HS2, HX, SH2 + HS2, color);
		canvas->fillCircle(HX, static_cast<int32_t>(SH2 + HS2 * pos), static_cast<int32_t>(5.0f), color);

		// x axis
		int
				HY = 2 * SH2 - MARGINH,
				WS2 = XSIZE / 2;

		// azimuth horizontal
		canvas->drawLine(SW2 - WS2, HY, SW2 + WS2, HY, color);
		canvas->fillCircle(static_cast<int32_t>(SW2 + WS2 * fAxisX), HY, static_cast<int32_t>(5.0f), color);

	}

	void AxisController::inputIncremental(float xdelta, float ydelta) {
		if (xdelta != 0) nInputCounter += 2;
		if (nInputCounter > 5) nInputCounter = 5;
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
			LogV(TAG, SF("axis input %f %f %d", xAxis, yAxis, nInputCounter));

	}

	void AxisController::inputGyroscope(float radAzimuth, float radPitch) {
		float xAxis = sin(radAzimuth);
		float yAxis = sin(-radPitch);
		inputNormalized(xAxis, yAxis);
	}

	void AxisController::sync(float fElapsedTime) {

		fAxisX = fNextAxisX;
		fAxisY = fNextAxisY;

		if (nInputCounter > 0)
			nInputCounter--;
		
		// process interpolation

		constexpr float THR = 0.001;

		// auto center constants
		if (nInputCounter == 0) {
			fAxisX *= CONFIG.AUTOX;
			fAxisY *= CONFIG.AUTOY;
		}

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

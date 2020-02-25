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


namespace rgl {

	GenericAxisController *GenericAxisController::pInstance = nullptr;

	GenericAxisController::GenericAxisController(float xmin, float xmax, float ymin, float ymax, bool autoCenterX, bool autoCenterY,
												 bool invx, bool invy)
			: AxisController::AxisController(xmin, xmax, ymin, ymax, autoCenterX, autoCenterY, invx, invy) {}


	AxisController::AxisController()
			: AxisController::AxisController(-1, 1, -1, 1, true, true) {}

	AxisController::AxisController(float xmin, float xmax, float ymin, float ymax, bool autoCenterX, bool autoCenterY, bool xinv, bool yinv)
			: fAxisX(0),
			  fAxisY(0),
			  fNextAxisX(0),
			  fNextAxisY(0),
			  fCurrentX(0),
			  fCurrentY(0),
			  XMIN(xmin), XMAX(xmax), YMIN(ymin), YMAX(ymax),
			  INVX(xinv), INVY(yinv),
			  AUTOX(autoCenterX), AUTOY(autoCenterY) {}

	AxisController::~AxisController() = default;

	void AxisController::drawSelf(Canvas2D *canvas, Pixel color) {


		int
				SW2 = canvas->width() / 2,
				SH2 = canvas->height() / 2;

		int
				MARGINV = 20,
				MARGINH = 20,
				XSIZE = (int)(SW2 * 0.7),
				YSIZE = (int)(SW2 * 0.7);

		// y axis
		int
				HX = 2 * SW2 - MARGINV,
				HS2 = YSIZE / 2;

		float pos = INVY ? -fAxisY : fAxisY;

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
		if (nInputCounter > 20) nInputCounter = 20;
		inputNormalized(fNextAxisX + xdelta, fNextAxisY + ydelta);
	}

	void AxisController::inputNormalized(float xAxis, float yAxis) {

		if (xAxis > XMAX) xAxis = XMAX;
		if (xAxis < XMIN) xAxis = XMIN;

		if (yAxis > YMAX) yAxis = YMAX;
		if (yAxis < YMIN) yAxis = YMIN;

		fNextAxisX = xAxis;
		fNextAxisY = yAxis;


		LogV("axis", SF("axis input %f %f %d", xAxis, yAxis, nInputCounter));

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

		const float STEP = fElapsedTime / 6;
		const float RECOVERY = STEP * 4;

		if (nInputCounter == 0) {
			if (AUTOX) {
				if (fAxisX > 0) {
					fAxisX -= RECOVERY;
				} else if (XMIN < 0 && fAxisX < 0) {
					fAxisX += RECOVERY;
				}
			}

			if (AUTOY) {
				if (fAxisY > 0) {
					fAxisY -= RECOVERY;
				} else if (YMIN < 0 && fAxisY < 0) {
					fAxisY += RECOVERY;
				}
			}
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

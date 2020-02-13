//
// Created by rodo on 2020-02-10.
//

#include "AxisController.hpp"
#include "Canvas2D.hpp"
#include <cmath>

namespace rgl {

	AxisController::AxisController(int axisXlen, int axisYlen) : nAxisXLength(axisXlen),
																 nAxisYLength(axisYlen) {}

	AxisController::~AxisController() {}

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
		canvas->fillCircle(HX, SH2 + HS2 * fAxisY, 5.0, color);

		// x axis
		int
				HY = 2 * SH2 - MARGINH,
				WS2 = XSIZE / 2;

		// azimuth horizontal
		canvas->drawLine(SW2 - WS2, HY, SW2 + WS2, HY, color);
		canvas->fillCircle(SW2 + WS2 * fAxisX, HY, 5.0, color);

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

	void AxisController::sync() {
		// dump new state into old
		fAxisX = fNextAxisX;
		fAxisY = fNextAxisY;
	}

}
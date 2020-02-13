//
// Created by rodo on 2020-02-10.
//

#ifndef OLCPGE_LONEKART_ANDROID_AXISCONTROLLER_H
#define OLCPGE_LONEKART_ANDROID_AXISCONTROLLER_H

#include "PixFu.hpp"
#include "Canvas2D.hpp"

namespace rgl {

	class AxisController : public InputDevice {


		static AxisController *pCurrentInstance;
		float fAxisX, fAxisY, fNextAxisX, fNextAxisY;
		int nAxisXLength, nAxisYLength;


	public:

		AxisController(int axisXlen, int axisYlen);
		virtual ~AxisController();

		void inputGyroscope(float radAzimuth, float radPitch);

		void inputNormalized(float xAxis, float yAxis);

		void drawSelf(Canvas2D *canvas, Pixel color);

		float xNorm();

		float yNorm();

		void sync();
	};

	inline float AxisController::xNorm() { return fAxisX; }

	inline float AxisController::yNorm() { return fAxisY; }

}
#endif //OLCPGE_LONEKART_ANDROID_AXISCONTROLLER_H

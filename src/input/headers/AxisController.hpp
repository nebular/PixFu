//
// Created by rodo on 2020-02-10.
//

#ifndef OLCPGE_LONEKART_ANDROID_AXISCONTROLLER_H
#define OLCPGE_LONEKART_ANDROID_AXISCONTROLLER_H

#include "PixEngine.hpp"
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

		void DrawSelf(Canvas2D *canvas, Pixel color);

		float xNorm();

		float yNorm();

		float xRads(float max);

		float yRads(float max);

		void sync();
	};

	inline float AxisController::xNorm() { return fAxisX; }

	inline float AxisController::yNorm() { return fAxisY; }

	inline float AxisController::xRads(float maxRads) { return fAxisX * maxRads / 2.0f; }

	inline float AxisController::yRads(float maxRads) { return fAxisY * maxRads / 2.0f; }


}
#endif //OLCPGE_LONEKART_ANDROID_AXISCONTROLLER_H

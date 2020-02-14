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
		float fAxisX, fAxisY, fNextAxisX, fNextAxisY;    // raw
		float fCurrentX, fCurrentY;                        // interpolated

		int nAxisXLength, nAxisYLength;                    // drawSelf


	public:

		AxisController(int axisXlen, int axisYlen);

		virtual ~AxisController();

		void inputGyroscope(float radAzimuth, float radPitch);

		void inputNormalized(float xAxis, float yAxis);

		void drawSelf(Canvas2D *canvas, Pixel color);

		/**
		 * Get raw X axis value
		 * @return The normalized axis value
		 */
		float x();

		/**
		 * Get raw Y axis value
		 * @return The normalized axis value
		 */
		float y();

		/**
		 * Get interpolated X axis value
		 * @return The normalized interpolated axis value
		 */
		float xInterp();

		/**
		 * Get interpolated Y axis value
		 * @return The normalized interpolated axis value
		 */
		float yInterp();

		/**
		 * Updates inputdevice values
		 * @param fElapsedTime  frame time
		 */
		void sync(float fElapsedTime);
	};

	inline float AxisController::x() { return fAxisX; }

	inline float AxisController::y() { return fAxisY; }

	inline float AxisController::xInterp() { return fCurrentX; }

	inline float AxisController::yInterp() { return fCurrentY; }
}
#endif //OLCPGE_LONEKART_ANDROID_AXISCONTROLLER_H

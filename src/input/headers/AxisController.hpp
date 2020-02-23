//
// Created by rodo on 2020-02-10.
//

#ifndef OLCPGE_LONEKART_ANDROID_AXISCONTROLLER_H
#define OLCPGE_LONEKART_ANDROID_AXISCONTROLLER_H

#include "PixFu.hpp"
#include "Canvas2D.hpp"

namespace rgl {

	class AxisController : public InputDevice {

		float fAxisX, fAxisY, fNextAxisX, fNextAxisY;    // raw
		float fCurrentX, fCurrentY;                        // interpolated

		const float XMIN, XMAX, YMIN, YMAX;
		const bool AUTOX, AUTOY, INVX, INVY;
		
	public:

		AxisController();
		AxisController(float xmin = -1, float xmax=1, float ymin = -1, float ymax = 1, bool autoX=false, bool autoY=false, bool invx=false, bool invy=false);

		virtual ~AxisController();

		void inputGyroscope(float radAzimuth, float radPitch);

		void inputNormalized(float xAxis, float yAxis);

		void inputIncremental(float xdelta, float ydelta);
		
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

	class GenericAxisController : public AxisController {

		static GenericAxisController *pInstance;

	public:
		GenericAxisController(float xmin=-1, float ymin=-1, float xmax=-1, float ymax=-1, bool autoX=true, bool autoY=true, bool xinv=false, bool yinv=false);
		static void enable(float xmin=-1, float ymin=-1, float xmax=-1, float ymax=-1, bool autoX=true, bool autoY=true, bool xinv=false, bool yinv=false);
		static void disable();
		static GenericAxisController *instance();
		inline void poll() {}
	};
	
	inline GenericAxisController *GenericAxisController::instance() { return pInstance; }

	inline void GenericAxisController::enable(float xmin, float xmax, float ymin, float ymax, bool autoX, bool autoY, bool xinv, bool yinv) {
		if (pInstance == nullptr)
			pInstance = new GenericAxisController(xmin, xmax, ymin, ymax, autoX, autoY, xinv, yinv);
	}

	inline void GenericAxisController::disable() {
		if (pInstance != nullptr) {
			delete pInstance;
			pInstance = nullptr;
		}
	}

	inline float AxisController::x() { return fAxisX; }

	inline float AxisController::y() { return fAxisY; }

	inline float AxisController::xInterp() { return fCurrentX; }

	inline float AxisController::yInterp() { return fCurrentY; }
}
#endif //OLCPGE_LONEKART_ANDROID_AXISCONTROLLER_H

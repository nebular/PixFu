//
// Created by rodo on 2020-02-10.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#pragma once

#include "PixFu.hpp"
#include "Canvas2D.hpp"

namespace rgl {

	class AxisController : public InputDevice {

		float fAxisX, fAxisY, fNextAxisX, fNextAxisY;    // raw
		float fCurrentX, fCurrentY;                      // interpolated
		int nInputCounter = 0;

		const float XMIN, XMAX, YMIN, YMAX;
		const bool AUTOX, AUTOY, INVX, INVY;

	public:

		AxisController();

		/**
		 * Constructs the Axis Controller
		 * @param xmin Minimum calue for the X axis
		 * @param xmax MAximum calue for the X axis
		 * @param ymin Minimum calue for the Y axis
		 * @param ymax MAximum calue for the X axis
		 * @param autoX Whether to auto-center X if no input
		 * @param autoY Whether to auto-center Y if no input
		 * @param invx Whether to display the axis inverted
		 * @param invy Whether to display the axis inverted
		 */

		AxisController(float xmin = -1, float xmax = 1, float ymin = -1, float ymax = 1, bool autoX = false, bool autoY = false,
					   bool invx = false, bool invy = false);

		virtual ~AxisController();

		/**
		 * Input data from a gyroscope
		 * @param radAzimuth Azimuth in radians
		 * @param radPitch Pitch in radians
		 */

		void inputGyroscope(float radAzimuth, float radPitch);

		/**
		 * Inputs normalized data
		 * @param xAxis X Axis -1...1
		 * @param yAxis Y axis -1...1
		 */
		void inputNormalized(float xAxis, float yAxis);

		/**
		 * Inputs incremental data
		 * @param xdelta x delta
		 * @param ydelta y delta
		 */

		void inputIncremental(float xdelta, float ydelta);

		/**
		 * Draws the axis on a canvas
		 * @param canvas A Canvas
		 * @param color Color
		 */

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


	/**
	 * A generic axis controller (AxisController is abstract). This class is a singleton that
	 * you can ::enable, add to the engine, then use it across the app
	 */

	class GenericAxisController : public AxisController {

		static GenericAxisController *pInstance;

	public:
		GenericAxisController(float xmin = -1, float ymin = -1, float xmax = -1, float ymax = -1, bool autoX = true, bool autoY = true,
							  bool xinv = false, bool yinv = false);

		static void
		enable(float xmin = -1, float ymin = -1, float xmax = -1, float ymax = -1, bool autoX = true, bool autoY = true, bool xinv = false,
			   bool yinv = false);

		static void disable();

		static GenericAxisController *instance();

		inline void poll() {}
	};

	inline GenericAxisController *GenericAxisController::instance() { return pInstance; }

	inline void
	GenericAxisController::enable(float xmin, float xmax, float ymin, float ymax, bool autoX, bool autoY, bool xinv, bool yinv) {
		if (pInstance == nullptr)
			pInstance = new GenericAxisController(xmin, xmax, ymin, ymax, autoX, autoY, xinv, yinv);
	}

	inline void GenericAxisController::disable() {
		if (pInstance != nullptr) {
			delete pInstance;
			pInstance = nullptr;
		}
	}


}

#pragma clang diagnostic pop

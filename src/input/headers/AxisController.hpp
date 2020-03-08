//
// Created by rodo on 2020-02-10.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#pragma once

#include "Fu.hpp"
#include "Canvas2D.hpp"

namespace Pix {

	typedef struct sAxisControllerConfig {
		const float XMIN=-1, XMAX=1, YMIN=-1, YMAX=1;
		const float AUTOX=0.9F, AUTOY=0.8F;
		const bool INVX=false, INVY=true;
	} AxisControllerConfig_t;

	class AxisController : public InputDevice {

		inline static const std::string TAG="AxisController";
		
		const AxisControllerConfig_t CONFIG;
		
		float fAxisX, fAxisY, fNextAxisX, fNextAxisY;    // raw
		float fCurrentX, fCurrentY;                      // interpolated
		int nInputCounterX = 0;
		int nInputCounterY = 0;

	
	public:

		/**
		 * Constructs the Axis Controller
		 * @param config Configurator
		 */

		AxisController(AxisControllerConfig_t config={});

		virtual ~AxisController();

		void init(Fu *engine);

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

	inline void AxisController::init(Fu *engine) {}

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

		GenericAxisController(AxisControllerConfig_t config = {});

		static void enable(AxisControllerConfig_t config = {});

		static void disable();

		static GenericAxisController *instance();

		inline void poll() {}
	};

	inline GenericAxisController *GenericAxisController::instance() { return pInstance; }

	inline void
	GenericAxisController::enable(AxisControllerConfig_t config) {
		if (pInstance == nullptr)
			pInstance = new GenericAxisController(config);
	}

	inline void GenericAxisController::disable() {
		if (pInstance != nullptr) {
			delete pInstance;
			pInstance = nullptr;
		}
	}


}

#pragma clang diagnostic pop

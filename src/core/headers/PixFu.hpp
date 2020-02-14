//
//  PixFu.hpp
//  PixFu
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma ide diagnostic ignored "OCSimplifyInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef PixEngine_hpp
#define PixEngine_hpp

#include "OpenGL.h"
#include "Surface.hpp"
#include "Utils.hpp"
#include <string>
#include <vector>

namespace rgl {

	class PixFu;

/*-------------------------------------------------------------------*/

/**
 * a PixEngine extension. Extensions are added with PixEngine::addExtension.
 */

	class PixFuExtension {

	public:
		virtual ~PixFuExtension();

		virtual bool init(PixFu *engine) = 0;

		virtual void tick(PixFu *engine, float fElapsedTime) = 0;
	};

	inline PixFuExtension::~PixFuExtension() {}

/*-------------------------------------------------------------------*/

/**
 * The platform-dependent part of PixEngine
 */

	class PixFuPlatform {

		static const std::string TAG;
		static PixFuPlatform *spCurrentInstance;

	protected:

		// App CWD
		static std::string ROOTPATH;

	public:

		virtual ~PixFuPlatform();

		/** initializes the platform */
		virtual bool init(rgl::PixFu *engine) = 0;

		/** Process events and return <isLoopRunning,...> */
		virtual std::pair<bool, bool> events() = 0;

		/** Commit current frame */
		virtual void commit() = 0;

		/** Deinit platform */
		virtual void deinit() = 0;

		/** FPS info */
		virtual void onFps(int fps);

		/** Initializes the platform */
		static void init(PixFuPlatform *platform);

		/** Gets platform instance */
		static PixFuPlatform *instance();

		/** Get file path */
		static std::string getPath(std::string relpath);

		/** Set root path */
		static void setPath(std::string rootpath);

	};

	inline PixFuPlatform::~PixFuPlatform() {}

	inline std::string PixFuPlatform::getPath(std::string relpath) {
		return ROOTPATH + relpath;
	}

	inline void PixFuPlatform::setPath(std::string abspath) {
		if (DBG) LogV(TAG, SF("Local Path Root is %s", abspath.c_str()));
		ROOTPATH = abspath;
	}

	inline void PixFuPlatform::onFps(int fps) {}

/*-------------------------------------------------------------------*/

/**
 * An input device. Input devices are added with PixEngine::addInputDevice
 */

	class InputDevice {
	public:
		virtual ~InputDevice();

		// poll device values if makes sense. called by the engine loop.
		virtual void poll() = 0;

		// snapshot current values, these values will be used during a frame loop.
		virtual void sync(float fElapsedTime) = 0;
	};

	inline InputDevice::~InputDevice() {}

/*-------------------------------------------------------------------*/

	class PixFu {

		static const std::string TAG;

		// RendererPix belongs to the Android Launcher interface, we will
		// be calling the parts of the loop directly, but don't want to export
		// those functions as it doesn't make sense for the rest of platforms

		friend class RendererPix;

		const std::string SHADERNAME;                       // shader filename

		PixFuPlatform *pPlatform = nullptr;                 // platform layer

		Surface *pSurface = nullptr;                        // primary surface
		std::vector<PixFuExtension *> vExtensions;          // extensions
		std::vector<InputDevice *> vInputDevices;           // input devices

		bool bLoopActive = false;                           // whether loop is active
		bool bIsFocused = false;                            // whether app is focused
		bool bInited = false;                               // whether app has been already inited

		int nFrameCount = 0;                                // fps counter
		float fFrameTimer = 1.0f;                           // frame timer

		int nScreenWidth = 0, nScreenHeight = 0;            // screen dimensions

		// loop control

		/** Runs the loop synchronously if applies */
		void loop();

		/** Async loop: init */
		bool loop_init();

		/** async loop: tick */
		bool loop_tick(float fElapsedTime);

		/** async loop: deinit */
		void loop_deinit();

		/** reinit stopped loop */
		bool loop_reinit(int newWidth, int newHeight);

	protected:

		/**
		 * Adds an extension to the engine. Added extensions are integrated into the loop
		 * and can paint in OpenGL.
		 * @param extension The instantiated extension to add
		 */
		void addExtension(PixFuExtension *extension);

		/**
		 * Adds an input device
		 * @param inputDevice The instantiated input device (Mouse, Keyboard, AxisController ...)
		 */

		void addInputDevice(InputDevice *inputDevice);

		/**
		 * Gets the memory-backing buffer
		 * @return The buffer you can write to
		 */
		Drawable *buffer();

	public:

		const std::string APPNAME;							// application name

		PixFu(const std::string &appname="pixFuApp", const std::string &shader = "default");

		~PixFu();

		/**
		 * Initializes the engine
		 * @param width  Screen Widtg
		 * @param height Screen Height
		 * @return success
		 */
		bool init(int width, int height);

		/**
		 * Runs the loop if this platform uses it. This function would then be blocking.
		 */
		void start();

		/**
		 * Get screen width
		 * @return screen width in pixels
		 */
		int screenWidth();

		/**
		 * Get screen height
		 * @return screen height in pixels
		 */

		int screenHeight();                                  // get screen height

		virtual bool onUserCreate();

		virtual bool onUserUpdate(float fElapsedTime);

		virtual bool onUserDestroy();

	};

	inline int PixFu::screenWidth() { return nScreenWidth; }

	inline int PixFu::screenHeight() { return nScreenHeight; }

	inline void PixFu::addExtension(PixFuExtension *e) { vExtensions.push_back(e); }

	inline void PixFu::addInputDevice(InputDevice *inputDevice) {
		vInputDevices.push_back(inputDevice);
	}

	inline Drawable *PixFu::buffer() { return pSurface->buffer(); }

}
#endif /* PixEngine_hpp*/


#pragma clang diagnostic pop
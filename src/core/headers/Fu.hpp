//
//  PixFu.hpp
//  PixFu
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//
#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma ide diagnostic ignored "OCSimplifyInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "Utils.hpp"
#include "FuExtension.hpp"
#include "Surface.hpp"

#include <string>
#include <vector>

namespace Pix {

	class Fu;
	class Canvas2D;
	class Shader;

	/**
	 * The platform-dependent part of PixEngine
	 */

	class FuPlatform {

		static const std::string TAG;
		static FuPlatform *spCurrentInstance;

	protected:

		// App CWD
		static std::string ROOTPATH;

	public:

		virtual ~FuPlatform();

		/**
		 * Initializes the platform window to run the provided engine
		 */
		virtual bool init(Pix::Fu *engine) = 0;

		/**
		 * Process any platform events, and return if the loop should be running and if the
		 * window has focus.
		 */
		virtual std::pair<bool, bool> events() = 0;

		/**
		 * Commit current frame. Typically to swap buffers or call related function. It is called
		 * at the end of each frame.
		 */
		virtual void commit() = 0;

		/**
		 * Deinit the platform would close the window and the application resources.
		 */
		virtual void deinit() = 0;

		/**
		 * Called every second more or less, can be used to show stats,
		 * or in this case to handle window resizes. 
		 */

		virtual void onFps(Fu *engine, int fps);

		/* ------------ Static Platform Functions */

		/** Initializes the platform */
		static void init(FuPlatform *platform);

		/** Gets platform instance */
		static FuPlatform *instance();

		/** Get file path */
		static std::string getPath(std::string relpath);

		/** Set root path */
		static void setPath(std::string rootpath);

	};

	inline FuPlatform::~FuPlatform() {}

	inline std::string FuPlatform::getPath(std::string relpath) {
		return ROOTPATH + relpath;
	}

	inline void FuPlatform::setPath(std::string abspath) {
		if (DBG) LogV(TAG, SF("Local Path Root is %s", abspath.c_str()));
		ROOTPATH = abspath;
	}

	inline void FuPlatform::onFps(Fu *engine, int fps) {}

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

	typedef struct sFuConfig {
		const FontInfo_t fontInfo = {};
		const std::string shaderName = "default";
	} FuConfig_t;

	class Fu {

		static const std::string TAG;

		// RendererPix belongs to the Android Launcher interface, we will
		// be calling the parts of the loop directly, but don't want to export
		// those functions as it doesn't make sense for the rest of platforms

		friend class RendererPix;

		friend class PixFuPlatformApple;

		const FuConfig_t CONFIG;                       // shader filename

		FuPlatform *pPlatform = nullptr;                 // platform layer

		Surface *pSurface = nullptr;                        // primary surface
		std::vector<FuExtension *> vExtensions;          // extensions
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
		bool loop_init(bool reinit = false);

		/** async loop: tick */
		bool loop_tick(float fElapsedTime);

		/** async loop: deinit */
		void loop_deinit();

		/** reinit stopped loop */
		bool loop_reinit(int newWidth, int newHeight);

	protected:


		/**
		 * Gets the memory-backing buffer
		 * @return The buffer you can write to
		 */
		Drawable *buffer();


	public:

		const std::string APPNAME;                            // application name

		Fu(const std::string appname = "pixFuApp", FuConfig_t config = {});

		virtual ~Fu();

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

		virtual bool onUserCreate(bool restarted);

		virtual bool onUserUpdate(float fElapsedTime);

		virtual bool onUserDestroy();

		Shader *shader();
		Canvas2D *canvas();

		/**
		 * Adds an extension to the engine. Added extensions are integrated into the loop
		 * and can paint in OpenGL.
		 * @param extension The instantiated extension to add
		 */
		void addExtension(FuExtension *extension);

		bool removeExtension(FuExtension *extension);

		/**
		 * Adds an input device
		 * @param inputDevice The instantiated input device (Mouse, Keyboard, AxisController ...)
		 */

		void addInputDevice(InputDevice *inputDevice);

	};

	inline int Fu::screenWidth() { return nScreenWidth; }

	inline int Fu::screenHeight() { return nScreenHeight; }

	inline void Fu::addExtension(FuExtension *e) { vExtensions.push_back(e); }

	inline void Fu::addInputDevice(InputDevice *inputDevice) {
		vInputDevices.push_back(inputDevice);
	}

	inline Drawable *Fu::buffer() { return pSurface->buffer(); }

	inline Canvas2D *Fu::canvas() { return pSurface->canvas(); }

	inline Shader *Fu::shader() { return pSurface->shader(); }

}

#pragma clang diagnostic pop

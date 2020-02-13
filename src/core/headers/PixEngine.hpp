//
//  PixEngine.hpp
//  PixEngine
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef PixEngine_hpp
#define PixEngine_hpp

#include "OpenGL.h"
#include "Surface.hpp"
#include "Utils.hpp"
#include <string>
#include <vector>

namespace rgl {

	class PixEngine;

	/**
	 * a PixEngine extension. Extensions are added with PixEngine::addExtension.
	 */

	class PixEngineExtension {

	public:
		virtual ~PixEngineExtension();

		virtual bool init(PixEngine *engine) = 0;

		virtual void tick(PixEngine *engine, float fElapsedTime) = 0;
	};

	inline PixEngineExtension::~PixEngineExtension() {}

	/**
	 * The platform-dependent part of PixEngine
	 */
	class PixEnginePlatform {
		static const std::string TAG;

	protected:
		PixEngine *pEngine;

		// App CWD
		static std::string ROOTPATH;

	public:

		virtual ~PixEnginePlatform();

		void setEngine(PixEngine *engine);

		virtual bool init() = 0;

		/** Process events and return <isLoopRunning,...> */
		virtual std::pair<bool, bool> events() = 0;

		/** Commit current frame */
		virtual void commit() = 0;

		/** Deinit platform */
		virtual void deinit() = 0;

		/** FPS info */
		virtual void onFps(int fps);

		/** Get file path */
		static std::string getPath(std::string relpath);

		/** Set root path */
		static void setPath(std::string rootpath);

	};

	inline PixEnginePlatform::~PixEnginePlatform() {}

	inline void PixEnginePlatform::setEngine(PixEngine *engine) { pEngine = engine; }

	inline std::string PixEnginePlatform::getPath(std::string relpath) {
		return ROOTPATH + relpath;
	}

	inline void PixEnginePlatform::setPath(std::string abspath) {
		if (DBG) LogV(TAG, SF("Local Path Root is %s", abspath.c_str()));
		ROOTPATH = abspath;
	}

	inline void PixEnginePlatform::onFps(int fps) {}

	/**
	 * An input device. Input devices are added with PixEngine::addInputDevice
	 */

	class InputDevice {
	public:
		virtual ~InputDevice();

		// poll device values if makes sense. called by the engine loop.
		virtual void poll() = 0;

		// snapshot current values, these values will be used during a frame loop.
		virtual void sync() = 0;
	};

	inline InputDevice::~InputDevice() {}

	class PixEngine {

		static const std::string TAG;

		friend class RendererPix;

		const std::string sShaderName;                        // shader filename

		PixEnginePlatform *pPlatform;                        // platform layer

		Surface *pSurface;                                    // primary surface
		std::vector<PixEngineExtension *> vExtensions;        // extensions
		std::vector<InputDevice *> vInputDevices;            // input devices

		bool bLoopActive = false;                            // whether loop is active
		bool bIsFocused = false;                            // whether app is focused
		bool bInited = false;                                // whether app has been already inited

		int nFrameCount = 0;                                // fps counter
		float fFrameTimer = 1.0f;                            // frame timer

		int nScreenWidth, nScreenHeight;                    // screen dimensions

		// loop control

		void loop();                                        // runs the loop synchronously

		bool loop_init();                                    // async loop: init

		bool loop_tick(float fElapsedTime);                    // async loop: tick

		void loop_deinit();                                    // async loop: deinit

		bool loop_reinit(int newWidth, int newHeight);        // reinit stopped loop

	protected:

		void addExtension(PixEngineExtension *extension);    // adds extension to the engine

		void addInputDevice(InputDevice *inputDevice);        // adds input devide to the engine

		Drawable *buffer();                                    // get primary surface

	public:

		PixEngine(PixEnginePlatform *platform, std::string shader = "default");

		~PixEngine();

		bool init(int width, int height);                    // initializes the engine

		void
		start();                                        // runs the loop if this platforms uses it

		int screenWidth();                                    // get screen width

		int screenHeight();                                    // get screen height

		virtual bool onUserCreate();

		virtual bool onUserUpdate(float fElapsedTime);

		virtual bool onUserDestroy();

	};


	inline int PixEngine::screenWidth() { return nScreenWidth; }

	inline int PixEngine::screenHeight() { return nScreenHeight; }

	inline void PixEngine::addExtension(PixEngineExtension *e) { vExtensions.push_back(e); }

	inline void PixEngine::addInputDevice(InputDevice *inputDevice) {
		vInputDevices.push_back(inputDevice);
	}

	inline Drawable *PixEngine::buffer() { return pSurface->buffer(); }

}
#endif /* PixEngine_hpp*/


//
//  PixFu.cpp
//  PixFu
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "Fu.hpp"
#include "Mouse.hpp"
#include "Utils.hpp"
#include "Surface.hpp"
#include "Keyboard.hpp"
#include "OpenGlUtils.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "UnusedValue"
#pragma ide diagnostic ignored "OCSimplifyInspection"

namespace Pix {
/*-------------------------------------------------------------------*/

	const std::string FuPlatform::TAG = "PixFuPlaf";

// current platform
	FuPlatform *FuPlatform::spCurrentInstance = nullptr;

// platform cwd
	std::string FuPlatform::ROOTPATH;

/** Get current platform */
	FuPlatform *FuPlatform::instance() {

		if (spCurrentInstance == nullptr)
			throw std::runtime_error("No platform initialized.");

		return spCurrentInstance;

	}

/** static: Init the platform */

	void FuPlatform::init(FuPlatform *platform) {

		if (spCurrentInstance == nullptr) {
			if (DBG) LogV(TAG, "Setting current platform");
			spCurrentInstance = platform;
		} else throw std::runtime_error("Already have a platform set");

	}

/*-------------------------------------------------------------------*/

	const bool SURFACE = true;
	const std::string Fu::TAG = "PixFu";
	float Fu::METRONOME = 0;

	Fu::Fu(const std::string appName, FuConfig_t configuration)
			: CONFIG(std::move(configuration)),
			  pPlatform(nullptr),
			  nScreenWidth(0),
			  nScreenHeight(0),
			  APPNAME(std::move(appName)) {}

	Fu::~Fu() {

		LogV(TAG, SF("Destruct application %s", APPNAME.c_str()));

		// destroy primary surface

		if (pSurface != nullptr) {
			delete pSurface;
			pSurface = nullptr;
		}

		// clear but do not destroy input devices
		// as we have not created them
		vInputDevices.clear();

		// clear but do not destroy extensions
		// as we have not created them
		vExtensions.clear();

	}

	bool Fu::removeExtension(FuExtension *e) {
		int i = 0;
		for (FuExtension *ex : vExtensions) {
			if (ex == e) {
				vExtensions.erase(vExtensions.begin() + i);
				return true;
			}
		}
		return false;
	}

	/** initializes the engine */
	bool Fu::init(int width, int height) {

		pPlatform = FuPlatform::instance();

		nScreenWidth = width;
		nScreenHeight = height;

		if (DBG)
			LogV(TAG,
				 SF("Application %s init, wh=%d,%d, already inited %d", APPNAME.c_str(), width,
					height, bInited));

		if (!bInited) {

			bInited = pPlatform->init(this);

			if (Mouse::instance() != nullptr)
				addInputDevice(Mouse::instance());

			if (Keyboard::instance() != nullptr)
				addInputDevice(Keyboard::instance());

		}

		return bInited;
	}

	/** sunchronously run the loop */
	void Fu::loop() {

		if (!loop_init()) {
			if (DBG) LogE(TAG, "Error in egine init");
			return;
		}

		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = tp1;

		while (bLoopActive) {

			// Run as fast as possible
			while (bLoopActive) {

				// Handle Timing
				tp2 = std::chrono::system_clock::now();
				std::chrono::duration<float> elapsedTime = tp2 - tp1;
				tp1 = tp2;

				// Our time per frame coefficient
				float fElapsedTime = elapsedTime.count();

				bLoopActive = loop_tick(fElapsedTime);

				fFrameTimer += fElapsedTime;
				nFrameCount++;

				if (fFrameTimer >= 1.0F) {
					fFrameTimer -= 1.0F;
					pPlatform->onFps(this, nFrameCount);
					nFrameCount = 0;
				}
			}

			// Allow the user to free resources if they have overrided the destroy function
			bLoopActive = !onUserDestroy();
		}

		loop_deinit();

	}

	/** loop part: initialization */
	bool Fu::loop_init(bool reinit) {

		bLoopActive = true;

		if (SURFACE) {
			pSurface = new Surface(nScreenWidth, nScreenHeight, CONFIG.fontInfo, CONFIG.shaderName, "glbuffer", vExtensions.size() != 0);
			addExtension(pSurface);
		}

		// Create user resources as part of this thread
		if (!onUserCreate(reinit))
			return false;

		if (DBG) LogV(TAG, SF("Calling userCreate, reinit %d", reinit));
		if (!reinit) {
			if (DBG) LogV(TAG, "Initing Extensions");

			// initialize PixFu Extensions
			for (FuExtension *extension : vExtensions)
				if (!extension->init(this)) return false;

		}

		if (DBG) LogV(TAG, "Loop Inited.");
		return true;
	}

	/** loop part: tick */
	bool Fu::loop_tick(float fElapsedTime) {

		METRONOME+=fElapsedTime;

		// todo
		std::pair<bool, bool> status = pPlatform->events();

		bLoopActive = status.first;
		bIsFocused = status.second;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (bLoopActive) {

			// snapshot inputdevices values
			for (InputDevice *device:vInputDevices)
				device->sync(fElapsedTime);

			// Handle Frame Update
			bLoopActive = onUserUpdate(fElapsedTime);

			// update PixFu Extensions
			for (FuExtension *extension : vExtensions)
				extension->tick(this, fElapsedTime);

			// swap frames
			pPlatform->commit();

			// update inputDevices values
			for (InputDevice *device:vInputDevices)
				device->poll();
		}

		return bLoopActive;
	}

	/** loop part: reinitialize */
	bool Fu::loop_reinit(int newWidth, int newHeight) {

		if (DBG) LogV(TAG, SF("Loop Reinit: Screen size changed to %d,%d", newWidth, newHeight));
		loop_deinit();
		glViewport(0, 0, newWidth, newHeight);
		init(newWidth, newHeight);
		return loop_init(true);

	}

	/** loop part: deinitialize */
	void Fu::loop_deinit() {

		if (DBG) LogV(TAG, "Loop deinit");
		bLoopActive = false;
		if (pSurface != nullptr) {
			delete pSurface;
			pSurface = nullptr;
		}
		//	pPlatform->deinit(); to destructor
	}

	/** user method: create */
	bool Fu::onUserCreate(bool restarted) {
		return true;
	}

	/** user method: update */
	bool Fu::onUserUpdate(float fElapsedTime) {
		return true;
	}

	/** user method: destroy */
	bool Fu::onUserDestroy() {
		return true;
	}
}

#pragma clang diagnostic pop

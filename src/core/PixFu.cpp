//
//  PixFu.cpp
//  PixFu
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "PixFu.hpp"
#include "OpenGL.h"
#include "OpenGlUtils.h"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "Utils.hpp"
#include "SpriteSheets.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
#pragma ide diagnostic ignored "cert-err58-cpp"
#pragma ide diagnostic ignored "OCSimplifyInspection"

using namespace rgl;

/*-------------------------------------------------------------------*/

const std::string PixFuPlatform::TAG = "PixFuPlaf";

// current platform
PixFuPlatform *PixFuPlatform::spCurrentInstance = nullptr;

// platform cwd
std::string PixFuPlatform::ROOTPATH;

/** Get current platform */
PixFuPlatform *PixFuPlatform::instance() {

	if (spCurrentInstance == nullptr)
		throw std::runtime_error("No platform initialized.");

	return spCurrentInstance;

}

/** static: Init the platform */

void PixFuPlatform::init(PixFuPlatform *platform) {

	if (spCurrentInstance == nullptr) {
		if (DBG) LogV(TAG, "Setting current platform");
		spCurrentInstance = platform;
	} else throw std::runtime_error("Already have a platform set");

}

/*-------------------------------------------------------------------*/

const std::string PixFu::TAG = "PixFu";

PixFu::PixFu(const std::string appName, const std::string shader)
		: SHADERNAME(std::move(shader)),
		  APPNAME(std::move(appName)),
		  pPlatform(nullptr),
		  nScreenWidth(0),
		  nScreenHeight(0) {}

PixFu::~PixFu() {

	LogV(TAG, SF("Destruct application %s", APPNAME.c_str()));

	// destroy primary surface
	if (pSurface != nullptr) {
		delete pSurface;
		pSurface = nullptr;
	}

	// destroy input devices

	// TODO inputdevices are singletons
	// TODO destruct them o program exit
	//for (InputDevice *device:vInputDevices)
	//	delete device;

	vInputDevices.clear();

	// destroy all extensions

	//for (PixFuExtension *extension:vExtensions)
	//	delete extension;

	vExtensions.clear();

	// TODO might not be neccessary in situations
	// pPlatform->deinit();
	// delete pPlatform;

	SpriteSheets::unload();

}

bool PixFu::removeExtension(PixFuExtension *e) {
	int i = 0;
	for (PixFuExtension *ex : vExtensions) {
		if (ex == e) {
			vExtensions.erase(vExtensions.begin() + i);
			return true;
		}
	}
	return false;
}

/** initializes the engine */
bool PixFu::init(int width, int height) {

	pPlatform = PixFuPlatform::instance();

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
void PixFu::loop() {

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

const bool SURFACE = true;

/** loop part: initialization */
bool PixFu::loop_init(bool reinit) {

	if (SURFACE) {
		pSurface = new Surface(nScreenWidth, nScreenHeight, SHADERNAME, "glbuffer");
		addExtension(pSurface);
	}

	if (DBG) LogV(TAG, SF("Calling userCreate, reinit %d", reinit));

	bLoopActive = true;

	// Create user resources as part of this thread
	if (!onUserCreate(reinit))
		return false;

	if (!reinit) {
		if (DBG) LogV(TAG, "Initing Extensions");

		// initialize PixFu Extensions
		for (PixFuExtension *extension : vExtensions)
			if (!extension->init(this)) return false;

	}

	if (DBG) LogV(TAG, "Loop Inited.");
	return true;
}

/** loop part: tick */
bool PixFu::loop_tick(float fElapsedTime) {

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
		for (PixFuExtension *extension : vExtensions)
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
bool PixFu::loop_reinit(int newWidth, int newHeight) {

	if (DBG) LogV(TAG, SF("Loop Reinit: Screen size changed to %d,%d", newWidth, newHeight));
	loop_deinit();
	glViewport(0, 0, newWidth, newHeight);
	init(newWidth, newHeight);
	return loop_init(true);

}

/** loop part: deinitialize */
void PixFu::loop_deinit() {

	if (DBG) LogV(TAG, "Loop deinit");
	bLoopActive = false;
	if (pSurface != nullptr) {
		delete pSurface;
		pSurface = nullptr;
	}
	//	pPlatform->deinit(); to destructor
}

/** user method: create */
bool PixFu::onUserCreate(bool restarted) {
	return true;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

/** user method: update */
bool PixFu::onUserUpdate(float fElapsedTime) {
	return true;
}

#pragma clang diagnostic pop

/** user method: destroy */
bool PixFu::onUserDestroy() {
	return true;
}

#pragma clang diagnostic pop

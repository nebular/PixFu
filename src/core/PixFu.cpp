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

using namespace rgl;

const std::string PixFuPlatform::TAG = "PixFuPlaf";

PixFuPlatform *PixFuPlatform::spCurrentInstance = nullptr;
std::string PixFuPlatform::ROOTPATH = "";

PixFuPlatform *PixFuPlatform::instance() {

	if (spCurrentInstance == nullptr)
		throw "No platform initialized.";

	return spCurrentInstance;

}

void PixFuPlatform::init(PixFuPlatform *platform) {

	if (DBG) LogV(TAG, "Initing Platform");

	if (spCurrentInstance == nullptr)
		spCurrentInstance = platform;

	else throw "Already have a platform set";

}

PixFu::PixFu(std::string shader) : sShaderName(shader) {}

const std::string PixFu::TAG = "PixFu";

PixFu::~PixFu() {

	// destroy primary surface

	delete pSurface;
	pSurface = nullptr;

	// destroy input devices

	for (InputDevice *device:vInputDevices)
		delete device;

	vInputDevices.clear();

	// destroy all extensions

	for (PixFuExtension *extension:vExtensions)
		delete extension;

	vExtensions.clear();

	pPlatform->deinit();
	delete pPlatform;

}

bool PixFu::init(int width, int height) {

	pPlatform = PixFuPlatform::instance();

	nScreenWidth = width;
	nScreenHeight = height;

	if (DBG) LogV(TAG, SF("Init %d, %d, already %d", width, height, bInited));

	if (!bInited) {

		bInited = pPlatform->init(this);

		if (Mouse::instance() != nullptr)
			addInputDevice(Mouse::instance());

		if (Keyboard::instance() != nullptr)
			addInputDevice(Keyboard::instance());

	}

	return bInited;
}

void PixFu::loop() {

	if (!loop_init()) {
		if (DBG) LogE(TAG, "Error in egine init");
		return;
	}

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	bLoopActive = true;

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

			if (fFrameTimer >= 1.0f) {
				fFrameTimer -= 1.0f;
				pPlatform->onFps(nFrameCount);
				nFrameCount = 0;
			}
		}

		// Allow the user to free resources if they have overrided the destroy function
		bLoopActive = !onUserDestroy();
	}

	loop_deinit();

}

bool PixFu::loop_init() {

	pSurface = new Surface(nScreenWidth, nScreenHeight, "glbuffer", sShaderName);
	pSurface->init();

	if (DBG) LogV(TAG, "Calling userCreate()");

	// Create user resources as part of this thread
	if (!onUserCreate())
		return false;

	if (DBG) LogV(TAG, "Initing Extensions");

	// initialize PixFu Extensions
	for (PixFuExtension *extension : vExtensions)
		if (!extension->init(this)) return false;

	return true;
}

bool PixFu::loop_tick(float fElapsedTime) {

	// todo
	std::pair<bool, bool> status = pPlatform->events();

	bLoopActive = status.first;
	bIsFocused = status.second;

	glClear(GL_COLOR_BUFFER_BIT);

	if (bLoopActive) {

		// snapshot inputdevices values
		for (InputDevice *device:vInputDevices)
			device->sync();

		// Handle Frame Update
		bLoopActive = onUserUpdate(fElapsedTime);

		pSurface->tick();

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

// TODO !!!
bool PixFu::loop_reinit(int newWidth, int newHeight) {

	// so even screen dimensions might change on Android
	if (screenWidth() != newWidth || screenHeight() != newHeight) {
		if (DBG) LogV(TAG, SF("Reiniting loop, screen size changedto %d,%d", newWidth, newHeight));
		loop_deinit();
		init(newWidth, newHeight);
		return loop_init();
	}

	if (pSurface == nullptr)
		return loop_init();

	// continue opengl init
	return true;

}

void PixFu::loop_deinit() {

	if (DBG) LogV(TAG, "Loop deinit");
	bLoopActive = false;
	if (pSurface != nullptr) {
		pSurface->deinit();
		delete pSurface;
		pSurface = nullptr;
	}
//	pPlatform->deinit(); to destructor
}

bool PixFu::onUserCreate() {
	return true;
}

bool PixFu::onUserUpdate(float fElapsedTime) {
	return true;
}

bool PixFu::onUserDestroy() {
	return true;
}

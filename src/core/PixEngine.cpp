//
//  PixEngine.cpp
//  PixEngine
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "PixEngine.hpp"
#include "OpenGL.h"
#include "OpenGlUtils.h"
#include "Mouse.hpp"
#include "Keyboard.hpp"

using namespace rgl;

std::string PixEnginePlatform::ROOTPATH = "";

PixEngine::PixEngine(PixEnginePlatform *platform, std::string shader) : sShaderName(shader) {
	pPlatform = platform;
	pPlatform->setEngine(this);
}

PixEngine::~PixEngine() {

	// destroy primary surface

	delete pSurface;
	pSurface = nullptr;

	// destroy input devices

	for (InputDevice *device:vInputDevices)
		delete device;

	vInputDevices.clear();

	// destroy all extensions

	for (PixEngineExtension *extension:vExtensions)
		delete extension;

	vExtensions.clear();

	pPlatform->deinit();
	delete pPlatform;

}

bool PixEngine::init(int width, int height) {

	nScreenWidth = width;
	nScreenHeight = height;

	if (!bInited) {

		bInited = pPlatform->init();

		if (Mouse::instance() != nullptr)
			addInputDevice(Mouse::instance());

		if (Keyboard::instance() != nullptr)
			addInputDevice(Keyboard::instance());

	}

	return bInited;
}

void PixEngine::loop() {

	if (!loop_init()) {
		// error
		std::cerr << "Error: engine init" << std::endl;
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

bool PixEngine::loop_init() {

	pSurface = new Surface(nScreenWidth, nScreenHeight, sShaderName);
	pSurface->init_opengl();
	pSurface->init_texture();

	// Create user resources as part of this thread
	if (!onUserCreate())
		return false;

	// initialize PixEngine Extensions
	for (PixEngineExtension *extension : vExtensions)
		if (!extension->onUserCreate(this)) return false;

	return true;
}

bool PixEngine::loop_tick(float fElapsedTime) {

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

		// update PixEngine Extensions
		for (PixEngineExtension *extension : vExtensions)
			extension->onUserUpdate(this, fElapsedTime);

		// swap frames
		pPlatform->commit();

		// update inputDevices values
		for (InputDevice *device:vInputDevices)
			device->update();

	}

	return bLoopActive;
}

// TODO !!!
bool PixEngine::loop_reinit(int newWidth, int newHeight) {

	// so even screen dimensions might change on Android
	if (screenWidth() != newWidth || screenHeight() != newHeight) {
		loop_deinit();
		init(newWidth, newHeight);
		return loop_init();
	}

	if (pSurface == nullptr)
		return loop_init();

	// continue opengl init
	return true;

}

void PixEngine::loop_deinit() {
	bLoopActive = false;
	pSurface->deinit();
	delete pSurface;
	pSurface = nullptr;
//	pPlatform->deinit(); to destructor
}

bool PixEngine::onUserCreate() {
	return true;
}

bool PixEngine::onUserUpdate(float fElapsedTime) {
	return true;
}

bool PixEngine::onUserDestroy() {
	return true;
}

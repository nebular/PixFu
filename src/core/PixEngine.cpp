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

using namespace rgl;

std::string PixEnginePlatform::ROOTPATH = "";

PixEngine::PixEngine(int width, int height, PixEnginePlatform *platform, std::string shader)
: WIDTH(width), HEIGHT(height), sShaderName(shader) {
	init(platform);
}

bool PixEngine::init(PixEnginePlatform *platform) {
	
	pPlatform = platform;
	pPlatform->setEngine(this);
	
	pMouse = new Mouse(2);
	pKeyboard = new Keyboard(67);
	return pPlatform->init();
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
			
			// Update Title Bar
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
	
	pSurface = new Surface(WIDTH, HEIGHT, sShaderName);
	pSurface->init_opengl();
	pSurface->init_texture();
	
	// Create user resources as part of this thread
	if (!onUserCreate())
		return false;
		
	for (PixEngineExtension *extension : vExtensions)
		if (!extension->onUserCreate(this)) return false;

	return true;
}

bool PixEngine::loop_tick(float fElapsedTime) {
	
	// todo
	std::pair<bool,bool> status = pPlatform->events();

	bLoopActive = status.first;
	bIsFocused = status.second;

	glClear(GL_COLOR_BUFFER_BIT);
	
	if (bLoopActive) {
		
		pKeyboard->sync();
		pMouse->sync();
		
		// Handle Frame Update
		bLoopActive = onUserUpdate(fElapsedTime);
		
		pSurface->tick();
		
		for (PixEngineExtension *extension : vExtensions)
			extension->onUserUpdate(this, fElapsedTime);

		pPlatform->commit();

		pMouse->update();
		pKeyboard->update();

	}

	return bLoopActive;
}

bool PixEngine::loop_reinit(int newWidth, int newHeight) {
	/*
	 // so even screen dimensions might change on Android
	 if (screenWidth() != newWidth || screenHeight() != newHeight) {
	 if (pDefaultDrawTarget != nullptr) {
	 delete pDefaultDrawTarget;
	 pDefaultDrawTarget = nullptr;
	 Construct(newWidth, newHeight, 1, 1, false);
	 }
	 }
	 // continue opengl init
	 return thread_init();
	 */
	return false;
}

void PixEngine::loop_deinit() {
	bLoopActive = false;
	pSurface->deinit();
	pPlatform->deinit();
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

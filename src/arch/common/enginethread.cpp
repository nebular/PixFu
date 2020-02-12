#include "PixEngine.hpp"

//
// Created by rodo on 2020-01-24.
//
void PixEngine::EngineThread() {
	
	if (!olca_thread_init()) {
		// error
		std::cerr << "Error:  engine init" << std::endl;
		return;
	}
	
	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();
	
	
	while (bAtomActive) {
		
		// Run as fast as possible
		while (bAtomActive) {
			
			// Handle Timing
			tp2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = tp2 - tp1;
			tp1 = tp2;
			
			// Our time per frame coefficient
			float fElapsedTime = elapsedTime.count();
			
			bAtomActive = olca_thread_tick(fElapsedTime);
			
			// Update Title Bar
			fFrameTimer += fElapsedTime;
			nFrameCount++;
			
			if (fFrameTimer >= 1.0f) {
				
				fFrameTimer -= 1.0f;
				
				std::string sTitle =
				"OneLoneCoder.com - Pixel Game Engine - " + sAppName + " - FPS: " +
				std::to_string(nFrameCount);
				
#ifdef _WIN32
#ifdef UNICODE
				SetWindowText(olc_hWnd, ConvertS2W(sTitle).c_str());
#else
				SetWindowText(olc_hWnd, sTitle.c_str());
#endif
#elif __APPLE__
				set_window_name(sTitle.c_str());
#elif ANDROID
				std::cout << "TODO set window name" << std::endl;
#elif LINUX
				XStoreName(olc_Display, olc_Window, sTitle.c_str());
#endif
				
				nFrameCount = 0;
			}
		}
		
		// Allow the user to free resources if they have overrided the destroy function
		if (OnUserDestroy()) {
			// User has permitted destroy, so exit and clean up
		} else {
			// User denied destroy for some reason, so continue running
			bAtomActive = true;
		}
	}
	
	olca_thread_deinit();
	
}


// this function will be called on application resume
bool PixEngine::olca_thread_reinit(int newWidth, int newHeight) {
	// so even screen dimensions might change on Android
	if (ScreenWidth() != newWidth || ScreenHeight() != newHeight) {
		ALOGV("PGEReinit: Size changed");
		if (pDefaultDrawTarget != nullptr) {
			delete pDefaultDrawTarget;
			pDefaultDrawTarget = nullptr;
			Construct(newWidth, newHeight, 1, 1, false);
		}
	}
	// continue opengl init
	return olca_thread_init();
}

// this function also will be called on application resume
bool PixEngine::olca_thread_init() {
	
	ALOGV("PGEInit: Create Context");
	// Start OpenGL, the context is owned by the game thread
	olc_OpenGLCreate();
	
	glGenTextures(1, &glBuffer);
	glBindTexture(GL_TEXTURE_2D, glBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
#if !defined __APPLE__ && !defined ANDROID
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
#endif
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nScreenWidth, nScreenHeight, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, pDefaultDrawTarget->GetData());
	
	glUseProgram(shader);
	glUniform1i(glGetUniformLocation(shader, "glbuffer"), 0);
		
	ALOGV("PGEInit: Calling OnUserCreate");
	// Create user resources as part of this thread
	if (!OnUserCreate())
		return false;
	
	// ------------------
	// RLP: As derived classes can load textures in OnUserCreate, we
	// reselect primary texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, glBuffer);
	// ------------------
	
	return true;
}

bool PixEngine::olca_thread_tick(float fElapsedTime) {
	
	// todo
	arch_ProcessWindowEvents();
	glClear(GL_COLOR_BUFFER_BIT);

	pKeyboard->sync();
	pMouse->sync();

	// Handle Frame Update
	bAtomActive = OnUserUpdate(fElapsedTime);
	
//	glViewport(nViewX, nViewY, nViewW, nViewH);
	
	// TODO: This is a bit slow (especially in debug, but 100x faster in release mode???)
	// Copy pixel array into texture
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nScreenWidth, nScreenHeight, GL_RGBA, GL_UNSIGNED_BYTE,
					pDefaultDrawTarget->GetData());
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, glBuffer);
	
	glUseProgram(shader);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	arch_tick();
	
	return bAtomActive;
	
}


void PixEngine::olca_thread_deinit() {
	
	bAtomActive = false;
	arch_deinit();
	
	
}


}

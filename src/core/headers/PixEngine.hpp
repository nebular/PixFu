//
//  PixEngine.hpp
//  PixEngine
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef PixEngine_hpp
#define PixEngine_hpp

#include "BasicInput.hpp"
#include "OpenGL.h"
#include "Surface.hpp"
#include <string>
#include <vector>

namespace rgl {

class PixEngine;

class PixEngineExtension {
public:
	virtual bool onUserCreate(PixEngine *engine) = 0;
	virtual void onUserUpdate(PixEngine *engine, float fElapsedTime) = 0;
};

class PixEnginePlatform {


protected:
	PixEngine *pEngine;
	static std::string ROOTPATH;

public:
	void setEngine(PixEngine *engine);
	virtual bool init() = 0;
	virtual std::pair<bool,bool> events() = 0;
	virtual void commit() = 0;
	virtual void deinit() = 0;

	virtual void onFps(int fps);

	static std::string getPath(std::string relpath);

	
};
inline void PixEnginePlatform::setEngine(PixEngine *engine) { pEngine = engine; }
inline std::string PixEnginePlatform::getPath(std::string relpath) { return ROOTPATH + relpath; }
inline void PixEnginePlatform::onFps(int fps) {}

class PixEngine {
	
	const std::string sShaderName;
	
	PixEnginePlatform *pPlatform;

	Surface *pSurface;
	std::vector<PixEngineExtension *> vExtensions;
	
	bool bLoopActive = false;
	bool bIsFocused = false;
	
	int nFrameCount = 0;
	float fFrameTimer = 1.0f;

	bool init(PixEnginePlatform *platform);
	
	// loop control

	void loop();
	bool loop_init();
	bool loop_tick(float fElapsedTime);
	void loop_deinit();

	bool loop_reinit(int newWidth, int newHeight);

protected:

	void addExtension(PixEngineExtension *extension);
	Drawable *buffer();
	Mouse *pMouse;
	Keyboard *pKeyboard;

public:
	
	const int WIDTH, HEIGHT;

	PixEngine(int width, int height, PixEnginePlatform *platform, std::string shader = "default");
	~PixEngine();

	virtual bool onUserCreate();
	virtual bool onUserUpdate(float fElapsedTime);
	virtual bool onUserDestroy();
	
	void start();

};

inline void PixEngine::addExtension(PixEngineExtension *e) { vExtensions.push_back(e);}

inline Drawable *PixEngine::buffer() { return pSurface->buffer(); }

}
#endif /* PixEngine_hpp*/


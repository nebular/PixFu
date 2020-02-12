//
// Created by rodo on 2020-02-12.
//
#ifdef ANDROID

#ifndef PIXENGINE_ANDROID_ANDROIDLAYER_H
#define PIXENGINE_ANDROID_ANDROIDLAYER_H

#include "OpenGL.h"

class PixEngineAndroid : public rgl::PixEnginePlatform {


	bool init();

	std::pair<bool, bool> events();

	void commit();

	void deinit();

	void onFps(int fps);

	std::string getPath(std::string relpath);

};

#endif

#endif //PIXENGINE_ANDROID_ANDROIDLAYER_H

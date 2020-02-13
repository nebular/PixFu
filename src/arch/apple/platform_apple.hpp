#ifdef __APPLE__
//
// Created by rodo on 2020-01-24.
//

#ifndef GLES3JNI_APPLE_H
#define GLES3JNI_APPLE_H
#include "cocoawindowing.h"
#include "Utils.hpp"
#include "OpenGL.h"

namespace rgl {

	class PixFuPlatformApple : public PixFuPlatform {


		bool init(PixFu *engine);

		std::pair<bool, bool> events();

		void commit();

		void deinit();

		void onFps(int fps);

		std::string getPath(std::string relpath);

	};
}
#endif //GLES3JNI_APPLE_H
#endif

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

	/** Platform configuration, you can pass this object when initing the platform */
	typedef struct sAppleConfig {
		bool allowWindowResize = true;
		bool recreateToResized = false;
	} AppleConfig_t;

	class PixFuPlatformApple : public PixFuPlatform {

		static constexpr unsigned APPLE_NUM_KEYS = 72;
		
	public:

		PixFuPlatformApple(AppleConfig_t config = {});

		static float sfScaleX, sfScaleY;

	private:
		static std::string TAG;

		// platform configuration
		AppleConfig_t mConfiguration;

		// window learnt aspect ratio, will be kept on resizes
		float fAspectRatio;

		/**
		 * Initializes the platform window to run the provided engine
		 */
		bool init(PixFu *engine);

		/**
		 * Process any platform events, and return if the loop should be running and if the
		 * window has focus.
		 */
		std::pair<bool, bool> events();

		/**
		 * Commit current frame. Typically to swap buffers or call related function. It is called
		 * at the end of each frame.
		 */
		void commit();

		/**
		 * Deinit the platform would close the window and the application resources.
		 */
		void deinit();

		/**
		 * Called periodically (but not every frame) can be used to show stats,
		 * or in this case to peek window size to detect user resizes
		 */
		void onFps(PixFu *engine, int fps);

		/**
		 * Gets CWD for user files
		 */
		std::string getPath(std::string relpath);

	};
}
#endif //GLES3JNI_APPLE_H
#endif

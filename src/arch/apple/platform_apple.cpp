#ifdef __APPLE__

//
// Created by rodo on 2020-01-24.
//

#include "Fu.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "platform_apple.hpp"
#include <iostream>
#include <cmath>

namespace Pix {

	void LogV(const std::string &tag, std::string text) {
		std::cerr << "[V:" << tag << "] " << text << std::endl;
	}

	void LogE(const std::string &tag, std::string text) {
		std::cerr << "[E:" << tag << "] " << text << std::endl;
	}

	std::string PixFuPlatformApple::TAG = "Apple";
	float PixFuPlatformApple::sfScaleX = 1;
	float PixFuPlatformApple::sfScaleY = 1;

	PixFuPlatformApple::PixFuPlatformApple(AppleConfig_t config) : mConfiguration(config) {}

	bool PixFuPlatformApple::init(Fu *engine) {

		try {
			init_application();
		} catch (...) {
			return false;
		}

		std::string currentDir(get_current_working_directory());
		ROOTPATH = currentDir + "/";

		int width = engine->screenWidth(), height = engine->screenHeight();
		create_window(engine->APPNAME.c_str(), width, height);

		if (DBG) LogV(TAG, SF("Init Platform for App %s, size %dx%d, CWD ", engine->APPNAME.c_str(), width, height, ROOTPATH.c_str()));

		Mouse::enable();
		Keyboard::enable(APPLE_NUM_KEYS);

		fAspectRatio = (float) width / height;
		sfScaleX = sfScaleY = 1;
		if (width <= get_screen_width() && height <= get_screen_height()) {
			set_window_size(width, height);
			set_window_background_color(0, 0, 0, 1);
			set_window_title_bar_hidden(false);
			set_window_title_hidden(false);
			set_window_resizable(mConfiguration.allowWindowResize);
			return true;
		}
		return false;
	}

	std::pair<bool, bool> PixFuPlatformApple::events() {
		bool focused = window_is_focused();
		process_window_events();
		bool active = !get_window_is_closing();
		return {active, focused};
	}

	void PixFuPlatformApple::commit() {
		refresh_window();
	}

	void PixFuPlatformApple::deinit() {
		if (DBG) LogV(TAG, "Deinit");
		close_window();
		close_application();
	}

	void PixFuPlatformApple::onFps(Fu *engine, int fps) {

		std::string sTitle = engine->APPNAME + " - FPS: " + std::to_string(fps);
		set_window_name(sTitle.c_str());

		// allow resize, but keep aspect ratio
		int w = get_window_width(), h = get_window_height();
		int th = ((float) w / fAspectRatio);

		if (!mConfiguration.recreateToResized) {
			// in hardware rescale mode, opengl is just stretched to fit the window
			// however then we will have to translate mouse coordinates to a modified
			// scale
			sfScaleX = engine->screenWidth() / (float) w;
			sfScaleY = engine->screenHeight() / (float) th;
		}

		if (fabs(h - th) > 2) {
			set_window_size(w, th);
			if (mConfiguration.recreateToResized) {
				// recreate the running engine to the new resolution
				engine->loop_reinit(w, th);
			}
		}
	}

	void Mouse::poll() {
		input(PixFuPlatformApple::sfScaleX * get_mouse_position_x(), PixFuPlatformApple::sfScaleY * get_mouse_position_y());
		inputWheel(get_mouse_scroll_x(), get_mouse_scroll_y());
		for (char i = 0; i < BUTTONS; i++) inputButton(i, get_mouse_button(i));
	}

	void Keyboard::poll() {

		for (int i = 0; i < NUMKEYS; i++) {
			if (get_key_down(::Key::All[i])) {
				Keyboard::pNextState[i] = true;
			}
			if (get_key_up(::Key::All[i])) {
				Keyboard::pNextState[i] = false;
			}
		}

		if (get_modifier_key_down(::SHIFT)) 	Keyboard::pNextState[Keys::SHIFT] = true;
		if (get_modifier_key_up(::SHIFT)) 		Keyboard::pNextState[Keys::SHIFT] = false;
		if (get_modifier_key_down(::CONTROL)) 	Keyboard::pNextState[Keys::CTRL] = true;
		if (get_modifier_key_up(::CONTROL)) 	Keyboard::pNextState[Keys::CTRL] = false;
		if (get_modifier_key_down(::OPTION)) 	Keyboard::pNextState[Keys::ALT] = true;
		if (get_modifier_key_up(::OPTION)) 		Keyboard::pNextState[Keys::ALT] = false;
		if (get_modifier_key_down(::COMMAND)) 	Keyboard::pNextState[Keys::COMMAND] = true;
		if (get_modifier_key_up(::COMMAND)) 	Keyboard::pNextState[Keys::COMMAND] = false;

	}

	void Fu::start() {
		loop();
	}

}

#endif

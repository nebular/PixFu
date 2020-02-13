#ifdef __APPLE__

//
// Created by rodo on 2020-01-24.
//

#include "PixFu.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "apple.h"
#include <iostream>

namespace rgl {

void LogV(std::string tag, std::string text) {
	std::cerr << "["<<tag<<"] V "<<text<<std::endl;
}

void LogE(std::string tag, std::string text) {
	std::cerr << "["<<tag<<"] E "<<text<<std::endl;
}

bool PixFuPlatformApple::init(PixFu *engine) {
	
	try {
		init_application();
	} catch (...) {
		return false;
	}
	
	std::string currentDir(get_current_working_directory());
	ROOTPATH = currentDir + "/";
	
	int width = engine->screenWidth(), height = engine->screenHeight();
	create_window("Tests", width, height);
	std::cerr << "sw " << get_screen_width() << " sh " << get_screen_height() << std::endl;
	
	if (width<=get_screen_width() && height<=get_screen_height()) {
		set_window_size(width, height);
		set_window_background_color(0, 0, 0, 1);
		set_window_title_bar_hidden (false);
		set_window_title_hidden (false);
		set_window_resizable(false);
		return true;
	}
	return false;
}

std::pair<bool,bool> PixFuPlatformApple::events() {
	bool focused = window_is_focused();
	process_window_events();
	bool active = !get_window_is_closing();
	return {active, focused};
}

void PixFuPlatformApple::commit() {
	refresh_window();
}

void PixFuPlatformApple::deinit() {
	close_window();
	close_application();
}

void PixFuPlatformApple::onFps(int fps) {
	std::string sTitle = "PixEngine - FPS: " + std::to_string(fps);
	set_window_name(sTitle.c_str());
}

void Mouse::poll() {
	input(get_mouse_position_x(), get_mouse_position_y());
	inputWheel(get_mouse_scroll_x(), get_mouse_scroll_y());
	for (char i = 0; i < BUTTONS; i++) inputButton(i, get_mouse_button(i));
}

void Keyboard::poll() {
	
	for (int i = 0; i < NUMKEYS; i++) {
		if (get_key_down(::Key::All[i])) {
			pNextState[i] = true;
		}
		if (get_key_up(::Key::All[i])) {
			pNextState[i] = false;
		}
	}
	
	if (get_modifier_key_down(::SHIFT)) pNextState[Keys::SHIFT] = true;
	if (get_modifier_key_up(::SHIFT)) pNextState[Keys::SHIFT] = false;
	if (get_modifier_key_down(::CONTROL)) pNextState[Keys::CTRL] = true;
	if (get_modifier_key_up(::CONTROL)) pNextState[Keys::CTRL] = false;
	
}

void PixFu::start() {
	loop();
}

}

#endif

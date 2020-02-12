#ifdef ANDROID

//
// Created by rodo on 2020-01-24.
//

#include "PixEngine.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "androidlayer.h"
#include <iostream>

bool PixEngineAndroid::init() {


	cLoneKeys = new LoneScreenKey(1);
	LoneScreenKey::currentInstance = cLoneKeys; // todo singleton

	nWindowHeight = nScreenHeight;
	nWindowWidth = nScreenWidth;

	pLoneSensor = new LoneSensor();
	pLoneSensor->init();
	cLoneKeys->reset();

	return true;

}

std::pair<bool,bool> PixEngineAndroid::events() {
	bool focused = window_is_focused();
	process_window_events();
	bool active = !get_window_is_closing();
	return {active, focused};
}

void PixEngineAndroid::commit() {
	refresh_window();
}

void PixEngineAndroid::deinit() {
	close_window();
	close_application();
}

void PixEngineAndroid::onFps(int fps) {
	std::string sTitle = "PixEngine - FPS: " + std::to_string(fps);
	set_window_name(sTitle.c_str());
}

void rgl::Mouse::update() {
	input(get_mouse_position_x(), get_mouse_position_y());
	inputWheel(get_mouse_scroll_x(), get_mouse_scroll_y());
	for (char i = 0; i < BUTTONS; i++) inputButton(i, get_mouse_button(i));
}

void rgl::Keyboard::update() {

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

bool rgl::PixEngine::start() {
	return true;
}

#endif

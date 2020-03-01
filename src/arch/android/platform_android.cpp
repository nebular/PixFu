#ifdef ANDROID

//
// Created by rodo on 2020-01-24.
//

#include "Fu.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "platform_android.hpp"
#include "OpenGlUtils.h"
#include "Utils.hpp"
#include "androidapi.h"

#include <iostream>

namespace Pix {

	void LogV(const std::string &tag, std::string text) {
		ALOGV("[V:%s] %s", tag.c_str(), text.c_str());
	}

	void LogE(const std::string &tag, std::string text) {
		ALOGE("[E:%s] %s", tag.c_str(), text.c_str());
	}

	static constexpr int ACTION_UP = 1;
	static constexpr int ACTION_DOWN = 0;
	static constexpr int ACTION_MOVE = 2;
	static constexpr int ACTION_CANCEL = 3;

// other than primary finger
	static constexpr int ACTION_POINTER_DOWN = 5;
	static constexpr int ACTION_POINTER_UP = 6;

	PixFuPlatformAndroid::PixFuPlatformAndroid(Fu *bootInstance) {
		OpenGlUtils::VERSION = "v300es";
		pBootInstance = bootInstance;
	}

	PixFuPlatformAndroid::~PixFuPlatformAndroid() {
		deinit();
	}

	bool PixFuPlatformAndroid::init(Fu *engine) {

		Mouse::enable();
		Keyboard::enable();

		cLoneKeys = new LoneScreenKey(1);
		LoneScreenKey::currentInstance = cLoneKeys; // todo singleton

		cLoneKeys->reset();

		return true;

	}

	std::pair<bool, bool> PixFuPlatformAndroid::events() {
		// onpause will route here
		bool focused = true; // window_is_focused();
		bool active = true; // !get_window_is_closing();
		return {active, focused};
	}

	void PixFuPlatformAndroid::commit() {
		// no frame commit, android does that after returning from tick
	}

	void PixFuPlatformAndroid::deinit() {
		// something will come here for sure
	}

	void PixFuPlatformAndroid::onFps(Fu *engine, int fps) {
		std::string sTitle = "PixFu - FPS: " + std::to_string(fps);
	}

	void Mouse::poll() {
		// already done
	}

	void Keyboard::poll() {

	}

	void Fu::start() {
		// this platform does not run a loop
	}


	// called from Launcher to send a MotionEvent (Touch)
	void PixFuPlatformAndroid::inputMotionEvent(MotionEvent_t event) {


		Mouse *mouse = Mouse::instance();

		if (mouse == nullptr) return;


		// we have touch information
		// lets simulate a mouse
		//
		// I tried an elaborate routine to simulate clicks on touch but it was getting too complicated and prone to fail
		// I came up with this simple approach that kind of works pretty well. It uses LoneKeys to paint
		// 2 virtual mouse buttons on screen, and multitouch is supported so you can move the pointer with one finger
		// and use the buttons with other. For implementing another mouse behavior, you have all details in tCurrentMotionEvent
		//

		tCurrentMotionEvent = event;
		int action = event.RawAction;

		// Iterate through virtual keys and set their state.
		// will return 0 -> key pressed with primary pointer (X0Y0), 1 -> with secondary (X1Y1), -1 No key pressed.
		int keysRegion = cLoneKeys->sync(tCurrentMotionEvent.X0, tCurrentMotionEvent.Y0,
										 tCurrentMotionEvent.X1, tCurrentMotionEvent.Y1,
										 tCurrentMotionEvent.Action == ACTION_UP);

		bool twoFingers = event.PointersCount > 1;
//		LogV ("ANDR", SF("Event: buttons %d, action %d, index %d", tCurrentMotionEvent.PointersCount, tCurrentMotionEvent.RawAction, tCurrentMotionEvent.PointerId));

		switch (action) {

			case ACTION_POINTER_UP:

				for (int i = 0; i < mouse->BUTTONS; i++)
					mouse->inputButton(i, false);

				break;

			case ACTION_UP:
			case ACTION_CANCEL:

				// end touch (no fingers on screen)
				// reset virtual mouse button status
				cLoneKeys->reset();

				// break through to update coordinates and real state (that we have reset here)

			case ACTION_DOWN:
			case ACTION_POINTER_DOWN:

				if (twoFingers)
					for (int i = 0; i < mouse->BUTTONS; i++)
						mouse->inputButton(i, cLoneKeys->GetFakeMouse(i));

			case ACTION_MOVE:

				// These actions update the engine mouse button status from
				// our local copy in cLoneKeys

				if (twoFingers)
					for (int i = 0; i < mouse->BUTTONS; i++)
						mouse->inputButton(i, cLoneKeys->GetFakeMouse(i));

				// then updates the right coordinates: One of them is the finger in the buttons area,
				// the other the pointer finger. We select the right one !

				if (keysRegion != 0)
					mouse->input(tCurrentMotionEvent.X0, tCurrentMotionEvent.Y0);
				else if (twoFingers) // may happen that only one finger is pressing buttons but none is pointing
					mouse->input(tCurrentMotionEvent.X1, tCurrentMotionEvent.Y1);
				break;
			default:
				break;
		}
	}


}

#endif

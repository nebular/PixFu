//
//  Mouse.cpp
//  PixFu
//
//  Created by rodo on 12/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "Mouse.hpp"

namespace rgl {

///////// MOUSE SINGLETON
	std::string Mouse::TAG="Mouse";
	Mouse *Mouse::pInstance = nullptr;

	Mouse::Mouse(int buttons) : BUTTONS(buttons) {
		pNextButtonState = new bool[BUTTONS];
		pButtonState = new bool[BUTTONS];
		pStateHeld = new bool[BUTTONS];
		pStatePressed = new bool[BUTTONS];
		pStateReleased = new bool[BUTTONS];
	};

	Mouse::~Mouse() {
		delete pNextButtonState;
		delete pButtonState;
		delete pStateHeld;
		delete pStatePressed;
		delete pStateReleased;
		pNextButtonState = pButtonState = pStateHeld = pStateReleased = pStatePressed = nullptr;

		if (pInstance == this)
			pInstance = nullptr;

	}

	void Mouse::enable(int buttons) {

		if (DBG) LogV(TAG, SF("mouse enable, %d buttons", buttons));

		if (pInstance == nullptr)
			pInstance = new Mouse(buttons);
	}

	void Mouse::disable() {
		if (pInstance != nullptr) {
			delete pInstance;
			pInstance = nullptr;
		}
	}

	Mouse *Mouse::instance() {
		return pInstance;
	}

	// snapshot values
	void Mouse::sync() {

		nX = nNewX;
		nY = nNewY;
		nWheelX = nNewWheelX;
		nWheelY = nNewWheelY;

		for (int i = 0; i < BUTTONS; i++) {

			pStateReleased[i] = pStatePressed[i] = false;

			if (pNextButtonState[i] != pButtonState[i]) {
				if (pNextButtonState[i]) {
					pStatePressed[i] = !pStateHeld[i];
					pStateHeld[i] = true;
				} else {
					pStateReleased[i] = true;
					pStateHeld[i] = false;
				}
			}

			pButtonState[i] = pNextButtonState[i];
		}
	}

}


//
//  Mouse.hpp
//  PixFu
//
//  Created by rodo on 12/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma once

#include "Fu.hpp"
#include "Utils.hpp"

namespace Pix {

	class Mouse : public InputDevice {

		// todo unfriends
		friend class PixFuPlatformAndroid;

		static Mouse *pInstance;
		static std::string TAG;

		int nX = 0, nY = 0, nWheelX = 0, nWheelY = 0;
		int nNewX = 0, nNewY = 0, nNewWheelX = 0, nNewWheelY = 0;
		bool *pNextButtonState = nullptr;
		bool *pButtonState = nullptr;
		bool *pStatePressed;
		bool *pStateReleased;
		bool *pStateHeld;

		bool *getBuffer();

		void input(int px, int py);

		void inputWheel(int sx, int sy);

		void inputButton(int b, bool status);

		Mouse(int buttons = 2);

	public:

		const int BUTTONS;

		// singleton: enable mouse
		static void enable(int buttons = 2);

		// singleton: disable mouse
		static void disable();

		// singleton: get instance if enabled
		static Mouse *instance();

		static int x();

		static int y();

		static bool isPressed(int button);

		static bool isHeld(int button);

		static bool isReleased(int button);

		~Mouse();

		void poll();

		void sync(float fElapsedTime);

	};

	inline bool *Mouse::getBuffer() { return pNextButtonState; }


	// query mouse position (static)
	inline int Mouse::x() { return pInstance->nX; }

	inline int Mouse::y() { return pInstance->nY; }

	// input coordinates from platform layer
	inline void Mouse::input(int px, int py) {
		nNewX = px;
		nNewY = py;
	}

	inline void Mouse::inputWheel(int px, int py) {
		nNewWheelX = px;
		nNewWheelY = py;
	}

	inline void Mouse::inputButton(int b, bool stat) { pNextButtonState[b] = stat; }

	// button status (Static)
	inline bool Mouse::isPressed(int button) { return pInstance->pStatePressed[button]; }

	inline bool Mouse::isHeld(int button) { return pInstance->pStateHeld[button]; }

	inline bool Mouse::isReleased(int button) { return pInstance->pStateReleased[button]; }

}

#pragma clang diagnostic pop
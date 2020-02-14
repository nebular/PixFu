//
//  Mouse.hpp
//  PixFu
//
//  Created by rodo on 12/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef Mouse_hpp
#define Mouse_hpp

#include "PixFu.hpp"
#include "Utils.hpp"

namespace rgl {

	class Mouse : public InputDevice {

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

		int _x();

		int _y();

		bool _isPressed(int button);

		bool _isHeld(int button);

		bool _isReleased(int button);

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

// query mouse position (instance)
	inline int Mouse::_x() { return nX; }

	inline int Mouse::_y() { return nY; }

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

// button status (instance)
	inline bool Mouse::_isPressed(int button) { return pStatePressed[button]; }

	inline bool Mouse::_isHeld(int button) { return pStateHeld[button]; }

	inline bool Mouse::_isReleased(int button) { return pStateReleased[button]; }

// button status (Static)
	inline bool Mouse::isPressed(int button) { return pInstance->_isPressed(button); }

	inline bool Mouse::isHeld(int button) { return pInstance->_isHeld(button); }

	inline bool Mouse::isReleased(int button) { return pInstance->_isReleased(button); }

}
#endif /* Mouse_hpp */

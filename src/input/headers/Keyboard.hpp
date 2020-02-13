//
//  Keyboard.hpp
//  PixFu
//
//  Created by rodo on 12/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef Keyboard_hpp
#define Keyboard_hpp

#include "PixFu.hpp"

namespace rgl {

	enum Keys {
		NONE,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		UP, DOWN, LEFT, RIGHT,
		SPACE, TAB,
		INS, DEL, HOME, END, PGUP, PGDN,
		BACK, ESCAPE, RETURN, ENTER, PAUSE, SCROLL,
		SHIFT, CTRL
	};

	class Keyboard : public InputDevice {

		friend class PixFuPlatformAndroid;
		friend class LoneScreenKey;

		static Keyboard *pInstance;


		bool *pNextState;
		bool *pThisState;
		bool *pStatePressed;
		bool *pStateReleased;
		bool *pStateHeld;

		Keyboard(int numkeys);

		bool _isPressed(Keys key);

		bool _isHeld(Keys key);

		bool _isReleased(Keys key);

		bool *getBuffer();

	public:

		const int NUMKEYS;

		static void enable(int numkeys = 67);

		static void disable();

		static Keyboard *instance();

		static bool isPressed(Keys key);

		static bool isHeld(Keys key);

		static bool isReleased(Keys key);

		~Keyboard();

		void poll();

		void sync();
	};

	inline bool *Keyboard::getBuffer() { return pNextState; }

// query key position
	inline bool Keyboard::_isPressed(Keys key) { return pStatePressed[key]; }

	inline bool Keyboard::_isReleased(Keys key) { return pStateReleased[key]; }

	inline bool Keyboard::_isHeld(Keys key) { return pStateHeld[key]; }

// query key position (static)
	inline bool Keyboard::isPressed(Keys key) { return pInstance->pStatePressed[key]; }

	inline bool Keyboard::isReleased(Keys key) { return pInstance->pStateReleased[key]; }

	inline bool Keyboard::isHeld(Keys key) { return pInstance->pStateHeld[key]; }

}
#endif /* Keyboard_hpp */

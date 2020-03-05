//
//  Keyboard.hpp
//  PixFu
//
//  Created by rodo on 12/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#include "Fu.hpp"

namespace Pix {

	enum Keys {
		NONE,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		UP, DOWN, LEFT, RIGHT,
		SPACE, TAB,
		INS, DEL, HOME, END, PGUP, PGDN,
		BACK, ESCAPE, RETURN, ENTER, PAUSE, SCROLL,
		SHIFT, CTRL, ALT, COMMAND
	};

	class Keyboard : public InputDevice {

		friend class PixFuPlatformAndroid;

		friend class LoneScreenKey;

		static std::string TAG;
		static Keyboard *pInstance;

		static bool *pNextState;
		static bool *pThisState;
		static bool *pStatePressed;
		static bool *pStateReleased;
		static bool *pStateHeld;

		Keyboard(int numkeys);

		static bool *getBuffer();

	public:

		const int NUMKEYS;

		/**
		 * Enables the keyboard. If called before onUserUpdate, then the keyboard
		 * will be automatically added as an Input Device. If called after, you will
		 * need to dd it (addInputDevice(Keyboard::instance()))
		 */

		static void enable(int numkeys = 67);

		/**
		  * Disables the keyboard input device.
		 */

		static void disable();

		/**
		 * Whether a key is pressed
		 * @param key key code
		 * @return whether
		 */

		static bool isPressed(Keys key);

		/**
		 * Whether a key is held
		 * @param key key code
		 * @return whether
		 */

		static bool isHeld(Keys key);

		/**
		 * Whether a key is released
		 * @param key key code
		 * @return whether
		 */

		static bool isReleased(Keys key);

		static Keyboard *instance();
		
		~Keyboard();

		void poll();

		void sync(float fElapsedTime);
	};

	inline Keyboard *Keyboard::instance() { return pInstance; }
	inline bool *Keyboard::getBuffer() { return pNextState; }

	// query key position (static)
	inline bool Keyboard::isPressed(Keys key) { return pStatePressed[key]; }

	inline bool Keyboard::isReleased(Keys key) { return pStateReleased[key]; }

	inline bool Keyboard::isHeld(Keys key) { return pStateHeld[key]; }

}

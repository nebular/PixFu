//
//  Keyboard.hpp
//  PixEngine
//
//  Created by rodo on 12/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef Keyboard_hpp
#define Keyboard_hpp

#include "PixEngine.hpp"

namespace rgl {

class Keyboard : public InputDevice {
	
	enum KeyNames {
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
	
	static Keyboard *pInstance;
	
	const int NUMKEYS;
	
	bool *pNextState;
	bool *pThisState;
	bool *pStatePressed;
	bool *pStateReleased;
	bool *pStateHeld;
	
	Keyboard(int numkeys);
	bool _isPressed(KeyNames key);
	bool _isHeld(KeyNames key);
	bool _isReleased(KeyNames key);
	
public:
	
	static void enable(int numkeys = 67);
	static void disable();
	static Keyboard *instance();
	static bool isPressed(KeyNames key);
	static bool isHeld(KeyNames key);
	static bool isReleased(KeyNames key);
	
	~Keyboard();
	void update();
	void sync();
};

// query key position
inline bool Keyboard::_isPressed(KeyNames key) { return pStatePressed[key];}
inline bool Keyboard::_isReleased(KeyNames key) { return pStateReleased[key];}
inline bool Keyboard::_isHeld(KeyNames key) { return pStateHeld[key];}

// query key position (static)
inline bool Keyboard::isPressed(KeyNames key) { return pInstance->pStatePressed[key];}
inline bool Keyboard::isReleased(KeyNames key) { return pInstance->pStateReleased[key];}
inline bool Keyboard::isHeld(KeyNames key) { return pInstance->pStateHeld[key];}

}
#endif /* Keyboard_hpp */

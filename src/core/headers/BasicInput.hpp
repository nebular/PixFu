//
//  BasicInput.hpp
//  PixEngine
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef BasicInput_hpp
#define BasicInput_hpp

namespace rgl {

class Mouse {
	
	const int BUTTONS;
	int nX = 0, nY=0, nWheelX=0, nWheelY=0;
	int nNewX=0, nNewY=0, nNewWheelX=0, nNewWheelY=0;
	bool *pNextButtonState = nullptr;
	bool *pButtonState = nullptr;
	bool *pStatePressed;
	bool *pStateReleased;
	bool *pStateHeld;
	
	void input(int px, int py);
	void inputWheel(int sx, int sy);
	void inputButton(int b, bool status);
	
public:
	Mouse(int buttons = 2);
	void update();
	void sync();
	
	int x();
	int y();
	bool isPressed(int button);
	bool isHeld(int button);
	bool isReleased(int button);
};

inline int Mouse::x() { return nX; }
inline int Mouse::y() { return nY; }

inline bool Mouse::isPressed(int button) { return pStatePressed[button]; }
inline bool Mouse::isHeld(int button) { return pStateHeld[button]; }
inline bool Mouse::isReleased(int button) { return pStateReleased[button]; }

class Keyboard {
	
	enum KeyNames {
		NONE,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		UP, DOWN, LEFT, RIGHT,
		SPACE, TAB,
		INS, DEL, HOME, END, PGUP, PGDN,
		BACK, ESCAPE, RETURN, ENTER, PAUSE, SCROLL,
		SHIFT, CTRL,
	};
	
	const int NUMKEYS;
	bool *pNextState;
	bool *pThisState;
	bool *pStatePressed;
	bool *pStateReleased;
	bool *pStateHeld;
	
	
public:
	Keyboard(int numkeys);
	void update();
	void sync();
	bool isPressed(KeyNames key);
	bool isHeld(KeyNames key);
	bool isReleased(KeyNames key);
};

};
#endif /* BasicInput_hpp */

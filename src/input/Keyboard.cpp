//
//  Keyboard.cpp
//  PixEngine
//
//  Created by rodo on 12/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "Keyboard.hpp"

namespace rgl {

///////// KEYBOARD SINGLETON

Keyboard *Keyboard::pInstance = nullptr;

void Keyboard::enable(int numkeys) {
	if (pInstance == nullptr)
		pInstance = new Keyboard(numkeys);
}

void Keyboard::disable() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

Keyboard *Keyboard::instance() {
	return pInstance;
}

Keyboard::Keyboard(int numkeys):NUMKEYS(numkeys) {
	pNextState = new bool[NUMKEYS];
	pThisState = new bool[NUMKEYS];
	pStateHeld = new bool[NUMKEYS];
	pStatePressed = new bool[NUMKEYS];
	pStateReleased = new bool[NUMKEYS];
	for (int i = 0; i<NUMKEYS; i++)
		pThisState[i] = pNextState[i]=pStatePressed[i]=pStateHeld[i]=false;
};

Keyboard::~Keyboard() {
	delete pNextState;
	delete pThisState;
	delete pStateHeld;
	delete pStatePressed;
	delete pStateReleased;
	pNextState = pThisState = pStateHeld = pStateReleased = pStatePressed = nullptr;
	if (pInstance == this)
		pInstance = nullptr;
}

void Keyboard::sync() {
	
	for (int i = 0; i < NUMKEYS; i++) {
		
		pStateHeld[i] = pStatePressed[i] = false;
		
		if (pNextState[i] != pThisState[i]) {
			if (pNextState[i]) {
				pStatePressed[i] = !pStateHeld[i];
				pStateHeld[i] = true;
			} else {
				pStateReleased[i] = true;
				pStateHeld[i] = false;
			}
		}
		
		pThisState[i] = pNextState[i];
	}
}

}

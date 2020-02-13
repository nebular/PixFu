//
//  Keyboard.cpp
//  PixEngine
//
//  Created by rodo on 12/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "Keyboard.hpp"
#include "Utils.hpp"

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
		
		pStateReleased[i] = pStatePressed[i] = false;
		
		if (pNextState[i] != pThisState[i]) {
			if (pNextState[i]) {
				pStatePressed[i] = !pStateHeld[i];
				pStateHeld[i] = true;
				LogV("keyb", SF("next IS P, code %d old %d new %d pressed %d held %d", i, pThisState[i], pNextState[i], pStatePressed[i], pStateHeld[i]));
			} else {
				pStateReleased[i] = true;
				pStateHeld[i] = false;
				LogV("keyb", SF("next NOTP, code %d old %d new %d pressed %d held %d", i, pThisState[i], pNextState[i], pStatePressed[i], pStateHeld[i]));
			}
		}
		
		pThisState[i] = pNextState[i];
	}
}

}


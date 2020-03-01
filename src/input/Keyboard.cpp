//
//  Keyboard.cpp
//  PixFu
//
//  A Keyboard singleton based on olcPGE keyboard - onelonecoder.com
//
//  Created by rodo on 12/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "Keyboard.hpp"
#include "Utils.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
#pragma clang diagnostic ignored "-Wunused-parameter"
namespace Pix {

	///////// KEYBOARD SINGLETON

	std::string Keyboard::TAG = "Keyboard";
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

	Keyboard::Keyboard(int numkeys) : NUMKEYS(numkeys) {

		if (DBG)
			LogV(TAG, SF("construct for %d keys", numkeys));

		pNextState = new bool[NUMKEYS];
		pThisState = new bool[NUMKEYS];
		pStateHeld = new bool[NUMKEYS];
		pStatePressed = new bool[NUMKEYS];
		pStateReleased = new bool[NUMKEYS];

		for (int i = 0; i < NUMKEYS; i++)
			pThisState[i] = pNextState[i] = pStatePressed[i] = pStateHeld[i] = false;

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

	void Keyboard::sync(float fElapsedTime) {

		for (int i = 0; i < NUMKEYS; i++) {

			pStateReleased[i] = pStatePressed[i] = false;

			if (pNextState[i] != pThisState[i]) {
				if (pNextState[i]) {
					pStatePressed[i] = !pStateHeld[i];
					pStateHeld[i] = true;
					LogV(TAG, SF("Held %d", i));
				} else {
					pStateReleased[i] = true;
					pStateHeld[i] = false;
					LogV(TAG, SF("Release %d", i));
				}
			}

			pThisState[i] = pNextState[i];
		}
	}

}


#pragma clang diagnostic pop
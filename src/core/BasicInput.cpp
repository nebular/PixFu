//
//  BasicInput.cpp
//  PixEngine
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "BasicInput.hpp"

using namespace rgl;

Keyboard::Keyboard(int numkeys):NUMKEYS(numkeys) {
	pNextState = new bool[NUMKEYS];
	pThisState = new bool[NUMKEYS];
	pStateHeld = new bool[NUMKEYS];
	pStatePressed = new bool[NUMKEYS];
	pStateReleased = new bool[NUMKEYS];
	for (int i = 0; i<NUMKEYS; i++) pThisState[i] = pNextState[i]=pStatePressed[i]=pStateHeld[i]=false;
};

Keyboard::~Keyboard() {
	delete pNextState;
	delete pThisState;
	delete pStateHeld;
	delete pStatePressed;
	delete pStateReleased;
}

bool Keyboard::isPressed(KeyNames key) { return pStatePressed[key];}
bool Keyboard::isReleased(KeyNames key) { return pStateReleased[key];}
bool Keyboard::isHeld(KeyNames key) { return pStateHeld[key];}

void Keyboard::sync() {
	
	// Handle User Input - Keyboard
	
	for (int i = 0; i < NUMKEYS; i++) {
		
		pStateHeld[i]=pStatePressed[i]=false;
		
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

void Mouse::input(int px, int py) { nNewX=px; nNewY = py; }
void Mouse::inputWheel(int px, int py) { nNewWheelX=px; nNewWheelY = py; }
void Mouse::inputButton(int b, bool stat) { pNextButtonState[b] = stat; }

Mouse::Mouse(int buttons):BUTTONS(buttons) {
	pNextButtonState= new bool[BUTTONS];
	pButtonState= new bool[BUTTONS];
	pStateHeld= new bool[BUTTONS];
	pStatePressed= new bool[BUTTONS];
	pStateReleased= new bool[BUTTONS];
};
void Mouse::sync() {
	nX=nNewX; nY=nNewY; nWheelX=nNewWheelX; nWheelY=nNewWheelY;
	for (int i=0; i<BUTTONS; i++) {
		
		pStateReleased[i]=pStatePressed[i]=false;
		
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

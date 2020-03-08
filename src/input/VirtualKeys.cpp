//
// VirtualKeys Fu Extension
// PixFu Engine
//
// Provides on-screen virtual keys.
//
// Created by rodo on 2020-01-26.
//

#include "VirtualKeys.hpp"
#include "Keyboard.hpp"
#include "Canvas2D.hpp"
#include "Mouse.hpp"
#include "Font.hpp"

namespace Pix {

	VirtualKeys *VirtualKeys::currentInstance = nullptr;

	VirtualKeys::VirtualKeys(VirtualKeysConfig_t config)
	: FuExtension(true), CONFIG(std::move(config)) {}
	
	/*
	int VirtualKeys::input(int32_t x, int32_t y) {
		x /= fResolution;
		y /= fResolution;
		for (ScreenKey_t key : vKeys)
			if (x > key.x0 && x < key.x0 + key.w && y > key.y0 && y < key.y0 + key.h)
				return key.code;

		return NO_KEYZ;
	}
	*/

	void VirtualKeys::add(ScreenKey_t key) {
		vKeys.push_back(key);
	}

	// convenience to add 4 arrow keys
	void VirtualKeys::addCursors(int PX, int PY) {
		add({Pix::Keys::UP, PX, PY + 0, 100, 50, false, "U"});
		add({Pix::Keys::LEFT, PX, PY + 50, 50, 50, false, "L"});
		add({Pix::Keys::RIGHT, PX + 50, PY + 50, 50, 50, false, "R"});
		add({Pix::Keys::DOWN, PX, PY + 100, 100, 50, false, "D"});

	}

	// convenience to add mouse buttons
	bool VirtualKeys::addMouse(int PX, int PY) {
		if (CONFIG.multitouch) {
			add({0, PX, PY, 50, 50, true, "L"});
			add({1, PX + 50, PY, 50, 50, true, "R"});
			return true;
		}
		return false;
	}

	// remove all vkeys
	void VirtualKeys::clear() {
		vKeys.clear();
	}

	bool VirtualKeys::GetFakeMouse(int button) { return aMyMouseStats[button]; }

	void VirtualKeys::DrawSelf(Pix::Canvas2D *canvas, Pixel color, bool mouse) {

		if (mouse)
			canvas->drawCircle(Mouse::x(), Mouse::y(), 40.0, Colors::RED);

		for (ScreenKey_t& key : vKeys) {
			if (((!key.mouse && Keyboard::instance()->isHeld(static_cast<Pix::Keys>(key.code)))
				 || (key.mouse && aMyMouseStats[key.code])))
				canvas->fillRect(key.x0, key.y0, key.w, key.h, color);
			else
				canvas->drawRect(key.x0, key.y0, key.w, key.h, color);

			if (key.caption.size() > 0) {
				const int SC = 2;
				const int FH = canvas->font()->INFO.charHeight * SC;
				const int FW = (int)key.caption.size() * canvas->font()->INFO.charWidth *SC;
				canvas->drawString(key.x0 + key.w / 2 - FW/2, key.y0+key.h/2-FH/2, key.caption, color, SC);
			}

		}

	}

	void VirtualKeys::reset() {
		for (int i = 0; i < Mouse::instance()->BUTTONS; i++) aMyMouseStats[i] = false;
	}

	void VirtualKeys::tick(Fu *engine, float fElapsedTime) {
		if (!CONFIG.multitouch && vKeys.size()>0) {
			if (Mouse::isPressed(0) || Mouse::isHeld(0) || Mouse::isReleased(0)) {
				input(Mouse::x(), Mouse::y(), 0,0,  Mouse::isReleased(0));
			}
			DrawSelf(engine->canvas(), CONFIG.color, true);
		}
	}

	bool VirtualKeys::init( Fu *engine) {
		return true;
	}


// todo
	int
	VirtualKeys::input(int32_t x0, int32_t y0, int32_t x1, int32_t y1, bool isUp) {

		int MAXBUTTONS = Mouse::instance()->BUTTONS;
		bool *keystats = Keyboard::instance()->pNextState;

		x0 /= CONFIG.resolution;
		y0 /= CONFIG.resolution;
		x1 /= CONFIG.resolution;
		y1 /= CONFIG.resolution;

		bool any = NO_KEYZ;

		for (ScreenKey_t& key : vKeys) {
			if (x1 > key.x0 && x1 < key.x0 + key.w && y1 > key.y0 && y1 < key.y0 + key.h) {
				// if the secondary pointer is in the key region
				any = 1;
				if (DBG) LogV("Keys", SF("reg 1 sec code %d, isUp %d", key.code, isUp));
				if (!key.mouse) keystats[key.code] = !isUp;
				else if (key.code < MAXBUTTONS) aMyMouseStats[key.code] = true;
			} else if (x0 > key.x0 && x0 < key.x0 + key.w && y0 > key.y0 && y0 < key.y0 + key.h) {
				// if the primary pointer is in the key region
				any = 0;
				if (DBG) LogV("Keys", SF("reg 0 prim code %d, isUp %d", key.code, isUp));
				if (!key.mouse) keystats[key.code] = !isUp;
				else if (key.code < MAXBUTTONS) aMyMouseStats[key.code] = true;
			} else {
				if (key.mouse) aMyMouseStats[key.code] = false;
				else keystats[key.code] = false;
			}
		}

		return any;
	}
}

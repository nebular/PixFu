//
// Created by rodo on 2020-01-26.
//

#ifndef GLES3JNI_LONESCREENKEY_H
#define GLES3JNI_LONESCREENKEY_H

#include <vector>
#include "Canvas2D.hpp"

#define NO_KEYZ -1

namespace Pix {

	typedef struct sScreenKey {
		int code;
		int x0, y0, w, h;
		bool mouse = false;
	} ScreenKey_t;

	class LoneScreenKey {
		std::vector<ScreenKey_t> vKeys;
		float fResolution = 1;
		bool aMyMouseStats[8];

	public:
		LoneScreenKey(int resolution);

		static LoneScreenKey *currentInstance;

		int input(int32_t x, int32_t y);

		int sync(int32_t x0, int32_t y0, int32_t x1, int32_t y1, bool isUp = false);

		void add(ScreenKey_t key);

		void addCursors(int px, int py);

		void addMouse(int px, int py);

		void clear();

		void reset();

		bool GetFakeMouse(int button);

		void DrawSelf(Canvas2D *canvas, Pixel color, bool mouse = false);
	};
}

#endif //GLES3JNI_LONESCREENKEY_H

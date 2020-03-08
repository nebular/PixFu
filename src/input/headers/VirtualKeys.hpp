//
// VirtualKeys Fu Extension
// PixFu Engine
//
// Provides on-screen virtual keys.
//
// Created by rodo on 2020-01-26.
//

#pragma once

#include <vector>
#include "Fu.hpp"

#define NO_KEYZ -1

namespace Pix {

	class Canvas2D;
	class Fu;

	typedef const struct sVirtualKeysConfig {
		const Pixel color = Colors::WHITE;
		const int resolution = 1;
		const bool multitouch = false;
	} VirtualKeysConfig_t;

	typedef const struct sScreenKey {
		const int code;
		const int x0, y0, w, h;
		const bool mouse = false;
		const std::string caption="";
	} ScreenKey_t;

	class VirtualKeys : public FuExtension {

		static VirtualKeys *currentInstance;

		/** Indicates platform will use alternate mutitouch entry data point (For Android) */
		const VirtualKeysConfig_t CONFIG;
		
		/** Virtual Keys */
		std::vector<ScreenKey_t> vKeys;
		
		
		/** Mouse status emulator */
		bool aMyMouseStats[8];

		VirtualKeys(VirtualKeysConfig_t config={});

		/**
		 * Draws the extension UI
		 */
		void DrawSelf(Canvas2D *canvas, Pixel color, bool mouse = false);


	public:

		/**
		 * Enables the Virtual Keys. REmember you must then add the Extension
		 * to the engine using addExtension.
		 */
		static void enable(VirtualKeysConfig_t config);
		
		/**
		 * Returns VirtualKeys instance, if any.
		 */

		static VirtualKeys *instance();
		
		virtual ~VirtualKeys() = default;

		bool init(Fu *engine) override;
		void tick(Fu *engine, float fElapsedTime) override;

//		int input(int32_t x, int32_t y);
		
		/**
		 * Data Entry point for multitouch devices
		 */
		int input(int32_t x0, int32_t y0, int32_t x1, int32_t y1, bool isUp = false);

		/**
		 * Add a new Virtual Key
		 */
		void add(ScreenKey_t key);

		/**
		 * Add a cluster of keys mapped to cursor keys
		 */
		void addCursors(int px, int py);

		/**
		 * Add virtual mouse buttons (only makes sense for multitouch)
		 * @return whether keys were added
		 */

		bool addMouse(int px, int py);

		/**
		 * Deletes all added keys
		 */
		void clear();

		/**
		 * Reset all keys status to false
		 */
		void reset();

		/**
		 * Gets state of virtual mouse keys
		 */
		bool GetFakeMouse(int button);

	};

	inline VirtualKeys *VirtualKeys::instance() { return currentInstance; }


	inline void
	VirtualKeys::enable(VirtualKeysConfig_t config) {
		if (currentInstance == nullptr)
			currentInstance = new VirtualKeys(config);
	}


}


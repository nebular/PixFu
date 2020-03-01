//
//  FuExtension.hpp
//  PixEngine
//
//  Created by rodo on 16/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

namespace Pix {

	class Fu;

	/**
	 * a PixEngine extension. Extensions are added with PixEngine::addExtension.
	 */

	class FuExtension {

	public:

		virtual ~FuExtension();

		virtual bool init(Fu *engine);

		virtual void tick(Fu *engine, float fElapsedTime) = 0;
	};

	inline FuExtension::~FuExtension() {}

	inline bool FuExtension::init(Fu *engine) { return true; }

}


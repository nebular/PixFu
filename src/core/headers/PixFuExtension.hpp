//
//  PixFuExtension.hpp
//  PixEngine
//
//  Created by rodo on 16/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

namespace rgl {

	class PixFu;

	/**
	 * a PixEngine extension. Extensions are added with PixEngine::addExtension.
	 */

	class PixFuExtension {

	public:
		virtual ~PixFuExtension();

		virtual bool init(PixFu *engine);

		virtual void tick(PixFu *engine, float fElapsedTime) = 0;
	};

	inline PixFuExtension::~PixFuExtension() {}

	inline bool PixFuExtension::init(PixFu *engine) { return true; }
/*-------------------------------------------------------------------*/
}


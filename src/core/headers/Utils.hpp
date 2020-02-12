//
//  Utils.hpp
//  PixEngine
//
//  Created by rodo on 12/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef Utils_h
#define Utils_h
#include <chrono>

namespace rgl {
	inline long nowms() { return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); }
}

#endif /* Utils_h */

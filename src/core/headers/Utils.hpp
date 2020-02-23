//
//  Utils.hpp
//  PixFu
//
//  Created by rodo on 12/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <stdexcept>

namespace rgl {

	static constexpr bool DBG = true;

	// ms now
	inline long nowms() {
		return static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()).count());
	}
	
	// ns now
	inline long nowns() {
		return  std::chrono::duration_cast< std::chrono::microseconds >(std::chrono::system_clock::now().time_since_epoch()).count();
	}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"

	template<typename ... Args>
	std::string SF(const std::string &format, Args ... args) {
		size_t size = static_cast<size_t>(snprintf(nullptr, 0, format.c_str(), args ...) +
										  1); // Extra space for '\0'
		if (size <= 0) { throw std::runtime_error("Error during formatting."); }

		std::unique_ptr<char[]> buf(new char[size]);
		snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}

#pragma clang diagnostic pop

	void LogV(const std::string &tag, std::string text);

	void LogE(const std::string &tag, std::string text);

}

#pragma once
#include<vector>
namespace utils {
	template<typename T>
	T lerp(T first, T second, float lerp_val) {
		//lerp_val = max(min(lerp_val, 1), 0);
		return first * (1 - lerp_val) + second * lerp_val;
	}
}
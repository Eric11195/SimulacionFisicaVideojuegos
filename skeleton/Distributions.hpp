#pragma once
#include <random>

namespace Distributions {

	inline static std::default_random_engine generator;

	class LinearDistribution {
	public:
		static float get() {
			return dist(generator);
		}
	private:
		inline static std::uniform_real_distribution<float> dist = std::uniform_real_distribution<float>(0, 1);
	};

	class NormalDistribution {
	public:
		static float get() {
			return dist(generator);
		}
	private:
		inline static std::normal_distribution<float> dist = std::normal_distribution<float>(0, 1);
	};

	class RandomSignDistribution {
	public:
		static float get() {
			return dist(generator) == 0 ? -1 : 1;
		}
	private:
		inline static std::uniform_int_distribution<> dist = std::uniform_int_distribution<>(0, 1);
	};
}
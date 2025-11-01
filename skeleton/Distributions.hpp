#pragma once
#include <random>

namespace Distributions {

	inline static std::random_device rd;
	inline static std::mt19937 generator(rd());

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
		enum normal_deviation {
			d_10 = 0,
			d_05 = 1,
			d_025 = 2
		};
		static float get(normal_deviation nd) {
			return dist[int(nd)](generator);
		}
	private:
		inline static std::normal_distribution<float> dist[3] = {
			std::normal_distribution<float>(0, 1),
			std::normal_distribution<float>(0, 0.5f),
			std::normal_distribution<float>(0, 0.25f)
		};
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
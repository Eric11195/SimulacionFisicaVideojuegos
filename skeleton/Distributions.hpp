#pragma once
#include <random>

class Distribution {
public:
	Distribution()
		: generator(std::random_device()){}
	virtual float return_val();
protected:
	std::mt19937 generator;
};

class ConstantDistribution : public Distribution {
public:
	ConstantDistribution(float constant)
		:constant(constant){}
	virtual float return_val() {
		return constant;
	}
protected:
	const float constant;
};

class LinearDistribution : public Distribution {
public:
	LinearDistribution(float min, float max){
		dist = std::uniform_real_distribution<float>(min, max);
	}
	float return_val() override {
		return dist(generator);
	}
private:
	std::uniform_real_distribution<float> dist;
};

class NormalDistribution : public Distribution {
public:
	NormalDistribution(float mid, float std_deviation) {
		dist = std::normal_distribution<float>(mid,std_deviation);
	}
	float return_val() override {
		return dist(generator);
	}
private:
	std::normal_distribution<float> dist;
};

class RandomSignDistribution : public Distribution {
public:
	RandomSignDistribution(){
		dist = std::uniform_int_distribution<>(0, 1);
	}
	float return_val() override{
		return dist(generator) == 0 ? -1 : 1;
	}
private:
	std::uniform_int_distribution<> dist;
};
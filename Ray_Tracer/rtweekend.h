#pragma once
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

// C++ Std Using
using std::make_shared;
using std::shared_ptr;
using std::sqrt;
using std::fabs;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility Functions
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

inline double random_double() {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

// Common Headers
#include "Color.h"
#include "Ray.h"
#include "Vec3.h"
#include "Interval.h"
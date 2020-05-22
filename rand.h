#ifndef RAND_H
#define RAND_H

#include <random>

inline double nrand() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

inline double nrand(double min, double max) {
  return min + (max - min) * nrand();
}

#endif
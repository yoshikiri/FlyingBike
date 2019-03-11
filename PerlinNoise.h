#ifndef PERLIN_NOISE
#define PERLIN_NOISE

#include <vector>
#include <numeric>
#include <random>
#include <algorithm>

class PerlinNoise{
public:
  PerlinNoise();
  PerlinNoise(unsigned int seed);
  double noise(double x, double y, double z);

private:
  std::vector<int> p;
  double fade(double t);
  double lerp(double t, double a, double b);
  double grad(int hash, double x, double y, double z);
};


#endif
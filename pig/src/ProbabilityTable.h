#include <endian.h>
#include <iostream>
#include <math.h>

const float epsilon = 0.000001;
const int goal = 100;

class ProbabilityTable {
private:
  float P[goal][goal][goal];
  float old[goal][goal][goal];

  float max(float a, float b);
  float lookup(float P[goal][goal][goal], int i, int j, int k);

public:
  ProbabilityTable();
  float getProbability(int i, int j, int k);
  bool getOptimalChoice(int i, int j, int k); // 1 is flip, 0 is hold
};

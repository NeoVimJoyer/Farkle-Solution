#include <endian.h>
#include <math.h>
#include "ProbabilityTable.h"

float ProbabilityTable::getProbability(int i, int j, int k) {
  return P[i][j][k];
}

bool ProbabilityTable::getOptimalChoice(int i, int j, int k) {
  float PHold = (i + k >= goal) ? 1.0 : 1 - P[j][i+k][0];
  float PFlip = (
    (1 - P[j][i][0])
    + lookup(P, i, j, k + 2)
    + lookup(P, i, j, k + 3)
    + lookup(P, i, j, k + 4)
    + lookup(P, i, j, k + 5)
    + lookup(P, i, j, k + 6)
  ) / 6;
  
  if(PHold > PFlip)
    return false;
  else
    return true;
}

float ProbabilityTable::max(float a, float b) {
  if(a >= b)
    return a;
  else
    return b;
}

float ProbabilityTable::lookup(float P[goal][goal][goal], int i, int j, int k) {
  if(i + k >= goal) return 1.0;
  return P[i][j][k];
}

ProbabilityTable::ProbabilityTable() {
  // Every possability  i, j, k  and set it at 0.5(this is arbitrary)
  for(int i = 0; i < goal; i++)
    for(int j = 0; j < goal; j++)
      for(int k = 0; k < goal; k++)
        P[i][j][k] = 0.5;

  int partitionTotal = (goal - 1) * 2;
  while(partitionTotal >= 0) {
    while(1) {
      float maxChange = 0;

      for(int i = 0; i < goal; i++)
        for(int j = 0; j < goal; j++)
          for(int k = 0; k < goal; k++)
            old[i][j][k] = P[i][j][k];

      for(int i = 0; i < goal; i++)
        for(int j = 0; j < goal; j++)
          if(i + j == partitionTotal)
            for(int k = 0; k < goal; k++) {
              float PHold = (i + k >= goal) ? 1.0 : 1 - P[j][i+k][0];
              float PFlip = (
                (1 - P[j][i][0])
                + lookup(P, i, j, k + 2)
                + lookup(P, i, j, k + 3)
                + lookup(P, i, j, k + 4)
                + lookup(P, i, j, k + 5)
                + lookup(P, i, j, k + 6)
              ) / 6;
              P[i][j][k] = max(PHold, PFlip);
            }

      for(int i = 0; i < goal; i++)
        for(int j = 0; j < goal; j++)
          if(i + j == partitionTotal)
            for(int k = 0; k < goal; k++)
              if(std::fabs(old[i][j][k] - P[i][j][k]) > maxChange)
                maxChange = std::fabs(old[i][j][k] - P[i][j][k]);
      if(maxChange <= epsilon)
        break;
    }
    --partitionTotal;
  }
}

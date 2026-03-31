#include <endian.h>
#include <math.h>
#include "ProbabilityTable.h"
#include <vector>

float ProbabilityTable::getProbability(int i, int j, int k, int d) {
  return P[i][j][k][d];
}

bool ProbabilityTable::getOptimalChoice(int i, int j, int k, int d) {
  float PHold = (i + k >= goal / 50) ? 1.0 : 1 - P[j][i+k][0][diceNum - 1];
  float PRoll = getRollPossabilities(P, i, j, k, d);
  
  if(PHold > PRoll)
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

float ProbabilityTable::lookup(float P[goal / 50][goal / 50][goal / 50 ][diceNum], int i, int j, int k, int d) {
  d = normalize(d);
  if(i + k >= goal / 50) return 1.0;
  return P[i][j][k][d];
}

// This is to reset the dice back to 3 when the one is rolled 
int ProbabilityTable::normalize(int num) {
  if(num < 0)
    return 2;
  return num;
}

struct roll ProbabilityTable::scoreRoll(std::vector<int> pastRolls) {
  struct roll toReturn;

  // Check for three of a kind
  if(pastRolls.size() == 3 && pastRolls[0] == pastRolls[1] && pastRolls[1] == pastRolls[2]) {
    if(pastRolls[0] == 1)
      toReturn.roll = 1000;
    else
      toReturn.roll = pastRolls[0] * 100;

    toReturn.diceNum = 3;
    return toReturn;
  }

  for(int i = 0; i < pastRolls.size(); i++)
    if(pastRolls[i] == 1) {
      toReturn.roll += 100;
      ++toReturn.diceNum;
    } else if(pastRolls[i] == 5) {
      toReturn.roll += 50;
      ++toReturn.diceNum;
    }
  return toReturn;
}

void ProbabilityTable::getRoll(std::vector<struct roll>& rollValues, std::vector<int>& pastRolls, int d) {
  if(d == 0) {
    for(int i = 1; i <= 6; i++) {
      pastRolls.push_back(i);
      struct roll toAdd = scoreRoll(pastRolls); toAdd.roll /= 50;
      rollValues.push_back(toAdd);
      pastRolls.pop_back();
    }
  } else {
    for(int i = 1; i <= 6; i++) {
      pastRolls.push_back(i);
      getRoll(rollValues, pastRolls, d - 1);
      pastRolls.pop_back();
    }
  }
}

float ProbabilityTable::getRollPossabilities(float P[goal / 50][goal / 50][goal / 50 ][diceNum], int i, int j, int k, int d) {
  int possabilityCount = pow(6, d + 1);
  std::vector<struct roll> rollValues;
  std::vector<int> pastRolls;
  getRoll(rollValues, pastRolls, d);
  
  float total = 0;
  for(int f = 0; f < rollValues.size(); f++) {
    if(rollValues[f].roll == 0)
      total += 1 - P[j][i][0][diceNum - 1];
    else
      total += lookup(P, i, j, k + rollValues[f].roll, normalize(d - rollValues[f].diceNum));
  }
  return total / possabilityCount;
}

ProbabilityTable::ProbabilityTable() {
  // Every possability  i, j, k  and set it at 0.5(this is arbitrary)
  for(int i = 0; i < goal / 50; i++)
    for(int j = 0; j < goal / 50; j++)
      for(int k = 0; k < goal / 50; k++)
        for(int d = 0; d < diceNum; d++)
          P[i][j][k][d] = 0.5;

  int partitionTotal = (goal / 50 - 1) * 2;
  while(partitionTotal >= 0) {
    while(1) {
      float maxChange = 0;

      for(int i = 0; i < goal / 50; i++)
        for(int j = 0; j < goal / 50; j++)
          if(i + j == partitionTotal)
            for(int k = 0; k < goal / 50; k++)
              for(int d = 0; d < diceNum; d++)
                old[i][j][k][d] = P[i][j][k][d];

      for(int i = 0; i < goal / 50; i++)
        for(int j = 0; j < goal / 50; j++)
          if(i + j == partitionTotal)
            for(int k = 0; k < goal / 50; k++)
              for(int d = 0; d < diceNum; d++){
                float PHold = (i + k >= goal / 50) ? 1.0 : 1 - P[j][i+k][0][2];
                float PRoll = getRollPossabilities(P, i, j, k, d);
                P[i][j][k][d] = max(PHold, PRoll);
              }

      for(int i = 0; i < goal / 50; i++)
        for(int j = 0; j < goal / 50; j++)
          if(i + j == partitionTotal)
            for(int k = 0; k < goal / 50; k++)
              for(int d = 0; d < diceNum; d++)
                if(std::fabs(old[i][j][k][d] - P[i][j][k][d]) > maxChange)
                  maxChange = std::fabs(old[i][j][k][d] - P[i][j][k][d]);
      if(maxChange <= epsilon)
        break;
    }
    --partitionTotal;
  }
}

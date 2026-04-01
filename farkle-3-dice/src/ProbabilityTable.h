#include <endian.h>
#include <iostream>
#include <math.h>
#include <vector>

const float epsilon = 0.000001;
const int goal = 10000; // Note that the arrays will count on intervals of 50 because you will never have a score in between multiples of 50
const int diceNum = 3;

struct roll {
  int roll = 0;
  int diceNum = 0;
};

class ProbabilityTable {
private:
  float P[goal / 50][goal / 50][goal / 50][diceNum];
  float old[goal / 50][goal / 50][goal / 50][diceNum];

  // Precomputed roll outcomes
  std::vector<struct roll> rollOutcomes[diceNum];

  // These are the scores for
  // Waste index 0 as all the functions use 1-7
  struct roll oneDie[7];
  struct roll twoDice[7][7];
  struct roll threeDice[7][7][7];
  struct roll getRollScore(std::vector<int>& pastRolls);
  struct roll scoreRoll(std::vector<int> pastRolls);

  float max(float a, float b);
  float lookup(float P[goal / 50][goal / 50][goal / 50][diceNum], int i, int j, int k, int d);
  int normalize(int num);

  void getRoll(std::vector<struct roll>& rollValues, std::vector<int>& pastRolls, int d);
  float getRollPossabilities(float P[goal / 50][goal / 50][goal / 50][diceNum], int i, int j, int k, int d);

public:
  ProbabilityTable();
  float getProbability(int i, int j, int k, int d);
  bool getOptimalChoice(int i, int j, int k, int d); // 1 is flip, 0 is hold
};

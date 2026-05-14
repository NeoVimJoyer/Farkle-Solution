#include <endian.h>
#include <iostream>
#include <math.h>
#include <vector>

const float epsilon = 0.0001;
const int goal = 10000; // Note that the arrays will count on intervals of 50 because you will never have a score in between multiples of 50
const int diceNum = 6;

struct roll {
  int roll = 0;
  int diceNum = 0;
  bool operator==(const struct roll& other) const {
    return roll == other.roll && diceNum == other.diceNum;
  }
};

struct PossibleRoll {
  int combinationsPossible = 1;
  std::vector<struct roll> choices;
};

class ProbabilityTable {
private:
  float P[goal / 50][goal / 50][goal / 50][diceNum];
  float old[goal / 50][goal / 50][goal / 50][diceNum];

  // These are the scores for each roll
  // Waste index 0 as all the functions use 1-7
  std::vector<struct roll> oneDieArr[7];
  std::vector<struct roll> twoDiceArr[7][7];
  std::vector<struct roll> threeDiceArr[7][7][7];
  std::vector<struct roll> fourDiceArr[7][7][7][7];
  std::vector<struct roll> fiveDiceArr[7][7][7][7][7];
  std::vector<struct roll> sixDiceArr[7][7][7][7][7][7];

  std::vector<struct PossibleRoll> oneDie;
  std::vector<struct PossibleRoll> twoDie;
  std::vector<struct PossibleRoll> threeDie;
  std::vector<struct PossibleRoll> fourDie;
  std::vector<struct PossibleRoll> fiveDie;
  std::vector<struct PossibleRoll> sixDie;

  std::vector<struct roll> getRollChoices(std::vector<int> pastRolls);

  float max(float a, float b);
  float lookup(float P[goal / 50][goal / 50][goal / 50][diceNum], int i, int j, int k, int d);
  int normalize(int num);
  int normalizeToOne(int num);

  float getRollPossabilities(float P[goal / 50][goal / 50][goal / 50][diceNum], int i, int j, int k, int d);

public:
  ProbabilityTable();
  float getProbability(int i, int j, int k, int d);
  bool getOptimalChoice(int i, int j, int k, int d); // 1 is flip, 0 is hold
};

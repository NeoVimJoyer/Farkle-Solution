#include <endian.h>
#include <iostream>
#include <math.h>
#include <vector>

const float epsilon = 0.0001;
const int error = 8000; // this is a reasonable place where there is very little chance it gets there
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

class ChanceTable {
private:
  int goal;

  float P[error / 50][diceNum];
  float old[error / 50][diceNum];

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

  float lookup(float P[error / 50][diceNum], int k, int d);
  int normalize(int num);
  float getRollPossabilities(float P[error / 50][diceNum], int k, int d);

public:
  ChanceTable(int goalPar);
  float getProbability(int startingDice);
};

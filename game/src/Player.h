#include <vector>
#include <string>

#ifndef PLAYER_H 
#define PLAYER_H

int const goal = 10000;
int const error = 8000; // 8000 was reasoned a good cuttoff where there is almost no chance a player could score more than that
int const diceNum = 6;

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

class Player {
private:
  float accuracyDeviance;
  float turnsTaken;

  void addDevianceHOrR(int i, int j, int k, int d, char choice);
  void addDevianceRolled(std::vector<struct roll> choices, int i, int j, int k, int d, int choice);

  float max(float num1, float num2);
  float min(float num1, float num2);

protected:
  int score;

  float C[error / 50][diceNum];
  void getBinChance();

  float P[(goal + error) / 50][(goal + error) / 50][error / 50][diceNum];
  void getBin();

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

  float lookupArchived(float P[(goal + error) / 50][(goal + error) / 50][error / 50][diceNum], int i, int j, int k, int d);
  float lookupChance(int k, int d);
  float getRollPossabilities(float P[(goal + error) / 50][(goal + error) / 50][error / 50][diceNum], int i, int j, int k, int d);
  bool getOptimalChoice(int i, int j, int k, int d);

  int normalize(int num); // To reset the dice num to six if they are all used
  std::vector<struct roll> getRollChoices(std::vector<int> rolls);

public:
  std::string divider = "--------------------------------------------------";

  int getScore();
  void takeTurn(const int opponentScore);
  float getAverageAccuracyDeviance();
  Player();
};

#endif

#include "AI.h"
#include <vector>

float AI::getAverageAccuracyDeviance() {
  return 0.0;
}

void AI::takeTurn(const int opponentScore) {
  int turnTotal = 0;
  int dice = diceNum;

  while(1) {
    bool choice = getOptimalChoice(score / 50, opponentScore / 50, turnTotal / 50, dice - 1);
    
    if(choice) {
      std::vector<int> rolls;
      for(int i = 0; i < dice; i++)
        rolls.push_back(std::rand() % 6 + 1);

      std::vector<struct roll> choices = getRollChoices(rolls);
      if(choices.empty())
        break;
      else {
        float max = 0.0;
        int choiceNum = 0;
        for(int i = 0; i < choices.size(); i++) {
          if(lookupArchived(P, score / 50, opponentScore / 50, turnTotal / 50 + choices[i].roll, normalize(dice - choices[i].diceNum) - 1) > max) {
            max = lookupArchived(P, score / 50, opponentScore / 50, turnTotal / 50 + choices[i].roll, normalize(dice - choices[i].diceNum) - 1);
            choiceNum = i;
          }
        }

        turnTotal += choices[choiceNum].roll * 50;
        dice = normalize(dice - choices[choiceNum].diceNum);
      }
    } else {
      score += turnTotal;
      break;
    }
  }
}

AI::AI() {
  getBin();
  getBinChance();

  // First get all the roll scores
  std::vector<int> pastRolls;
  for(int i = 1; i <= 6; i++) {
    pastRolls.push_back(i);
    oneDieArr[i] = getRollChoices(pastRolls);
    pastRolls.pop_back();
  }

  for(int i = 1; i <= 6; i++) {
    pastRolls.push_back(i);
    for(int j = 1; j <= 6; j++) {
      pastRolls.push_back(j);
      twoDiceArr[i][j] = getRollChoices(pastRolls);
      pastRolls.pop_back();
    }
    pastRolls.pop_back();
  }

  for(int i = 1; i <= 6; i++) {
    pastRolls.push_back(i);
    for(int j = 1; j <= 6; j++) {
      pastRolls.push_back(j);
      for(int k = 1; k <= 6; k++) {
        pastRolls.push_back(k);
        threeDiceArr[i][j][k] = getRollChoices(pastRolls);
        pastRolls.pop_back();
      }
      pastRolls.pop_back();
    }
    pastRolls.pop_back();
  }

  for(int i = 1; i <= 6; i++) {
    pastRolls.push_back(i);
    for(int j = 1; j <= 6; j++) {
      pastRolls.push_back(j);
      for(int k = 1; k <= 6; k++) {
        pastRolls.push_back(k);
        for(int l = 1; l <= 6; l++) {
          pastRolls.push_back(l);
          fourDiceArr[i][j][k][l] = getRollChoices(pastRolls);
          pastRolls.pop_back();
        }
        pastRolls.pop_back();
      }
      pastRolls.pop_back();
    }
    pastRolls.pop_back();
  }

  for(int i = 1; i <= 6; i++) {
    pastRolls.push_back(i);
    for(int j = 1; j <= 6; j++) {
      pastRolls.push_back(j);
      for(int k = 1; k <= 6; k++) {
        pastRolls.push_back(k);
        for(int l = 1; l <= 6; l++) {
          pastRolls.push_back(l);
          for(int m = 1; m <= 6; m++) {
            pastRolls.push_back(m);
            fiveDiceArr[i][j][k][l][m] = getRollChoices(pastRolls);
            pastRolls.pop_back();
          }
          pastRolls.pop_back();
        }
        pastRolls.pop_back();
      }
      pastRolls.pop_back();
    }
    pastRolls.pop_back();
  }

  for(int i = 1; i <= 6; i++) {
    pastRolls.push_back(i);
    for(int j = 1; j <= 6; j++) {
      pastRolls.push_back(j);
      for(int k = 1; k <= 6; k++) {
        pastRolls.push_back(k);
        for(int l = 1; l <= 6; l++) {
          pastRolls.push_back(l);
          for(int m = 1; m <= 6; m++) {
            pastRolls.push_back(m);
            for(int n = 1; n <= 6; n++) {
              pastRolls.push_back(n);
              sixDiceArr[i][j][k][l][m][n] = getRollChoices(pastRolls);
              pastRolls.pop_back();
            }
            pastRolls.pop_back();
          }
          pastRolls.pop_back();
        }
        pastRolls.pop_back();
      }
      pastRolls.pop_back();
    }
    pastRolls.pop_back();
  }

  // After you get those arrays, condense them 
  for(int i = 1; i <= 6; i++) {
    bool inVec = false;
    for(int a = 0; a < oneDie.size(); a++)
      if(oneDieArr[i] == oneDie[a].choices) {
        ++oneDie[a].combinationsPossible;
        inVec = true;
        break;
      }
    if(!inVec) {
      struct PossibleRoll toAdd;
      toAdd.choices = oneDieArr[i];
      oneDie.push_back(toAdd);
    }
  }

  for(int i = 1; i <= 6; i++) 
    for(int j = 1; j <= 6; j++) {
      bool inVec = false;
      for(int a = 0; a < twoDie.size(); a++)
        if(twoDiceArr[i][j] == twoDie[a].choices) {
          ++twoDie[a].combinationsPossible;
          inVec = true;
          break;
        }
      if(!inVec) {
        struct PossibleRoll toAdd;
        toAdd.choices = twoDiceArr[i][j];
        twoDie.push_back(toAdd);
      }
    }

  for(int i = 1; i <= 6; i++) 
    for(int j = 1; j <= 6; j++) 
      for(int k = 1; k <= 6; k++) {
        bool inVec = false;
        for(int a = 0; a < threeDie.size(); a++)
          if(threeDiceArr[i][j][k] == threeDie[a].choices) {
            ++threeDie[a].combinationsPossible;
            inVec = true;
            break;
          }
        if(!inVec) {
          struct PossibleRoll toAdd;
          toAdd.choices = threeDiceArr[i][j][k];
          threeDie.push_back(toAdd);
        }
      }

  for(int i = 1; i <= 6; i++) 
    for(int j = 1; j <= 6; j++) 
      for(int k = 1; k <= 6; k++)
        for(int l = 1; l <= 6; l++) {
          bool inVec = false;
          for(int a = 0; a < fourDie.size(); a++)
            if(fourDiceArr[i][j][k][l] == fourDie[a].choices) {
              ++fourDie[a].combinationsPossible;
              inVec = true;
              break;
            }
          if(!inVec) {
            struct PossibleRoll toAdd;
            toAdd.choices = fourDiceArr[i][j][k][l];
            fourDie.push_back(toAdd);
          }
        }

  for(int i = 1; i <= 6; i++) 
    for(int j = 1; j <= 6; j++) 
      for(int k = 1; k <= 6; k++)
        for(int l = 1; l <= 6; l++)
          for(int m = 1; m <= 6; m++) {
            bool inVec = false;
            for(int a = 0; a < fiveDie.size(); a++)
              if(fiveDiceArr[i][j][k][l][m] == fiveDie[a].choices) {
                ++fiveDie[a].combinationsPossible;
                inVec = true;
                break;
              }
            if(!inVec) {
              struct PossibleRoll toAdd;
              toAdd.choices = fiveDiceArr[i][j][k][l][m];
              fiveDie.push_back(toAdd);
            }
          }

  for(int i = 1; i <= 6; i++) 
    for(int j = 1; j <= 6; j++) 
      for(int k = 1; k <= 6; k++)
        for(int l = 1; l <= 6; l++)
          for(int m = 1; m <= 6; m++)
            for(int n = 1; n <= 6; n++) {
              bool inVec = false;
              for(int a = 0; a < sixDie.size(); a++)
                if(sixDiceArr[i][j][k][l][m][n] == sixDie[a].choices) {
                  ++sixDie[a].combinationsPossible;
                  inVec = true;
                  break;
                }
              if(!inVec) {
                struct PossibleRoll toAdd;
                toAdd.choices = sixDiceArr[i][j][k][l][m][n];
                sixDie.push_back(toAdd);
              }
            }
}

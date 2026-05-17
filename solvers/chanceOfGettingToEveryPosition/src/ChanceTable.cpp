#include <endian.h>
#include <math.h>
#include "ChanceTable.h"
#include <vector>

float ChanceTable::getProbability(int startingDice) {
  return P[0][startingDice - 1]; // This just gives the probability of reaching the goal with a given starting amount of dice
}

float ChanceTable::lookup(float P[error / 50][diceNum], int k, int d) {
  d = normalize(d);
  if(k >= goal / 50) return 1.0;
  return P[k][d];
}

// This is to reset the dice back to 3 when the one is rolled 
int ChanceTable::normalize(int num) {
  if(num < 0)
    return diceNum - 1;
  return num;
}

std::vector<struct roll> ChanceTable::getRollChoices(std::vector<int> pastRolls) {
  std::vector<struct roll> choices;

  // First get the counts for all dice
  int rollcounts[diceNum];
  for(int i = 0 ; i < diceNum; i++)
    rollcounts[i] = 0;

  for(int i = 0; i < pastRolls.size(); i++) {
    if(pastRolls[i] == 1)
      ++rollcounts[0];
    else if(pastRolls[i] == 2)
      ++rollcounts[1];
    else if(pastRolls[i] == 3)
      ++rollcounts[2];
    else if(pastRolls[i] == 4)
      ++rollcounts[3];
    else if(pastRolls[i] == 5)
      ++rollcounts[4];
    else if(pastRolls[i] == 6)
      ++rollcounts[5];
  }

  if(pastRolls.size() == 6) {
    // Check for three doubles
    int doubleCount = 0;
    for(int i = 0; i < 6; i++)
      if(rollcounts[i] == 2)
        ++doubleCount;
    if(doubleCount == 3) {
      struct roll toAdd;
      toAdd.roll = 750 / 50;
      toAdd.diceNum = 6;
      choices.push_back(toAdd);
    }
  }

  // Then check for all the multiples
  for(int i = 0; i < 6; i++) {
    for(int j = 3; j <= 6; j++) {
      if(i == 0) {
        if(rollcounts[i] >= j) {
          struct roll toAdd;

          toAdd.roll = (1000 * (1 << (j - 3))) / 50;
          toAdd.diceNum = j;

          for(int b = 0; b <= rollcounts[4]; b++) {
            toAdd.roll += b;
            toAdd.diceNum += b;

            choices.push_back(toAdd);

            toAdd.roll -= b;
            toAdd.diceNum -= b;
          }
        }
      } else if(i == 4) {
        if(rollcounts[i] >= j) {
          struct roll toAdd;

          toAdd.roll = (500 * (1 << (j - 3))) / 50;
          toAdd.diceNum = j;

          for(int a = 0; a <= rollcounts[0]; a++) {
            toAdd.roll += a * 2;
            toAdd.diceNum += a;

            choices.push_back(toAdd);

            toAdd.roll -= a * 2;
            toAdd.diceNum -= a;
          }
        }
      } else {
        if(rollcounts[i] >= j) {
          struct roll toAdd;

          toAdd.roll = (((i + 1) * 100) * (1 << (j - 3))) / 50;
          toAdd.diceNum = j;

          for(int a = 0; a <= rollcounts[0]; a++) {
            for(int b = 0; b <= rollcounts[4]; b++) {
              toAdd.roll += a * 2 + b;
              toAdd.diceNum += a + b;

              choices.push_back(toAdd);

              toAdd.roll -= a * 2 + b;
              toAdd.diceNum -= a + b;
            }
          }
        }
      }
    }
  }

  // then get all the possible basic scores
  for(int a = 0; a <= rollcounts[0]; a++) {
    for(int b = 0; b <= rollcounts[4]; b++) {
      if(a == 0 && b == 0) continue;
      struct roll toAdd;
      toAdd.roll = a * 2 + b;
      toAdd.diceNum = a + b;
      choices.push_back(toAdd);
    }
  }

  return choices;
}

// This just gets the average probability of winning if you roll
float ChanceTable::getRollPossabilities(float P[error / 50][diceNum], int k, int d) {
  float probabilitiesAddedUp = 0;
  float divideBy = 0;

  if(d == 0) {
    for(int a = 0; a < oneDie.size(); a++) {
      divideBy += oneDie[a].combinationsPossible;

      if(oneDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < oneDie[a].choices.size(); b++) {
          float p = lookup(P, k + oneDie[a].choices[b].roll, d - oneDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * oneDie[a].combinationsPossible;
      }
    }
  } else if(d == 1) {
    for(int a = 0; a < twoDie.size(); a++) {
      divideBy += twoDie[a].combinationsPossible;
      
      if(twoDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < twoDie[a].choices.size(); b++) {
          float p = lookup(P, k + twoDie[a].choices[b].roll, d - twoDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * twoDie[a].combinationsPossible;
      }
    }
  } else if(d == 2) {
    for(int a = 0; a < threeDie.size(); a++) {
      divideBy += threeDie[a].combinationsPossible;
      if(threeDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < threeDie[a].choices.size(); b++) {
          float p = lookup(P, k + threeDie[a].choices[b].roll, d - threeDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * threeDie[a].combinationsPossible;
      }
    }
  } else if(d == 3) {
    for(int a = 0; a < fourDie.size(); a++) {
      divideBy += fourDie[a].combinationsPossible;
      if(fourDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < fourDie[a].choices.size(); b++) {
          float p = lookup(P, k + fourDie[a].choices[b].roll, d - fourDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * fourDie[a].combinationsPossible;
      }
    }
  } else if(d == 4) {
    for(int a = 0; a < fiveDie.size(); a++) {
      divideBy += fiveDie[a].combinationsPossible;
      if(fiveDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < fiveDie[a].choices.size(); b++) {
          float p = lookup(P, k + fiveDie[a].choices[b].roll, d - fiveDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * fiveDie[a].combinationsPossible;
      }
    }
  } else if(d == 5) {
    for(int a = 0; a < sixDie.size(); a++) {
      divideBy += sixDie[a].combinationsPossible;
      if(sixDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < sixDie[a].choices.size(); b++) {
          float p = lookup(P, k + sixDie[a].choices[b].roll, d - sixDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * sixDie[a].combinationsPossible;
      }
    }
  }

  return probabilitiesAddedUp / divideBy;
}

ChanceTable::ChanceTable(int goalPar) {
  goal = goalPar;

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

  // Every possability i, j, k  and set it at 0.5(this is arbitrary)
  for(int k = 0; k < error / 50; k++)
      for(int d = 0; d < diceNum; d++)
        P[k][d] = 0.5;

  int partitionTotal = (error / 50 - 1) * 2;
  while(partitionTotal >= 0) {
    while(1) {
      float maxChange = 0;

      for(int k = 0; k < error / 50; k++)
        for(int d = 0; d < diceNum; d++)
            old[k][d] = P[k][d];

      for(int k = 0; k < error / 50; k++)
        for(int d = 0; d < diceNum; d++)
          P[k][d] = getRollPossabilities(P, k, d);

      for(int k = 0; k < error / 50; k++)
        for(int d = 0; d < diceNum; d++) {
          float change = std::fabs(old[k][d] - P[k][d]);
          if(change > maxChange)
            maxChange = change;
        }
      if(maxChange <= epsilon)
        break;
    }
    --partitionTotal;
  }
}

#include "Player.h"
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>

const uint32_t size = ((goal + error) / 50) * ((goal + error) / 50) * (error / 50) * 6;

void Player::getBin() {
  std::ifstream in("src/probtable.bin", std::ios::binary);
  in.seekg(32);
  in.read(reinterpret_cast<char*>(P), size * sizeof(float));
  in.close();
}


const uint32_t sizeChance = (error / 50) * 6;

void Player::getBinChance() {
  std::ifstream in("src/chancetable.bin", std::ios::binary);
  in.seekg(24);  // skip the header
  in.read(reinterpret_cast<char*>(C), sizeChance * sizeof(float));
  in.close();
}

float Player::lookupArchived(float P[(goal + error) / 50][(goal + error) / 50][error / 50][diceNum], int i, int j, int k, int d) {
  if (d < 0) d = diceNum - 1;        // hot dice: all used, reset to 6 (zero-based index 5)
  if (i + k >= (goal + error) / 50) return 1.0;
  if(k >= error / 50) return P[i][j][error / 50 - 1][d];
  return P[i][j][k][d];
}

float Player::lookupChance(int k, int d) {
  if(k * 50 >= error) return 0.0;
  else if(k <= 0) return 1.0;
  return C[k][d];
}

// This just gets the average probability of winning if you roll
float Player::getRollPossabilities(float P[(goal + error) / 50][(goal + error) / 50][error / 50][diceNum], int i, int j, int k, int d) {
  float probabilitiesAddedUp = 0;
  float divideBy = 0;

  if(d == 0) {
    for(int a = 0; a < oneDie.size(); a++) {
      divideBy += oneDie[a].combinationsPossible;

      if(oneDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < oneDie[a].choices.size(); b++) {
          float p = lookupArchived(P, i, j, k + oneDie[a].choices[b].roll, d - oneDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * oneDie[a].combinationsPossible;
      } else
        probabilitiesAddedUp += (1 - lookupArchived(P, j, i, 0, diceNum - 1)) * oneDie[a].combinationsPossible;
    }
  } else if(d == 1) {
    for(int a = 0; a < twoDie.size(); a++) {
      divideBy += twoDie[a].combinationsPossible;
      
      if(twoDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < twoDie[a].choices.size(); b++) {
          float p = lookupArchived(P, i, j, k + twoDie[a].choices[b].roll, d - twoDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * twoDie[a].combinationsPossible;
      } else
        probabilitiesAddedUp += (1 - lookupArchived(P, j, i, 0, diceNum - 1)) * twoDie[a].combinationsPossible;
    }
  } else if(d == 2) {
    for(int a = 0; a < threeDie.size(); a++) {
      divideBy += threeDie[a].combinationsPossible;
      if(threeDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < threeDie[a].choices.size(); b++) {
          float p = lookupArchived(P, i, j, k + threeDie[a].choices[b].roll, d - threeDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * threeDie[a].combinationsPossible;
      } else
        probabilitiesAddedUp += (1 - lookupArchived(P, j, i, 0, diceNum - 1)) * threeDie[a].combinationsPossible;
    }
  } else if(d == 3) {
    for(int a = 0; a < fourDie.size(); a++) {
      divideBy += fourDie[a].combinationsPossible;
      if(fourDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < fourDie[a].choices.size(); b++) {
          float p = lookupArchived(P, i, j, k + fourDie[a].choices[b].roll, d - fourDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * fourDie[a].combinationsPossible;
      } else
        probabilitiesAddedUp += (1 - lookupArchived(P, j, i, 0, diceNum - 1)) * fourDie[a].combinationsPossible;
    }
  } else if(d == 4) {
    for(int a = 0; a < fiveDie.size(); a++) {
      divideBy += fiveDie[a].combinationsPossible;
      if(fiveDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < fiveDie[a].choices.size(); b++) {
          float p = lookupArchived(P, i, j, k + fiveDie[a].choices[b].roll, d - fiveDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * fiveDie[a].combinationsPossible;
      } else
        probabilitiesAddedUp += (1 - lookupArchived(P, j, i, 0, diceNum - 1)) * fiveDie[a].combinationsPossible;
    }
  } else if(d == 5) {
    for(int a = 0; a < sixDie.size(); a++) {
      divideBy += sixDie[a].combinationsPossible;
      if(sixDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < sixDie[a].choices.size(); b++) {
          float p = lookupArchived(P, i, j, k + sixDie[a].choices[b].roll, d - sixDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * sixDie[a].combinationsPossible;
      } else
        probabilitiesAddedUp += (1 - lookupArchived(P, j, i, 0, diceNum - 1)) * sixDie[a].combinationsPossible;
    }
  }

  return probabilitiesAddedUp / divideBy;
}

bool Player::getOptimalChoice(int i, int j, int k, int d) {
  float PHold;
  if(j >= goal / 50) {
    if (i + k > j) PHold = 1;
    else PHold = 0;
  } else {
    if(i + k >= (goal + error) / 50) PHold = 1;
    else if(i + k >= goal / 50) PHold = 1 - lookupChance((i + k - j) + 1, diceNum - 1); // get the opponents chance of passing you
    else PHold = 1 - P[j][i + k][0][diceNum - 1];
  }

  float PRoll;
  if(j >= goal / 50) {
    PRoll = lookupChance((j - i - k) + 1, d);
  } else
    PRoll = getRollPossabilities(P, i, j, k, d);
  
  if(PHold > PRoll)
    return false;
  else
    return true;
}

int Player::getScore() {
  return score;
}

std::vector<struct roll> Player::getRollChoices(std::vector<int> rolls) {
  std::vector<struct roll> choices;

  // First get the counts for all dice
  int rollcounts[diceNum];
  for(int i = 0 ; i < diceNum; i++)
    rollcounts[i] = 0;

  for(int i = 0; i < rolls.size(); i++) {
    if(rolls[i] == 1)
      ++rollcounts[0];
    else if(rolls[i] == 2)
      ++rollcounts[1];
    else if(rolls[i] == 3)
      ++rollcounts[2];
    else if(rolls[i] == 4)
      ++rollcounts[3];
    else if(rolls[i] == 5)
      ++rollcounts[4];
    else if(rolls[i] == 6)
      ++rollcounts[5];
  }

  if(rolls.size() == 6) {
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

int Player::normalize(int num) {
  if(num == 0) return diceNum;
  return num;
}

float Player::max(float num1, float num2) {
  if(num1 > num2) return num1;
  return num2;
}

float Player::min(float num1, float num2) {
  if(num1 < num2) return num1;
  return num2;
}

void Player::addDevianceHOrR(int i, int j, int k, int d, char choice) {
  float PHold;
  if(j >= goal / 50) {
    if (i + k > j) PHold = 1;
    else PHold = 0;
  } else {
    if(i + k >= (goal + error) / 50) PHold = 1;
    else if(i + k >= goal / 50) PHold = 1 - lookupChance((i + k - j) + 1, diceNum - 1); // get the opponents chance of passing you
    else PHold = 1 - P[j][i + k][0][diceNum - 1];
  }

  float PRoll;
  if(j >= goal / 50) {
    PRoll = lookupChance((j - i - k) + 1, d);
  } else
    PRoll = getRollPossabilities(P, i, j, k, d);

  if(!((PRoll > PHold && (choice == 'r' || choice == 'R')) || (PHold >= PRoll && (choice == 'h' || choice == 'H'))))
    accuracyDeviance += max(PRoll, PHold) - min(PRoll, PHold);
}

void Player::addDevianceRolled(std::vector<struct roll> choices, int i, int j, int k, int d, int choice) {
  float max = 0.0;
  int maxChoice = 0;

  for(int a = 0; a < choices.size(); a++)
    if(lookupArchived(P, i, j, k + choices[a].roll, normalize(d - choices[a].diceNum) - 1) > max) {
      max = lookupArchived(P, i, j, k + choices[a].roll, normalize(d - choices[a].diceNum) - 1);
      maxChoice = a;
    }

  if(!(choice == maxChoice))
    accuracyDeviance += max - lookupArchived(P, i, j, k + choices[choice].roll, normalize(d - choices[choice].diceNum) - 1);
}

float Player::getAverageAccuracyDeviance() {
  return accuracyDeviance / turnsTaken;
}

void Player::takeTurn(const int opponentScore) {
  ++turnsTaken;
  int turnTotal = 0;
  int dice = diceNum;

  while(1) {

    std::cout << '\n' << divider << "\n\n";
    std::cout << "Your score is: " << score << " | Your turn total is: " << turnTotal << " | Your dice left is: " << dice << " | Your opponents score is: " << opponentScore << "\n\n";

    char choice;
    while(1) {
      std::cout << "Enter if you want to (R)oll or (H)old: ";
      std::cin >> choice;

      if(choice == 'R' || choice == 'r' || choice == 'H' || choice == 'h')
        break;
      else
        std::cout << "Invalid input, try again\n";
    }

    addDevianceHOrR(score / 50, opponentScore / 50, turnTotal / 50, dice - 1, choice);
    
    if(choice == 'R' || choice == 'r') {
      std::vector<int> rolls;
      for(int i = 0; i < dice; i++)
        rolls.push_back(std::rand() % 6 + 1);

      std::vector<struct roll> choices = getRollChoices(rolls);

      std::cout << "Your roll is: ";
      for(int i = 0; i < dice; i++) std::cout << rolls[i] << ' ';
      std::cout << "\n\n";
      
      if(choices.size() > 0) {
        int scoreChoice;
        std::cout << "Your choices are: \n";
        for(int i = 0; i < choices.size(); i++)
          std::cout << i + 1 << " Score: " << choices[i].roll * 50 << " Dice used: " << choices[i].diceNum << '\n';
        while(1) {
          std::cout << "Enter your scoreChoice: "; std::cin >> scoreChoice; --scoreChoice; std::cout << '\n';
          if(scoreChoice >= 0 && scoreChoice < choices.size()) {
            addDevianceRolled(choices, score / 50, opponentScore / 50, turnTotal / 50, dice - 1, scoreChoice);
            break;
          } else
            std::cout << "Invalid choice, try again\n";
        }

        turnTotal += choices[scoreChoice].roll * 50;
        dice = normalize(dice - choices[scoreChoice].diceNum);
      } else {
        std::cout << "You farkled\n";
        break;
      }
    } else {
      score += turnTotal;
      break;
    }
  }
}

Player::Player() {
  score = 0;
  accuracyDeviance = 0.0;
  turnsTaken = 0.0;

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

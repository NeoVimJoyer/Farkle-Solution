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
    return diceNum - 1;
  return num;
}

std::vector<struct roll> ProbabilityTable::getRollChoices(std::vector<int> pastRolls) {
  std::vector<struct roll> choices;

  // First, check for three of a kind 
  if(pastRolls.size() == 3)
    if(pastRolls[0] == pastRolls[1] && pastRolls[1] == pastRolls[2]) {
      struct roll toAdd;
      
      if(pastRolls[0] == 1)
        toAdd.roll = 1000;
      else
        toAdd.roll = pastRolls[0] * 100;

      toAdd.roll /= 50;
      toAdd.diceNum = 3;

      choices.push_back(toAdd);
    }

  // Then check for scoring dice, and find the number of possabilities
  int oneCount = 0;
  int fiveCount = 0;

  for(int i = 0; i < pastRolls.size(); i++) {
    if(pastRolls[i] == 1)
      ++oneCount;
    else if(pastRolls[i] == 5)
      ++fiveCount;
  }

  for(int a = 0; a <= oneCount; a++) {
    for(int b = 0; b <= fiveCount; b++) {
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
float ProbabilityTable::getRollPossabilities(float P[goal / 50][goal / 50][goal / 50 ][diceNum], int i, int j, int k, int d) {
  float probabilitiesAddedUp = 0;
  float divideBy = 0;

  if(d == 0) {
    for(int a = 0; a < oneDie.size(); a++) {
      divideBy += oneDie[a].combinationsPossible;

      if(oneDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < oneDie[a].choices.size(); b++) {
          float p = lookup(P, i, j, k + oneDie[a].choices[b].roll, d - oneDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * oneDie[a].combinationsPossible;
      } else
        probabilitiesAddedUp += (1 - lookup(P, j, i, 0, diceNum - 1)) * oneDie[a].combinationsPossible;
    }
  } else if(d == 1) {
    for(int a = 0; a < twoDie.size(); a++) {
      divideBy += twoDie[a].combinationsPossible;
      
      if(twoDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < twoDie[a].choices.size(); b++) {
          float p = lookup(P, i, j, k + twoDie[a].choices[b].roll, d - twoDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * twoDie[a].combinationsPossible;
      } else
        probabilitiesAddedUp += (1 - lookup(P, j, i, 0, diceNum - 1)) * twoDie[a].combinationsPossible;
    }
  } else {
    for(int a = 0; a < threeDie.size(); a++) {
      divideBy += threeDie[a].combinationsPossible;
      if(threeDie[a].choices.size() > 0) {
        float maxProbability = 0;
        for(int b = 0; b < threeDie[a].choices.size(); b++) {
          float p = lookup(P, i, j, k + threeDie[a].choices[b].roll, d - threeDie[a].choices[b].diceNum);
          if(p > maxProbability)
            maxProbability = p;
        }
        probabilitiesAddedUp += maxProbability * threeDie[a].combinationsPossible;
      } else
        probabilitiesAddedUp += (1 - lookup(P, j, i, 0, diceNum - 1)) * threeDie[a].combinationsPossible;
    }
  }

  return probabilitiesAddedUp / divideBy;
}

ProbabilityTable::ProbabilityTable() {
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

  // Every possability i, j, k  and set it at 0.5(this is arbitrary)
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
              for(int d = 0; d < diceNum; d++) {
                float change = std::fabs(old[i][j][k][d] - P[i][j][k][d]);
                if(change > maxChange)
                  maxChange = change;
              }
      if(maxChange <= epsilon)
        break;
    }

    std::cout << partitionTotal * 50 << '\n';
    --partitionTotal;
  }
}

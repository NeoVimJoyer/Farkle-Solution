#include <iostream>
#include <fstream>
#include "ProbabilityTable.h"

// Declare this here so you dont put 8 MB on the stack
ProbabilityTable P;

std::ofstream output("output.txt");

int main() {
  for(int i = 0; i < goal / 50; i++)
    for(int j = 0; j < goal / 50; j++)
      for(int k = 0; k < goal / 50; k++)
        for(int d = 0; d < diceNum; d++) {
          output << '[' << i * 50 << ',' << j * 50 << ',' << k * 50 << ',' << d + 1 << ']' << " = ";
          bool move = P.getOptimalChoice(i, j, k, d);
          if(move)
            output << "roll";
          else
            output << "hold";
          output << '\n';
        }

  output.close();
  return 0;
}

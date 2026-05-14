#include <iostream>
#include <fstream>
#include "ProbabilityTable.h"

// Declare this here so you dont put 96 MB on the stack
ProbabilityTable P;

std::ofstream output("output.txt");

int main() {
  for(int i = 0; i < goal / 50; i++)
    for(int j = 0; j < goal / 50; j++)
      for(int k = 0; k < goal / 50; k++)
        for(int d = 0; d < diceNum; d++)
          output << '[' << i * 50 << ',' << j * 50 << ',' << k * 50 << ',' << d + 1 << ']' << " = " << P.getProbability(i, j, k, d) << '\n';

  output.close();
  return 0;
}

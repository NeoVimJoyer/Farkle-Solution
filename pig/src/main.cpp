#include <iostream>
#include <fstream>
#include "ProbabilityTable.h"

// Declare this here so you dont put 8 MB on the stack
ProbabilityTable P;

std::ofstream output("output.txt");

int main() {
  for(int i = 0; i < goal; i++)
    for(int j = 0; j < goal; j++)
      for(int k = 0; k < goal; k++) {
        output << '[' << i << ',' << j << ',' << k << ']' << " = ";
        bool move = P.getOptimalChoice(i, j, k);
        if(move)
          output << "flip";
        else
          output << "hold";
        output << '\n';
      }

  output.close();
  return 0;
}

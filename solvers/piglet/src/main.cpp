#include <endian.h>
#include <iostream>
#include <math.h>

const float epsilon = 0.000001;
const int goal = 2;

float max(float a, float b) {
  if(a >= b)
    return a;
  else
    return b;
}

int main() {
  // Every possability  i, j, k  and set it at 0.5(this is arbitrary)
  float P[goal][goal][goal];
  for(int i = 0; i < goal; i++)
    for(int j = 0; j < goal; j++)
      for(int k = 0; k < goal; k++)
        P[i][j][k] = 0.5;

  while(1) {
    float maxChange = 0.0;

    // Set old
    float old[goal][goal][goal];
    for(int i = 0; i < goal; i++)
      for(int j = 0; j < goal; j++)
        for(int k = 0; k < goal; k++)
          old[i][j][k] = P[i][j][k];

    for(int i = 0; i < goal; i++)
      for(int j = 0; j < goal; j++)
        for(int k = 0; k < goal; k++) {
          if(i + k >= goal)
            P[i][j][k] = 1;
          else {
            float p_hold = 1 - P[j][i + k][0];

            // This is to make sure k+1 isnt greater than 1
            float next = 0;
            if(i + k + 1 >= goal)
              next = 1;
            else
              next = P[i][j][k + 1];

            float p_flip = ((1 - P[j][i][0]) + next) / 2;
            P[i][j][k] = max(p_hold, p_flip);
          }
        }

    // Find maxChange
    for(int i = 0; i < goal; i++)
      for(int j = 0; j < goal; j++)
        for(int k = 0; k < goal; k++)
          if(std::fabs(old[i][j][k] - P[i][j][k]) > maxChange)
            maxChange = std::fabs(old[i][j][k] - P[i][j][k]);
    if(maxChange <= epsilon)
      break;
  }

  for(int i = 0; i < goal; i++)
    for(int j = 0; j < goal; j++)
      for(int k = 0; k < goal; k++)
        std::cout << '[' << i << ',' << j << ',' << k << ']' << " = " << P[i][j][k] << '\n';

  return 0;
}

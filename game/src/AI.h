#include "Player.h"

#ifndef AI_H 
#define AI_H 

class AI : public Player {
public:
  AI();
  void takeTurn(const int opponentScore);
  float getAverageAccuracyDeviance(); // This is just to override it because I don't want this to have the same one as the player without the variables to make it work
};

#endif

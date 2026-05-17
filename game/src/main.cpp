#include <iostream>
#include "AI.h"

// Declare them here. Don't try to make the stack deal with them
Player p;
AI a;

int main() {
  srand(time(0));

  char turn = 'p';
  while(1) {
    if(turn == 'p') {
      if(a.getScore() >= goal) {
        std::cout << '\n' << p.divider << '\n' << "\nYour opponent has reached " << goal << ". You have one turn to pass them\n";
        p.takeTurn(a.getScore());

        if(p.getScore() > a.getScore())
          std::cout << '\n' << p.divider << '\n' << "You won!\n";
        else if(p.getScore() < a.getScore())
          std::cout << '\n' << p.divider << '\n' << "Sorry, you lost\n";
        else
          std::cout << '\n' << p.divider << '\n' << "You tied";

        std::cout << "\n\n" << p.divider << '\n' << "Your were on average %" << p.getAverageAccuracyDeviance() * 100 << " off of the optimal line per turn\n\n";

        break;
      } else {
        p.takeTurn(a.getScore());
        turn = 'a';
      }
    } else {
      if(p.getScore() >= goal) {
        a.takeTurn(p.getScore());

        if(p.getScore() > a.getScore())
          std::cout << '\n' << p.divider << '\n' << "You won!\n";
        else if(p.getScore() < a.getScore())
          std::cout << '\n' << p.divider << '\n' << "Sorry, you lost\n";
        else
          std::cout << '\n' << p.divider << '\n' << "You tied";

        std::cout << "\n\n" << p.divider << '\n' << "Your were on average %" << p.getAverageAccuracyDeviance() * 100 << " off of the optimal line per turn\n\n";

        break;
      } else {
        a.takeTurn(p.getScore());
        turn = 'p';
      }
    }
  }

  return 0;
}

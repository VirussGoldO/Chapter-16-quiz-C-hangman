#include <iostream>
#include "Session.h"

int main()
{
	bool playAgain{ true };
	while (playAgain)
	{
		Settings::Difficulties d { Settings::selectGameDifficulty() };

		Session s{ d };
		s.displayIntro();
		s.displayState();

		while (s.getLives() > 0 && !s.testForWin())
		{
			char c{ s.getUserInput() };
			s.evaluateGuess(c);
			s.displayState();
		}

		playAgain = s.handleGameEnd();
	}

	std::cout << "Thank you for playing! :)\n";
	return 0;
}
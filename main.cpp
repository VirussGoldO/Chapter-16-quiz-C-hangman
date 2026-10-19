#include <iostream>
#include "Session.h"

int main()
{
	Session current_session{};
	current_session.displayIntro();
	current_session.displayState();

	while (current_session.getLives() > 0 && !current_session.testForWin())
	{
		current_session.evaluateGuess();
		current_session.displayState();
	}

	current_session.handleGameEnd();

	return 0;
}
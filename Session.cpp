#include "Session.h"
#include "Random.h"
#include "InputUtils.h"
#include <iostream>
#include <cctype>

// functions and constants for difficulty settings
namespace Settings
{
	int getStartingLives(Difficulties d)
	{
		switch (d)
		{
		case Difficulties::easy:	return 9;
		case Difficulties::medium:	return 6;
		case Difficulties::hard:	return 3;
		default: return defaultLives;
		}
	}

	// asks the user to choose the difficulty
	Difficulties selectGameDifficulty()
	{
		while (true)
		{
			std::cout << "Choose your difficulty: (0 for Easy, 1 for Medium, 2 for Hard)\n";

			int response{};
			std::cin >> response;

			// if bad input, ask again
			if (clearFailedExtraction())
			{
				std::cout << "That wasn't a valid input. Try again.\n";
				continue;
			}

			ignoreLine();

			// if out of range, ask again
			if (response < 0 || response > 2)
			{
				std::cout << "There's no such difficulty, sorry. Try again.\n";
				continue;
			}
			else
				return static_cast<Difficulties>(response);
		}
	}

}

// namespace with all the words
namespace WordList
{
	static const std::vector<std::string_view> easy_words{
		"apple", "bread", "chair", "dance", "earth",
		"house", "light", "mouse", "plant", "river",
		"smile", "table", "train", "water", "world",
	};

	static const std::vector<std::string_view> medium_words{
		"animal", "banana", "camera", "desert", "forest",
		"garden", "island", "jungle", "laptop", "museum",
		"planet", "puzzle", "rocket", "ticket", "window",
	};

	static const std::vector<std::string_view> hard_words{
		"avalanche", "blizzard", "chrysanthemum", "dwarf", "jazz",
		"labyrinth", "mystique", "pharaoh", "queue", "rhythm",
		"sphinx", "synchronize", "vocabulary", "xylophone", "zucchini",
	};

	// returns a random word from the list
	std::string_view getRandomWord(Settings::Difficulties d)
	{
		if (d == Settings::Difficulties::easy)
			return easy_words[Random::get<std::size_t>(0, easy_words.size() - 1)];
		if (d == Settings::Difficulties::medium)
			return medium_words[Random::get<std::size_t>(0, medium_words.size() - 1)];
		if (d == Settings::Difficulties::hard)
			return hard_words[Random::get<std::size_t>(0, hard_words.size() - 1)];
		else
			// just to stop the warning
			return medium_words[Random::get<std::size_t>(0, medium_words.size() - 1)];
	}
}




// constructor
Session::Session(Settings::Difficulties d)
	: m_livesLeft{ Settings::getStartingLives(d) }
	, m_randomWord{ WordList::getRandomWord(d) }
{}

// convert a letter to an index
std::size_t Session::toIndex(char c) const
{
	return static_cast<std::size_t>((c % 32) - 1);
}



// gets a guess letter from user
char Session::getUserInput()
{
	char input{};
	while (true)
	{
		std::cout << "Enter your next letter: ";
		std::cin >> input;

		// if bad input of not letter, ask again
		if (clearFailedExtraction() || !std::isalpha(input))
		{
			ignoreLine();
			std::cout << "That wasn't a valid input. Try again.\n";
			continue;
		}
		
		// if already guessed, ask again
		std::size_t index{ toIndex(input)};
		if (m_guessedLetters[index])
		{
			ignoreLine();
			std::cout << "You already guessed that. Try again.\n";
			continue;
		}
		else
		{
			ignoreLine();
			std::cout << '\n';
			m_guessedLetters[index] = 1;
			return static_cast<char>(std::tolower(input));
			// returns lower-case letter
		}
	}
}


// return true if the letter is in the random word
bool Session::isLetterInWord(char c) const
{
	for (char ch : m_randomWord)
	{
		if (ch == c)
			return true;
	}
	return false;
}

// returns true if the letter has been guessed
bool Session::isLetterGuessed(char c) const
{
	return m_guessedLetters[toIndex(c)];
}


// tests the guess against the generated word and removes a life if its wrong
void Session::evaluateGuess(char c)
{
		if (!isLetterInWord(c))
			--m_livesLeft;
}


// just displays the game stats
void Session::displayState() const
{
	std::cout << "The word: ";
	for (char c : m_randomWord)
	{
		if (isLetterGuessed(c))
			std::cout << c;
		else
			std::cout << "_";
	}
	std::cout << "\tWrong guesses: ";
	for (int i{0}; i < m_livesLeft; ++i)
		std::cout << '+';

	// print wrong gussed letters
	for (char c{'a'}; c <= 'z'; ++c)
		if (isLetterGuessed(c) && !isLetterInWord(c))
			std::cout << c;

	std::cout << '\n';
}

// modular functions if needed to change some text displays
void Session::displayIntro() const
{
	std::cout << "Welcome to C++man (a variant of Hangman)\n";
	std::cout << "To win: guess the word.	To lose: run out of pluses.\n\n";
}
void Session::displayWinMessage() const
{
	std::cout << "Congrats! You won!\n";
}
void Session::displayLossMessage() const
{
	std::cout << "You lost! The word was: " << m_randomWord << '\n';
}


// returns the number of lives left
int Session::getLives() const
{
	return m_livesLeft;
}

// tests if the user guessed the word
bool Session::testForWin() const
{
	for (char c : m_randomWord)
	{
		if (!isLetterGuessed(c))
			return false;
	}
		
	return true;
}

// modular function to handle the game end
bool Session::handleGameEnd() const
{
	if (testForWin())
		displayWinMessage();
	else
		displayLossMessage();

	std::cout << '\n';
	
	while (true)
	{
		std::cout << "Do you want another round? (enter y/n)";
		char response{};
		std::cin >> response;

		// if bad input, ask again
		if (clearFailedExtraction())
		{
			ignoreLine();
			std::cout << "That wasn't a valid input. Try again.\n";
			continue;
		}
		else
		{
			ignoreLine();
			if (response == 'y' || response == 'Y')
				return true;
			if (response == 'n' || response == 'N')
				return false;
			else
			{
				std::cout << "That wans't a valid response. Try again.\n";
				continue;
			}
		}
	}
}

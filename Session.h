#pragma once
#include <vector>
#include <string_view>

namespace Settings
{
	enum class Difficulties
	{
		easy,	// 0
		medium, // 1
		hard,	// 2
	};

	constexpr int defaultLives{ 6 };
	constexpr Difficulties defaultDifficulty{ Difficulties::medium };
	Difficulties selectGameDifficulty();
	int getStartingLives(Difficulties d);
}

namespace WordList
{
	std::string_view getRandomWord(Settings::Difficulties d = Settings::defaultDifficulty);
}

class Session
{
public:
	Session() = default;
	Session(Settings::Difficulties);

	// just dispaly functions
	void displayIntro() const;
	void displayWinMessage() const;
	void displayLossMessage() const;
	//
	char getUserInput();				// gets a letter from user
	void displayState() const;			// displays game state
	void evaluateGuess(char c);			// removes a life is a guess is wrong
	int getLives() const;	// returns number of lives left
	bool testForWin() const;			// tests if the user guessed the entire word
	bool handleGameEnd() const;			// prints game end message
	
private:
	const std::string_view m_randomWord{ WordList::getRandomWord( Settings::defaultDifficulty ) };
	std::vector<bool> m_guessedLetters{ std::vector<bool>(26) };
	int m_livesLeft{ Settings::defaultLives };
	std::size_t toIndex(char c) const;

	bool isLetterInWord(char c) const;	// looks if a letter is inside the generated word
	bool isLetterGuessed(char c) const;	// looks if a letter is guessed correctly
};
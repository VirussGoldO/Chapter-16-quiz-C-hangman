#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <deque>

namespace WordList
{
	std::string_view getRandomWord();
}

class Session
{
public:
	Session() = default;
	Session(int lives);

	void displayIntro() const;
	void displayWinMessage() const;
	void displayLossMessage() const;
	void displayState() const;
	void evaluateGuess();
	int getLives() const;
	bool testForWin() const;
	void handleGameEnd() const;

private:
	const std::string m_randomWord{ WordList::getRandomWord() };
	std::vector<bool> m_guessWord{ std::vector<bool>(m_randomWord.size())};
	std::vector<bool> m_guessedLetters{ std::vector<bool>(26) };
	int m_lives{ 6 };
	std::deque<char> m_wrongGuesses{ std::deque<char>(static_cast<std::size_t>(m_lives), '+')};

	char getUserInput();
};
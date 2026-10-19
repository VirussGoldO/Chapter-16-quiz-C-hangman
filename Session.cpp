#include "Session.h"
#include "Random.h"
#include "InputUtils.h"
#include <iostream>
#include <cctype>

namespace WordList
{
	static const std::vector<std::string_view> words{
		"mystery",
		"broccoli",
		"account",
		"almost",
		"spaghetti",
		"opinion",
		"beautiful",
		"distance",
		"luggage",
		"minecraft",
		"terraria"
	};

	std::string_view getRandomWord()
	{
		return words[Random::get<std::size_t>(0, words.size() - 1)];
	}
}

Session::Session(int lives)
	: m_lives(lives)
{}


char Session::getUserInput()
{
	char input{};
	while (true)
	{
		std::cout << "Enter your next letter: ";
		std::cin >> input;

		if (clearFailedExtraction() || !std::isalpha(input))
		{
			ignoreLine();
			std::cout << "That wasn't a valid input. Try again.\n";
			continue;
		}
		
		std::size_t index{ static_cast<std::size_t>((input % 32) - 1) };
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
		}
	}
}


void Session::processInput()
{
	char c{ getUserInput() };
	bool correctGuess{ false };

	for (std::size_t i{ 0 }; i < m_randomWord.size(); ++i)
		if (m_randomWord[i] == c)
		{
			m_guessWord[i] = true;
			correctGuess = true;
		}
	
	if (!correctGuess)
	{
		--m_lives;
		m_wrongGuesses.pop_front();
		m_wrongGuesses.push_back(c);
	}
}

void Session::displayState() const
{
	std::cout << "The word: ";
	for (std::size_t i{ 0 }; i < m_guessWord.size(); ++i)
	{
		if (m_guessWord[i])
			std::cout << m_randomWord[i];
		else
			std::cout << "_";
	}
	std::cout << "\tWrong guesses: ";
	for (char c : m_wrongGuesses)
		std::cout << c;

	std::cout << '\n';
}


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


int Session::getLives() const
{
	return m_lives;
}

bool Session::testForWin() const
{
	for (bool b : m_guessWord)
		if (!b)
			return false;
		
	return true;
}

void Session::handleGameEnd() const
{
	if (testForWin())
		displayWinMessage();
	else
		displayLossMessage();
}

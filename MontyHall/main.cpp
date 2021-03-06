// MontyHall.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <vector>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "gsl.h"
#include "LetsMakeADeal.h"

enum class ErrorType{ kGood, kWrongItemAmt, kBadStrategy, kNotEnoughDoors, kTooManyRevealed, kNotEnoughRevealed, kNotEnoughGames };

// This will push an error message into a stream based on errorType.
std::ostream& operator<<(std::ostream& stream, const ErrorType& errorType)
{
	stream << "Your command line arguments are incorrect. This is probably the issue:\n";
	switch (errorType)
	{
	case ErrorType::kGood: // Should never happen
	{
		Expects(false);
	}

	case ErrorType::kWrongItemAmt:
	{
		stream << "You do not have the right amount of items displayed.\n";
	}
	break;

	case ErrorType::kBadStrategy:
	{
		stream << "The strategy you tried to use was incorrect; use either \"Switch\" or \"Stay\", case sensitive.\n";
	}
	break;

	case ErrorType::kNotEnoughDoors:
	{
		stream << "You must have at least three doors to choose from.\n";
	}
	break;

	case ErrorType::kTooManyRevealed:
	{
		stream << "You cannot reveal too many doors, since there is a finite amount.\n";
	}
	break;

	case ErrorType::kNotEnoughRevealed:
	{
		stream << "You must reveal at least one door per game.\n";
	}
	break;

	case ErrorType::kNotEnoughGames:
	{
		stream << "You must run at least one instance of the game.\n";
	}
	break;

	default: // Should never happen
	{
		Expects(false);
	}

	}
	stream << "If this error does not describe your problem, type \"MontyHall.exe Help\" for a detailed description of what is needed to run.";
	return stream;
}

// This will take in cString argument from the command line and display a help message if the correct syntax is used.
void checkForHelp(const char* const arg)
{
	const std::string argString = arg;
	if (argString == "Help")
	{
		std::cout << "The syntax is \"MontyHall.exe [numDoors] [doorsRevealed] [gameInstances] [switchStrategy]\", where:\n" <<
			"- [numDoors] is the number of doors in each game, written as an integer. If left off, this will default to 3.\n" <<
			"- [doorsRevealed] is the number of doors to reveal in each game, written as an integer. If left off, this will default to 1.\n" <<
			"- [gameInstances] is the number of games to try and run, written as an integer.\n" <<
			"- [switchStrategy] is the strategy to use, written as \"Switch\" or \"Stay\", case sensitive." << std::endl;
	}
	else
	{
		std::cout << ErrorType::kWrongItemAmt << std::endl;
	}
}

// This will decipher whether or not the user asks for the Switch stategy, Stay strategy, or just some nonsense
Strategy getSwitchStrategy (std::stringstream& argStream)
{
	auto ret{ Strategy::kError };
	std::string strategyStr = "";
	argStream >> strategyStr;
	if (strategyStr == "Switch")
	{
		ret = Strategy::kSwitch;
	}
	else if (strategyStr == "Stay")
	{
		ret = Strategy::kStay;
	}
	return ret;
}

// This will run the simulation
void runSimulation(const int argc, const char* const argv[])
{
	// Get all of the arguments from the command line into a stringstream for processing
	std::stringstream argStream;
	for (auto i = 1; i < argc; i++)
	{
		argStream << argv[i] << " ";
	}

	auto numDoors{ 3 };
	auto doorsRevealed{ 1 };
	auto gameInstances{ 1 };
	auto switchStrategy{ Strategy::kError };
	auto error{ ErrorType::kGood };

	// Take the contents of the stringstream and put it into all of the right places based on the amount of arguments given
	switch (argc)
	{
	case 3:
	{
		argStream >> gameInstances;
		switchStrategy = getSwitchStrategy(argStream);
	}
	break;

	case 4:
	{
		argStream >> numDoors;
		argStream >> gameInstances;
		switchStrategy = getSwitchStrategy(argStream);
	}
	break;

	case 5:
	{
		argStream >> numDoors;
		argStream >> doorsRevealed;
		argStream >> gameInstances;
		switchStrategy = getSwitchStrategy(argStream);
	}
	break;

	default:
	{
		error = ErrorType::kWrongItemAmt;
	}
	break;
	}

	// Test for any errors other than the having an invalid amount of arguments
	if(error == ErrorType::kGood)
	{
		if (numDoors < 3)
		{
			error = ErrorType::kNotEnoughDoors;
		}
		else if (doorsRevealed > numDoors - 2)
		{
			error = ErrorType::kTooManyRevealed;
		}
		else if (doorsRevealed < 1)
		{
			error = ErrorType::kNotEnoughRevealed;
		}
		else if (gameInstances < 1)
		{
			error = ErrorType::kNotEnoughGames;
		}
		else if (switchStrategy == Strategy::kError)
		{
			error = ErrorType::kBadStrategy;
		}
	}

	if (error != ErrorType::kGood)
	{
		// Output any error that has occurred
		std::cout << error << std::endl;
	}
	else
	{
		// Run the simulations
		std::vector<const LetsMakeADeal*> games;
		auto carsWon{ 0 };
		auto goatsWon{ 0 };
		for (auto i = 0; i < gameInstances; i++)
		{
			games.push_back(new LetsMakeADeal{ numDoors, doorsRevealed, switchStrategy });
		}
		for (auto game : games)
		{
			if(game != nullptr)
			{
				if (game->simulateGame())
				{
					carsWon++;
				}
				else
				{
					goatsWon++;
				}
				delete game;
			}
		}

		// Get the win ratio & ensure that the ratio of cars won to goats won is ~1 (as stated in the spec)
		const auto carRatio = 1.0 * carsWon / gameInstances;
		const auto goatRatio = 1.0 * goatsWon / gameInstances;
		const auto delta = 1.0 - (carRatio + goatRatio);
		Expects(delta < 0.0001);

		std::cout << carRatio << std::endl;
	}
}

int main (const int argc, const char* const argv[])
{
	if(argc == 2) // Could either be "MontyHall.exe Help" or an input error
	{
		checkForHelp(argv[1]);
	}
	else
	{
		runSimulation(argc, argv);
	}
	_CrtDumpMemoryLeaks();
    return 0;
}


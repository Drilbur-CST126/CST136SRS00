// MontyHall.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <vector>
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#include "gsl.h"
#include "LetsMakeADeal.h"
#include "Strategy.h"

Strategy getSwitchStrategy (std::stringstream& stream)
{
	Strategy ret{ Strategy::kError };
	std::string strategyStr = "";
	stream >> strategyStr;
	if (!strategyStr.compare("Switch")) // If strategyStr is "switch"
	{
		ret = Strategy::kSwitch;
	}
	else if (!strategyStr.compare("Stay")) // If strategyStr is not "switch" and not "stay"; Double negative for clarity
	{
		ret = Strategy::kStay;
	}
	return ret;
}

void runSimulation(int argc, char* argv[])
{
	std::stringstream argStream;
	for (auto i = 1; i < argc; i++)
	{
		argStream << argv[i] << " ";
	}

	auto numDoors{ 3 };
	auto doorsRevealed{ 1 };
	auto gameInstances{ 1 };
	auto switchStrategy{ Strategy::kError };
	bool badInput;
	switch (argc)
	{
	case 3:
	{
		argStream >> gameInstances;
		switchStrategy = getSwitchStrategy(argStream);
		badInput = (switchStrategy == Strategy::kError);
	}
	break;
	case 4:
	{
		argStream >> numDoors;
		argStream >> gameInstances;
		switchStrategy = getSwitchStrategy(argStream);
		badInput = (switchStrategy == Strategy::kError);
	}
	break;
	case 5:
	{
		argStream >> numDoors;
		argStream >> doorsRevealed;
		argStream >> gameInstances;
		switchStrategy = getSwitchStrategy(argStream);
		badInput = (switchStrategy == Strategy::kError);
	}
	break;
	default:
		badInput = true;
		break;
	}

	if (numDoors < 3)
	{
		badInput = true;
	}
	if (doorsRevealed > numDoors - 2)
	{
		badInput = true;
	}
	if (doorsRevealed < 1)
	{
		badInput = true;
	}
	if (gameInstances < 0)
	{
		badInput = true;
	}

	if (badInput)
	{
		std::cout << "Your command line arguments are off! The syntax is \"MontyHall.exe [numDoors] [doorsRevealed] [gameInstances] [switchStrategy]\", where:\n" <<
			"- [numDoors] is the number of doors in each game, written as an integer. If left off, this will default to 3.\n" <<
			"- [doorsRevealed] is the number of doors to reveal in each game, written as an integer. If left off, this will default to 1.\n" <<
			"- [gameInstances] is the number of games to try and run, written as an integer.\n" <<
			"- [switchStrategy] is the strategy to use, written as \"Switch\" or \"Stay\", case sensitive." << std::endl;
	}
	else
	{
		std::vector<LetsMakeADeal*> games;
		auto carsWon{ 0 };
		auto goatsWon{ 0 };
		for (int i = 0; i < gameInstances; i++)
		{
			games.push_back(new LetsMakeADeal{ numDoors, doorsRevealed, switchStrategy });
		}
		for (auto game : games)
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

		auto carRatio = 1.0 * carsWon / gameInstances;
		auto goatRatio = 1.0 * goatsWon / gameInstances;
		auto delta = 1.0 - (carRatio + goatRatio);
		Expects(delta < 0.0001);
		std::cout << carRatio << std::endl;
		//std::cout << numDoors << ' ' << doorsRevealed << ' ' << gameInstances << ' ' << switchStrategy << ' ' << std::endl;
	}
}

int main (int argc, char* argv[])
{
	runSimulation(argc, argv);
	_CrtDumpMemoryLeaks();
    return 0;
}


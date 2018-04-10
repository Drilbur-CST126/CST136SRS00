#include "stdafx.h"
#include <random>
#include <vector>
#include "LetsMakeADeal.h"

// TODO: Remove
#include <iostream>

LetsMakeADeal::LetsMakeADeal(const int& doors, const int& reveal, const Strategy& strategy) : 
doors_{ doors },
reveal_{ reveal },
strategy_{ strategy }
{
	
}

bool LetsMakeADeal::simulateGame() const
{
	const auto carDoor{ getRandomInt(0, doors_) };
	auto selDoor{ getRandomInt(0, doors_) };
	std::vector<bool> doorOpened(doors_);
	auto doorsOpen = 0;
	while(doorsOpen < reveal_)
	{
		const auto doorToOpen{ getRandomInt(0, doors_) };
		if(doorToOpen != carDoor && doorToOpen != selDoor && !doorOpened[doorToOpen])
		{
			doorOpened[doorToOpen] = true;
			doorsOpen++;
		}
	}
	if(strategy_ == Strategy::kSwitch)
	{
		//std::cout << "SWITCH" << std::endl;
		int newSelDoor = selDoor;
		while(newSelDoor == selDoor || doorOpened[newSelDoor])
		{
			newSelDoor = getRandomInt(0, doors_);
		}
		selDoor = newSelDoor;
	}
	return selDoor == carDoor;
}

int LetsMakeADeal::getRandomInt(const int lower, const int upper)
{
	static std::mt19937 seed{ 42 };
	std::uniform_int_distribution<> dis(lower, upper - 1); // Open-closed; not pandering to Mitch, don't worry 'bout it
	return dis(seed);
}
#pragma once
#include <cstdlib>
#include <ctime>
#include "libraries.hpp"

class GamePlay
{
public:
	void InitGame();
	int* GetCards(); //

private:
	int startCards[6];
	
};


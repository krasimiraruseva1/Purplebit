#pragma once
#include <cstdlib>
#include <ctime>
#include "libraries.hpp"

class GamePlay
{
public:
	void InitGame();
	//void swap(int* a, int* b);
	//void shuffle(int card[], int n);
private:
};

struct Player {
	int cardIndexes[4] = { ((rand() % 48) + 1), ((rand() % 48) + 1), ((rand() % 48) + 1), ((rand() % 48) + 1) };
};


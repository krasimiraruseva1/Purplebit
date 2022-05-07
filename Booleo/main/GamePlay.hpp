#pragma once
#include <cstdlib>
#include <ctime>
class GamePlay
{
public:
	void InitGame();
	bool* GetCards();

private:
	bool startCards[6];
	
};


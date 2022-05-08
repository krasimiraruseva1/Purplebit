#include "GamePlay.hpp"
#include "entity.hpp"
#include "renderWindow.hpp"

void GamePlay::InitGame()
{
	srand(time(NULL));
	
	for (int i = 0; i < 6; i++)
	{
		startCards[i] = rand() % 49;
	}

}

int* GamePlay::GetCards() //
{
	return &startCards[0];
}



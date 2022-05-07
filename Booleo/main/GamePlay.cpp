#include "GamePlay.h"

void GamePlay::InitGame()
{
	srand(time(NULL));
	
	for (int i = 0; i < 6; i++)
	{
		startCards[i] = rand() % 2;
	}

}

bool* GamePlay::GetCards() {
	return &startCards[0];
}

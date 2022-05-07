#include "RenderWindow.hpp"
#include "entity.hpp"
#include "utilities.hpp"
#include "entity.hpp"
#include "main.h"
#include "GamePlay.h"

int main(int argc, char* args[])
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
	{
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
	}

	if (!(IMG_Init(IMG_INIT_PNG)))
	{
		std::cout << "IMG Error: " << SDL_GetError() << std::endl;
	}

	RenderWindow window("Booleo", 1280, 720);
	bool home = true;
	SDL_Texture* mainMenu = window.loadTexture("../assets/mainScreen.png");
	SDL_Texture* card = window.loadTexture("../assets/NotCardRE.png");
	SDL_Texture* card01 = window.loadTexture("../assets/NotCardRE.png");
	SDL_Texture* card01Flipped = window.loadTexture("../assets/OneAndCard.png");

	SDL_Renderer* renderer = SDL_CreateRenderer(window.getWindow(), -1, 0);

	ENTITY mainScreen(0, 0, mainMenu); //Button goes here
	bool gameRunning = true;
	SDL_Event event;
	ENTITY cardButton(0, 0, card);

	const float timeAdvanced = 0.01;
	float accumulator = 0.0;
	float currentTime = utils::hireTimeInSeconds();
	GamePlay gameplay;
	gameplay.InitGame();

	while (gameRunning)
	{
		int startTicks = SDL_GetTicks();
		float newTime = utils::hireTimeInSeconds();
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;
		while (accumulator >= timeAdvanced)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					gameRunning = false;
			}
			accumulator -= timeAdvanced;
		}
		const float alpha = accumulator / timeAdvanced; 

		window.clear();
		if (home) {
			ShowHome(window, mainScreen, cardButton, home);
		}
		else {
			window.clear();
			ENTITY* playCards[6];
			bool* cards = gameplay.GetCards();
			for (int i = 0; i < 6; i++)
			{
				if (cards[i]) {
					playCards[i] = new ENTITY(i * 50, 0, card01);
				}
				else {
					playCards[i] = new ENTITY(i * 50, 0, card01Flipped);
				}
				window.render(*playCards[i]);
			}
		}



		window.display();
		
		int frameTicks = SDL_GetTicks() - startTicks;
		if (frameTicks < 1000 / window.getRefreshRate())
		{
			SDL_Delay(1000 / window.getRefreshRate() - frameTicks);
		}
	}
	window.cleanUp();
	SDL_Quit();

	return 0;
}

void ShowHome(RenderWindow& window, ENTITY& mainScreen, ENTITY& cardButton, bool& home)
{
	window.render(mainScreen);
	window.render(cardButton);
	int buttonX = 0, buttonY = 0;
	Uint32 buttons;
	SDL_PumpEvents();  // make sure we have the latest mouse state.
	buttons = SDL_GetMouseState(&buttonX, &buttonY);
	if ((buttons & SDL_BUTTON_LMASK) != 0) {
		int width = cardButton.getCurrentFrame().w;
		int height = cardButton.getCurrentFrame().h;
		if (cardButton.getX() <= buttonX && buttonX <= cardButton.getX() + width
			&& cardButton.getY() <= buttonY && buttonY <= cardButton.getY() + height) {
			home = false;
		}
	}


}

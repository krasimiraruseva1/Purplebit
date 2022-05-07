#include "RenderWindow.hpp"
#include "entity.hpp"
#include "utilities.hpp"
#include "entity.hpp"
#include "main.hpp"
#include "GamePlay.hpp"


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
	SDL_Texture* PvC = window.loadTexture("../assets/PvC.png");
	SDL_Texture* PvCnot = window.loadTexture("../assets/PvCnot.png");
	SDL_Texture* PvP = window.loadTexture("../assets/PvP.png");
	SDL_Texture* PvPnot = window.loadTexture("../assets/PvPnot.png");

	SDL_Renderer* renderer = SDL_CreateRenderer(window.getWindow(), -1, 0);

	ENTITY mainScreen(0, 0, mainMenu); //Button goes here
	bool gameRunning = true;
	SDL_Event event;
	ENTITY PvCButton(360, 250, PvC);
	ENTITY PvCnotButton(377, 363, PvCnot);
	ENTITY PvPButton(385, 495, PvP);
	ENTITY PvPnotButton(396, 575, PvPnot);

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
			ShowHome(window, mainScreen, PvCButton, PvCnotButton, PvPButton, PvPnotButton, home);
		}
		else {
			window.clear();
			ENTITY* playCards[6];
			bool* cards = gameplay.GetCards();
			/*for (int i = 0; i < 6; i++)
			{
				if (cards[i]) {
					playCards[i] = new ENTITY(i * 50, 0, card01);
				}
				else {
					playCards[i] = new ENTITY(i * 50, 0, card01Flipped);
				}
				window.render(*playCards[i]);
			}*/
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

void ShowHome(RenderWindow& window, ENTITY& mainScreen, ENTITY& Button1, ENTITY& Button2, ENTITY& Button3, ENTITY& Button4, bool& home)
{
	window.render(mainScreen);
	window.render(Button1);
	window.render(Button2);
	window.render(Button3);
	window.render(Button4);
	int buttonX = 0, buttonY = 0;
	Uint32 buttons;
	SDL_PumpEvents();  // make sure we have the latest mouse state.
	buttons = SDL_GetMouseState(&buttonX, &buttonY);
	if ((buttons & SDL_BUTTON_LMASK) != 0) {
		int width = Button1.getCurrentFrame().w;
		int height = Button1.getCurrentFrame().h;
		if (Button1.getX() <= buttonX && buttonX <= Button1.getX() + width
			&& Button1.getY() <= buttonY && buttonY <= Button1.getY() + height) {
			home = false;
		}
	}


}

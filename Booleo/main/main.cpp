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
	int buttonPressed = NULL;
	SDL_Texture* mainMenu = window.loadTexture("../assets/mainScreen.png");
	SDL_Texture* playmat = window.loadTexture("../assets/playmat.png");
	SDL_Texture* PvC = window.loadTexture("../assets/PvC.png");
	SDL_Texture* PvCnot = window.loadTexture("../assets/PvCnot.png");
	SDL_Texture* PvP = window.loadTexture("../assets/PvP.png");
	SDL_Texture* PvPnot = window.loadTexture("../assets/PvPnot.png");

	SDL_Texture* BackCard = window.loadTexture("../assets/BackCard.png");
	SDL_Texture* NotCard = window.loadTexture("../assets/NotCard.png");
	SDL_Texture* OneAndCard = window.loadTexture("../assets/OneAndCard.png");
	SDL_Texture* OneOrCard = window.loadTexture("../assets/OneOrCard.png");
	SDL_Texture* OneXorCard = window.loadTexture("../assets/OneXorCard.png");
	SDL_Texture* ZeroAndCard = window.loadTexture("../assets/ZeroAndCard.png");
	SDL_Texture* ZeroOrCard = window.loadTexture("../assets/ZeroOrCard.png");
	SDL_Texture* ZeroXorCard = window.loadTexture("../assets/ZeroXorCard.png");

	SDL_Renderer* renderer = SDL_CreateRenderer(window.getWindow(), -1, 0);

	ENTITY mainScreen(0, 0, mainMenu); //Button goes here
	ENTITY playmatScreen(0, 0, playmat);
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
			ShowHome(window, mainScreen, PvCButton, PvCnotButton, PvPButton, PvPnotButton, home, buttonPressed);
		}
		else {
			window.clear();
			window.render(playmatScreen);
			if (buttonPressed == 1)
			{
				window.render(PvCButton);
			}
			else if (buttonPressed == 2)
			{
				window.render(PvCnotButton);
			}
			else if (buttonPressed == 3)
			{
				window.render(PvCnotButton);
			}
			else if (buttonPressed == 4)
			{
				window.render(PvCnotButton);
			}
			
			/*ENTITY* playCards[6];
			int* cards = gameplay.GetCards();

			for (int i = 0; i < 6; i++)
			{
				if (cards[i]) {
					playCards[i] = new ENTITY(i * 50, 0, BackCard);
				}
				else {
					playCards[i] = new ENTITY(i * 50, 0, cards);
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


void ShowHome(RenderWindow& window, ENTITY& mainScreen, ENTITY& Button1, ENTITY& Button2, ENTITY& Button3, ENTITY& Button4, bool& home, int& buttonPressed)
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
			&& Button1.getY() <= buttonY && buttonY <= Button1.getY() + height) 
		{
			home = false; //link to PvC
			buttonPressed = 1;

		}
		else if (Button2.getX() <= buttonX && buttonX <= Button2.getX() + width
			&& Button2.getY() <= buttonY && buttonY <= Button2.getY() + height)
		{
			home = false;
			buttonPressed = 2;
		}
		else if (Button3.getX() <= buttonX && buttonX <= Button3.getX() + width
			&& Button3.getY() <= buttonY && buttonY <= Button3.getY() + height)
		{
			home = false;
			buttonPressed = 3;
		}
		else if (Button4.getX() <= buttonX && buttonX <= Button4.getX() + width
			&& Button4.getY() <= buttonY && buttonY <= Button4.getY() + height)
		{
			home = false;
			buttonPressed = 4;
		}

	}


}

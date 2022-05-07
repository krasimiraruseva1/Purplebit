#include "RenderWindow.hpp"
#include "entity.hpp"
#include "utilities.hpp"

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
	SDL_Surface* surface = IMG_Load("../assets/mainScreen.png");

	SDL_Texture* mainMenu = window.loadTexture("../assets/mainScreen.png");
	ENTITY vsPlayerButton(0, 0, mainMenu); //Button goes here

	bool gameRunning = true;
	SDL_Event event;
	
	const float timeAdvanced = 0.01;
	float accumulator = 0.0;
	float currentTime = utils::hireTimeInSeconds();

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
		window.render(vsPlayerButton);
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
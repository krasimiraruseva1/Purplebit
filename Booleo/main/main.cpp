#include "RenderWindow.hpp"

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
	SDL_Texture* mainMenu = window.loadTexture(""); //main menu screen here

	bool gameRunning = true;
	SDL_Event event;

	while (gameRunning)
	{
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT)
				gameRunning = false;
		}
		window.clear();
		window.render(mainMenu);
		window.display();
	}
	window.cleanUp();
	SDL_Quit();

	return 0;
}
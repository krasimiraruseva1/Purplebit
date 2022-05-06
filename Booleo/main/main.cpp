#include "RenderWindow.hpp"
#include "entity.hpp"

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

	while (gameRunning)
	{
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT)
				gameRunning = false;
		}
		window.clear();
		window.render(vsPlayerButton);
		window.display();
	}
	window.cleanUp();
	SDL_Quit();

	return 0;
}
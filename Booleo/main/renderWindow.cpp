#include "renderWindow.hpp"
#include "libraries.hpp"
#include "entity.hpp" //includes libraries

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h) //renders window
	:window(NULL), renderer(NULL)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN); //creates window

	if (window == NULL)
	{
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl; //prints out error if window fails to render
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //creates renderer
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath) //loads texture
{
	SDL_Texture* texture = NULL; //declares texture
	texture = IMG_LoadTexture(renderer, p_filePath);
	if (texture == NULL)
	{
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl; //prints out error if texture fails to load
	}
	return texture; //returns texture
}

int RenderWindow::getRefreshRate() //gets refresh rate
{
	int displayIndex = SDL_GetWindowDisplayIndex(window); //gets index of the display associated with the window
	SDL_DisplayMode mode; //declares display mode
	SDL_GetDisplayMode(displayIndex, 0, &mode); //passes memory address of mode

	return mode.refresh_rate;
}

void RenderWindow::cleanUp() //cleans everything by destroying window
{
	SDL_DestroyWindow(window);
}

void RenderWindow::clear() //clears window by removing rendered objects
{
	SDL_RenderClear(renderer);
}

SDL_Window* RenderWindow::getWindow() //returns window
{
	return window;
}

// declares render
void RenderWindow::render(ENTITY& p_entity) 
{
	SDL_Rect src;
	src.x = p_entity.getCurrentFrame().x;
	src.y = p_entity.getCurrentFrame().y;
	src.w = p_entity.getCurrentFrame().w;
	src.h = p_entity.getCurrentFrame().h;

	SDL_Rect dest;
	dest.x = p_entity.getX();
	dest.y = p_entity.getY();
	dest.w = p_entity.getCurrentFrame().w;
	dest.h = p_entity.getCurrentFrame().h;

	// declares proper way of RenderCopy functioning
	SDL_RenderCopy(renderer, p_entity.getTex(), &src, &dest);
}

void RenderWindow::display() //renders display
{
	SDL_RenderPresent(renderer);
}

SDL_Renderer* RenderWindow::getRenderer()
{
	return renderer;
}

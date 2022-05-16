#pragma once
#include "entity.hpp"

class RenderWindow // declares RenderWindow class
{
public:
	RenderWindow(const char* p_title, int p_w, int p_h); // renders window
	SDL_Texture* loadTexture(const char* p_filePath); // loads texture

	int getRefreshRate(); // returns refresh rate
	void cleanUp(); // cleans up screen
	void clear(); // clears window
	SDL_Window* getWindow(); // returns window
	void render(ENTITY& p_entity); // renders textures
	void display(); //displays
	SDL_Renderer* getRenderer();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};
#pragma once
#include "entity.hpp"

class RenderWindow
{
public:
	RenderWindow(const char* p_title, int p_w, int p_h);
	SDL_Texture* loadTexture(const char* p_filePath);

	int getRefreshRate();
	void cleanUp();
	void clear();

	void render(ENTITY& p_entity);
	void display();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};
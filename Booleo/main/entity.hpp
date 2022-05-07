#pragma once
#include "libraries.hpp"

class ENTITY
{
public:
	ENTITY(float p_x, float p_y, SDL_Texture* p_tex);
	float getX();
	void setX(float value);
	float getY();
	void setY(float value);
	void drag(bool active);
	void controlDrag();
	void setDraggable(bool active);
	SDL_Texture* getTex();
	SDL_Rect getCurrentFrame();
private:
	float x, y;
	bool draggable = false;
	SDL_Rect currentFrame;
	SDL_Texture* texture;
};

class OBJECT
{
public:
	SDL_Texture* getTexture();
	SDL_Rect* getSrc();
	SDL_Rect* getDest();
	void setSrc(int x, int y, int h, int w);
	void setDest(int x, int y, int h, int w);
	void CreateTexture(const char* address, SDL_Renderer* ren);
	void virtual Render(SDL_Renderer* ren) = 0;
private:
	SDL_Rect* src, *dest;
	SDL_Texture* tex;
};
#include "entity.hpp"
#include "libraries.hpp"
#include "renderWindow.hpp"

ENTITY::ENTITY(float p_x, float p_y, SDL_Texture* p_tex)
	:x(p_x), y(p_y), texture(p_tex)
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 32;
	currentFrame.h = 32;
}

float ENTITY::getX()
{
	return x;
}

float ENTITY::getY()
{
	return y;
}

SDL_Texture* ENTITY::getTex()
{
	return texture;
}

SDL_Rect ENTITY::getCurrentFrame()
{
	return currentFrame;
}
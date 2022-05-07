#include "entity.hpp"
#include "libraries.hpp"
#include "renderWindow.hpp"
#include "textureManager.hpp"

ENTITY::ENTITY(float p_x, float p_y, SDL_Texture* p_tex)
	:x(p_x), y(p_y), texture(p_tex)
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	SDL_Point size;
	SDL_QueryTexture(p_tex, NULL, NULL, &size.x, &size.y);

	currentFrame.w = size.x;
	currentFrame.h = size.y;
}


float ENTITY::getX()
{
	return x;
}

void ENTITY::setX(float value)
{
	x = value;
}

float ENTITY::getY()
{
	return y;
}

void ENTITY::setY(float value)
{
	y = value;
}

void ENTITY::drag(bool active)
{
	setDraggable(active);
	if (draggable) 
	{
		controlDrag();
	}
}

void ENTITY::controlDrag()
{
	int buttonX = 0, buttonY = 0;
	Uint32 buttons;
	SDL_PumpEvents();  // make sure we have the latest mouse state.
	buttons = SDL_GetMouseState(&buttonX, &buttonY);

	if (draggable) {

		setX(buttonX-currentFrame.w/2);
		setY(buttonY-currentFrame.h/2);

		if ((buttons & SDL_BUTTON_LMASK) != 0) {
			draggable = false;
			SDL_Delay(500);
		}
	}
	else
	{
		if ((buttons & SDL_BUTTON_LMASK) != 0) {
			int width = currentFrame.w;
			int height = currentFrame.h;
			if (getX() <= buttonX && buttonX <= getX() + width && getY() <= buttonY && buttonY <= getY() + height) {
				draggable = true;
				SDL_Delay(500);
			}
		}

	}
}

void ENTITY::setDraggable(bool active)
{
	draggable = active;
}

SDL_Texture* ENTITY::getTex()
{
	return texture;
}

SDL_Rect ENTITY::getCurrentFrame()
{
	return currentFrame;
}


SDL_Texture* OBJECT::getTexture()
{
	return tex;
}

SDL_Rect* OBJECT::getSrc()
{
	return src;
}

SDL_Rect* OBJECT::getDest()
{
	return dest;
}


void OBJECT::setSrc(int x, int y, int h, int w)
{
	src->x = x;
	src->y = y;
	src->h = h;
	src->w = w;
}

void OBJECT::setDest(int x, int y, int h, int w)
{
	dest->x = x;
	dest->y = y;
	dest->h = h;
	dest->w = w;
}

void OBJECT::CreateTexture(const char* address, SDL_Renderer* ren)
{
	tex = TextureManager::Texture(address, ren);
}
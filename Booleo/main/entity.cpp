#include "entity.hpp"
#include "libraries.hpp"
#include "renderWindow.hpp"
#include "textureManager.hpp" //includes header files and libraries

ENTITY::ENTITY(float p_x, float p_y, SDL_Texture* p_tex) //declares proper entity values
	:x(p_x), y(p_y), texture(p_tex)
{
	currentFrame.x = 0;	//declares current frame coordinates
	currentFrame.y = 0;
	SDL_Point size; //defines two dimensional point
	SDL_QueryTexture(p_tex, NULL, NULL, &size.x, &size.y); //resets texture

	currentFrame.w = size.x;	//declares current frame width
	currentFrame.h = size.y;	//declares current frame height
}


float ENTITY::getX() //returns current x coord
{
	return x;
}

void ENTITY::setX(float value) //modifies x
{
	x = value;
}

float ENTITY::getY() //teturns y
{
	return y;
}

void ENTITY::setY(float value) //modifies y
{
	y = value;
}

void ENTITY::drag() //tells program how drag function works
{
	if (draggable) 
	{
		controlDrag();
	}
}

void ENTITY::controlDrag()
{
	int buttonX = 0, buttonY = 0; //declares button coords 
	Uint32 buttons; //defines buttons as unsigned integer (32-bit)
	SDL_PumpEvents();  // make sure we have the latest mouse state.
	buttons = SDL_GetMouseState(&buttonX, &buttonY); //makes button equal to mouse state
	setX(buttonX - currentFrame.w / 2);
	setY(buttonY - currentFrame.h / 2);
}

void ENTITY::setDraggable(bool active) 
{
	draggable = active; //makes draggable possible
}

bool ENTITY::isMouseClicked() //checks if left mouse button is clicked
{
	int x, y; //declares coordinates
	Uint32 buttons;

	SDL_PumpEvents();  // makes sure we have the latest mouse state

	buttons = SDL_GetMouseState(&x, &y); 

	if ((buttons & SDL_BUTTON_LMASK) != 0) //if left button is pressed
	{
		int width = getCurrentFrame().w; //change coordinates to mouse's position
		int height = getCurrentFrame().h;
		if (getX() <= x && x <= getX() + width
			&& getY() <= y && y <= getY() + height)
		{
			//SDL_Delay(60);
			return true;
		}
	}

	return false;
}

bool ENTITY::getDrag() //returns if draggable
{
	return draggable;
}

SDL_Texture* ENTITY::getTex()
{
	return texture;
}

void ENTITY::setTexture(SDL_Texture* tex)
{
	texture = tex;
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
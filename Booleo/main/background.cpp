#include "Background.hpp"

void Background::Render(SDL_Renderer* ren)
{
	SDL_RenderCopy(ren, getTexture(), NULL, NULL);
}

void Background::GroundRender(SDL_Renderer* ren)
{
	SDL_RenderCopy(ren, getTexture(), getSrc(), getDest());
}

void Background::GroundUpdate1()
{
	if (distance1 <= -330)
	{
		distance1 = 336;
	}
	else
	{
		distance1--;
		setSrc(0, 0, 112, 336);
		setDest(distance1, 420, 112, 336);
	}
}

void Background::GroundUpdate2()
{
	if (distance2 <= -330)
	{
		distance2 = 336;
	}
	else
	{
		distance2--;
		setSrc(0, 0, 112, 336);
		setDest(distance2, 420, 112, 336);
	}
}
#pragma once
#include "entity.hpp"

class Background :public OBJECT
{
public:
	void GroundUpdate1();
	void GroundUpdate2();
	void Render(SDL_Renderer* ren);
	void GroundRender(SDL_Renderer* ren);
private:
	int distance1 = 0;
	int distance2 = 336;
};
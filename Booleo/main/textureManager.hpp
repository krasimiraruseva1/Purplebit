#pragma once
#include "libraries.hpp" //includes libraries

class TextureManager //declares TextureManager class
{
public:
	static SDL_Texture* Texture(const char* filelocation, SDL_Renderer* ren); //tells texture proper values to program
};
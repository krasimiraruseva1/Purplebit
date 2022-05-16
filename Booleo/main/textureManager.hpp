#pragma once
// includes libraries
#include "libraries.hpp" 
// declares TextureManager class
class TextureManager 
{
public:
	// tells texture proper values to program
	static SDL_Texture* Texture(const char* filelocation, SDL_Renderer* ren); 
};
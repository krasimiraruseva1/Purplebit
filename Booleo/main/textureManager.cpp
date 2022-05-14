#include "TextureManager.hpp" //includes matching header file

SDL_Texture* TextureManager::Texture(const char* filelocation, SDL_Renderer* ren) //explains proper texture values to program
{
	SDL_Surface* surface; //declares a structure that contains a collection of pixels
	surface = IMG_Load(filelocation); //declares proper way for surface to function
	SDL_Texture* tex; //declares texture
	tex = SDL_CreateTextureFromSurface(ren, surface); //declares proper way for texture to function
	return tex;//returns texture
}
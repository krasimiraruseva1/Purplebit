// includes matching header file
#include "TextureManager.hpp" 

// explains proper texture values to program
SDL_Texture* TextureManager::Texture(const char* filelocation, SDL_Renderer* ren) 
{

	// declares a structure that contains a collection of pixels
	SDL_Surface* surface; 

	// declares proper way for surface to function
	surface = IMG_Load(filelocation); 

	// declares texture
	SDL_Texture* tex; 

	// declares proper way for texture to function
	tex = SDL_CreateTextureFromSurface(ren, surface); 

	// returns texture
	return tex;
}
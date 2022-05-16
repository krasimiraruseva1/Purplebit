// do only once
#pragma once 

// add libraries
#include "libraries.hpp" 

// initialize ENTITY class
class ENTITY 
{
	public:

	// declares entity values
		ENTITY(float p_x, float p_y, SDL_Texture* p_tex); 

		// returns x
		float getX(); 

		// changes value of x
		void setX(float value); 

		// returns y
		float getY();	

		// changes value of y
		void setY(float value);	

		// initializes dragging of entity
		void drag();

		// controls drag 
		void controlDrag();	

		// makes entity draggable/undraggable
		void setDraggable(bool active);

		// says if mouse is clicked
		bool isMouseClicked();

		// says if draggable
		bool getDrag(); 

		// gets texture
		SDL_Texture* getTex(); 

		// changes texture
		void setTexture(SDL_Texture* tex); 
		SDL_Rect getCurrentFrame();

		// initialize inaccessible variables
	private:

		// coordinates
		float x, y;	

		// makes entity draggable/undraggable
		bool draggable = false;	

		// initializes current frame
		SDL_Rect currentFrame;
		
		// initializes texture
		SDL_Texture* texture;	
};


//initializes object class
class OBJECT 
{
	public: 

		// returns texture
		SDL_Texture* getTexture();

		// gets source location
		SDL_Rect* getSrc();

		// gets destination
		SDL_Rect* getDest();

		// sets source
		void setSrc(int x, int y, int h, int w); 

		 // sets destinaton
		void setDest(int x, int y, int h, int w);

		// creates texture
		void CreateTexture(const char* address, SDL_Renderer* ren);

		// creates renderer
		void virtual Render(SDL_Renderer* ren) = 0; 
	private:

		// initializes source and destination
		SDL_Rect* src, *dest; 

		// initializes texture
		SDL_Texture* tex; 
};

struct CARD
{
	std::string name;
	SDL_Texture* texture;
};
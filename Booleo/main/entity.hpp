#pragma once //do only once
#include "libraries.hpp" //add libraries

class ENTITY //initialize ENTITY class
{
public:
	ENTITY(float p_x, float p_y, SDL_Texture* p_tex); //declares entity values
	float getX(); //returns x
	void setX(float value); //changes value of x
	float getY();	//returns y
	void setY(float value);	//changes value of y
	void drag();	//initializes dragging of entity
	void controlDrag();	//controls drag 
	void setDraggable(bool active); //makes entity draggable/undraggable
	bool isMouseClicked(); //says if mouse is clicked
	bool getDrag(); //says if draggablr
	SDL_Texture* getTex(); //gets texture
	SDL_Rect getCurrentFrame();
private:	//initialize inaccessible variables
	float x, y;	//coordinates
	bool draggable = false;	//makes entity draggable/undraggable
	SDL_Rect currentFrame;	//initializes current frame
	SDL_Texture* texture;	//initializes texture
};

class OBJECT //initializes object class
{
public: 
	SDL_Texture* getTexture(); //returns texture
	SDL_Rect* getSrc();	//gets source location
	SDL_Rect* getDest(); //gets destination
	void setSrc(int x, int y, int h, int w); //sets source
	void setDest(int x, int y, int h, int w); //sets destinaton
	void CreateTexture(const char* address, SDL_Renderer* ren); //creates texture
	void virtual Render(SDL_Renderer* ren) = 0; //creates renderer
private:
	SDL_Rect* src, *dest; //initializes source and destination
	SDL_Texture* tex; //initializes texture
};
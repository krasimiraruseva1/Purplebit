#include "RenderWindow.hpp"
#include "entity.hpp"
#include "libraries.hpp"
#include "utilities.hpp"
#include "entity.hpp"
#include "main.hpp"
#include "GamePlay.hpp"

int main(int argc, char* args[])
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
	{
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
	}

	if (!(IMG_Init(IMG_INIT_PNG)))
	{
		std::cout << "IMG Error: " << SDL_GetError() << std::endl;
	}

	RenderWindow window("Booleo", 1280, 720);
	bool home = true;
	int buttonPressed = NULL;
	GamePlay gameplay;
	std::string P1hand[10];
	std::string P2hand[10];
	std::string deck[48];

	srand(time(0));
	for (int i = 0; i < 48; i++)
	{
		if (i <= 7)
		{
			deck[i] = "OR0";
		}
		else if (i > 7 && i <= 15)
		{
			deck[i] = "OR1";
		}
		else if (i > 15 && i <= 23)
		{
			deck[i] = "AND0";
		}
		else if (i > 23 && i <= 31)
		{
			deck[i] = "AND1";
		}
		else if (i > 31 && i <= 39)
		{
			deck[i] = "XOR0";
		}
		else if (i > 39 && i <= 47)
		{
			deck[i] = "XOR1";
		}
	}
	for (int i = 0; i < 48; i++)
	{
		int index = rand() % 48;
		std::string temp = deck[i];
		deck[i] = deck[index];
		deck[index] = temp;
	}
	
	SDL_Texture* mainMenu = window.loadTexture("../assets/mainScreen.png");
	SDL_Texture* playmat = window.loadTexture("../assets/playmat.png");
	SDL_Texture* PvC = window.loadTexture("../assets/PvC.png");
	SDL_Texture* PvCnot = window.loadTexture("../assets/PvCnot.png");
	SDL_Texture* PvP = window.loadTexture("../assets/PvP.png");
	SDL_Texture* PvPnot = window.loadTexture("../assets/PvPnot.png");

	SDL_Texture* BackCard = window.loadTexture("../assets/BackCard.png");
	SDL_Texture* NotCard = window.loadTexture("../assets/NotCard.png");
	SDL_Texture* OneAndCard = window.loadTexture("../assets/OneAndCard.png");
	SDL_Texture* OneOrCard = window.loadTexture("../assets/OneOrCard.png");
	SDL_Texture* OneXorCard = window.loadTexture("../assets/OneXorCard.png");
	SDL_Texture* ZeroAndCard = window.loadTexture("../assets/ZeroAndCard.png");
	SDL_Texture* ZeroOrCard = window.loadTexture("../assets/ZeroOrCard.png");
	SDL_Texture* ZeroXorCard = window.loadTexture("../assets/ZeroXorCard.png");

	SDL_Renderer* renderer = SDL_CreateRenderer(window.getWindow(), -1, 0);

	ENTITY mainScreen(0, 0, mainMenu); //Button goes here
	ENTITY playmatScreen(0, 0, playmat);
	bool gameRunning = true;
	SDL_Event event;
	ENTITY PvCButton(360, 250, PvC);
	ENTITY PvCnotButton(377, 363, PvCnot);
	ENTITY PvPButton(385, 495, PvP);
	ENTITY PvPnotButton(396, 575, PvPnot);

	const float timeAdvanced = 0.01;
	float accumulator = 0.0;
	float currentTime = utils::hireTimeInSeconds();
	
	
	while (gameRunning)
	{
		int startTicks = SDL_GetTicks();
		float newTime = utils::hireTimeInSeconds();
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;
		while (accumulator >= timeAdvanced)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					gameRunning = false;
			}
			accumulator -= timeAdvanced;
		}
		const float alpha = accumulator / timeAdvanced; 

		window.clear();
		if (home) {
			ShowHome(window, mainScreen, PvCButton, PvCnotButton, PvPButton, PvPnotButton, home, buttonPressed);
		}
		else {
			window.clear();
			window.render(playmatScreen);
			if (buttonPressed == 1)
			{
				SDL_Texture* CARD1_P1C = NULL;
				SDL_Texture* CARD2_P1C = NULL;
				SDL_Texture* CARD3_P1C = NULL;
				SDL_Texture* CARD4_P1C = NULL;

				SDL_Texture* CARD1_P2C = NULL;
				SDL_Texture* CARD2_P2C = NULL;
				SDL_Texture* CARD3_P2C = NULL;
				SDL_Texture* CARD4_P2C = NULL;
				for (int i = 0; i < 8; i++)
				{
					if (i == 0)
					{
						if (deck[i] == "OR0")
						{
							CARD1_P1C = ZeroOrCard;
						}
						else if (deck[i] == "OR1")
						{
							CARD1_P1C = OneOrCard;
						}
						else if (deck[i] == "AND0")
						{
							CARD1_P1C = ZeroAndCard;
						}
						else if (deck[i] == "AND1")
						{
							CARD1_P1C = OneAndCard;
						}
						else if (deck[i] == "XOR0")
						{
							CARD1_P1C = ZeroXorCard;
						}
						else if (deck[i] == "XOR1")
						{
							CARD1_P1C = OneOrCard;
						}
					}
					else if (i == 1)
					{
						if (deck[i] == "OR0")
						{
							CARD2_P1C = ZeroOrCard;
						}
						else if (deck[i] == "OR1")
						{
							CARD2_P1C = OneOrCard;
						}
						else if (deck[i] == "AND0")
						{
							CARD2_P1C = ZeroAndCard;
						}
						else if (deck[i] == "AND1")
						{
							CARD2_P1C = OneAndCard;
						}
						else if (deck[i] == "XOR0")
						{
							CARD2_P1C = ZeroXorCard;
						}
						else if (deck[i] == "XOR1")
						{
							CARD2_P1C = OneOrCard;
						}
					}

					else if (i == 2)
					{
						if (deck[i] == "OR0")
						{
							CARD3_P1C = ZeroOrCard;
						}
						else if (deck[i] == "OR1")
						{
							CARD3_P1C = OneOrCard;
						}
						else if (deck[i] == "AND0")
						{
							CARD3_P1C = ZeroAndCard;
						}
						else if (deck[i] == "AND1")
						{
							CARD3_P1C = OneAndCard;
						}
						else if (deck[i] == "XOR0")
						{
							CARD3_P1C = ZeroXorCard;
						}
						else if (deck[i] == "XOR1")
						{
							CARD3_P1C = OneOrCard;
						}
					}
					else if (i == 3)
					{
						if (deck[i] == "OR0")
						{
							CARD4_P1C = ZeroOrCard;
						}
						else if (deck[i] == "OR1")
						{
							CARD4_P1C = OneOrCard;
						}
						else if (deck[i] == "AND0")
						{
							CARD4_P1C = ZeroAndCard;
						}
						else if (deck[i] == "AND1")
						{
							CARD4_P1C = OneAndCard;
						}
						else if (deck[i] == "XOR0")
						{
							CARD4_P1C = ZeroXorCard;
						}
						else if (deck[i] == "XOR1")
						{
							CARD4_P1C = OneOrCard;
						}
						
					}
					for (int i = 4; i < 8; i++)
					{
						if (i == 4)
						{
							if (deck[i] == "OR0")
							{
								CARD1_P2C = ZeroOrCard;
							}
							else if (deck[i] == "OR1")
							{
								CARD1_P2C = OneOrCard;
							}
							else if (deck[i] == "AND0")
							{
								CARD1_P2C = ZeroAndCard;
							}
							else if (deck[i] == "AND1")
							{
								CARD1_P2C = OneAndCard;
							}
							else if (deck[i] == "XOR0")
							{
								CARD1_P2C = ZeroXorCard;
							}
							else if (deck[i] == "XOR1")
							{
								CARD1_P2C = OneOrCard;
							}
						}

						else if (i == 5)
						{
							if (deck[i] == "OR0")
							{
								CARD2_P2C = ZeroOrCard;
							}
							else if (deck[i] == "OR1")
							{
								CARD2_P2C = OneOrCard;
							}
							else if (deck[i] == "AND0")
							{
								CARD2_P2C = ZeroAndCard;
							}
							else if (deck[i] == "AND1")
							{
								CARD2_P2C = OneAndCard;
							}
							else if (deck[i] == "XOR0")
							{
								CARD2_P2C = ZeroXorCard;
							}
							else if (deck[i] == "XOR1")
							{
								CARD2_P2C = OneOrCard;
							}
						}

						else if (i == 6)
						{
							if (deck[i] == "OR0")
							{
								CARD3_P2C = ZeroOrCard;
							}
							else if (deck[i] == "OR1")
							{
								CARD3_P2C = OneOrCard;
							}
							else if (deck[i] == "AND0")
							{
								CARD3_P2C = ZeroAndCard;
							}
							else if (deck[i] == "AND1")
							{
								CARD3_P2C = OneAndCard;
							}
							else if (deck[i] == "XOR0")
							{
								CARD3_P2C = ZeroXorCard;
							}
							else if (deck[i] == "XOR1")
							{
								CARD3_P2C = OneOrCard;
							}
						}

						else if (i == 7)
						{
							if (deck[i] == "OR0")
							{
								CARD4_P2C = ZeroOrCard;
							}
							else if (deck[i] == "OR1")
							{
								CARD4_P2C = OneOrCard;
							}
							else if (deck[i] == "AND0")
							{
								CARD4_P2C = ZeroAndCard;
							}
							else if (deck[i] == "AND1")
							{
								CARD4_P2C = OneAndCard;
							}
							else if (deck[i] == "XOR0")
							{
								CARD4_P2C = ZeroXorCard;
							}
							else if (deck[i] == "XOR1")
							{
								CARD4_P2C = OneOrCard;
							}
						}
					}
				}

				ENTITY CARD1_P1(75, 450 ,CARD1_P1C);
				ENTITY CARD2_P1(75, 570, CARD2_P1C);
				ENTITY CARD3_P1(160, 450, CARD3_P1C);
				ENTITY CARD4_P1(160, 570, CARD4_P1C);

				ENTITY CARD1_P2(77, 40, CARD1_P2C);
				ENTITY CARD2_P2(77, 160, CARD2_P2C);
				ENTITY CARD3_P2(160, 40, CARD3_P2C);
				ENTITY CARD4_P2(160, 160, CARD4_P2C);

				ENTITY DRAWDECK(160, 305, BackCard);
				window.render(DRAWDECK);

				window.render(CARD1_P1);
				window.render(CARD2_P1);
				window.render(CARD3_P1);
				window.render(CARD4_P1);

				window.render(CARD1_P2);
				window.render(CARD2_P2);
				window.render(CARD3_P2);
				window.render(CARD4_P2);
			}
			else if (buttonPressed == 2)
			{
				window.render(PvCnotButton);
			}
			else if (buttonPressed == 3)
			{
				window.render(PvCnotButton);
			}
			else if (buttonPressed == 4)
			{
				window.render(PvCnotButton);
			}
			
		}



		window.display();
		
		int frameTicks = SDL_GetTicks() - startTicks;
		if (frameTicks < 1000 / window.getRefreshRate())
		{
			SDL_Delay(1000 / window.getRefreshRate() - frameTicks);
		}
	}
	window.cleanUp();
	SDL_Quit();

	return 0;
}


void ShowHome(RenderWindow& window, ENTITY& mainScreen, ENTITY& Button1, ENTITY& Button2, ENTITY& Button3, ENTITY& Button4, bool& home, int& buttonPressed)
{
	window.render(mainScreen);
	window.render(Button1);
	window.render(Button2);
	window.render(Button3);
	window.render(Button4);
	int buttonX = 0, buttonY = 0;
	Uint32 buttons;
	SDL_PumpEvents();  // make sure we have the latest mouse state.
	buttons = SDL_GetMouseState(&buttonX, &buttonY);
	if ((buttons & SDL_BUTTON_LMASK) != 0) {
		int width = Button1.getCurrentFrame().w;
		int height = Button1.getCurrentFrame().h;
		if (Button1.getX() <= buttonX && buttonX <= Button1.getX() + width
			&& Button1.getY() <= buttonY && buttonY <= Button1.getY() + height) 
		{
			home = false; //link to PvC
			buttonPressed = 1;

		}
		else if (Button2.getX() <= buttonX && buttonX <= Button2.getX() + width
			&& Button2.getY() <= buttonY && buttonY <= Button2.getY() + height)
		{
			home = false;
			buttonPressed = 2;
		}
		else if (Button3.getX() <= buttonX && buttonX <= Button3.getX() + width
			&& Button3.getY() <= buttonY && buttonY <= Button3.getY() + height)
		{
			home = false;
			buttonPressed = 3;
		}
		else if (Button4.getX() <= buttonX && buttonX <= Button4.getX() + width
			&& Button4.getY() <= buttonY && buttonY <= Button4.getY() + height)
		{
			home = false;
			buttonPressed = 4;
		}

	}


}

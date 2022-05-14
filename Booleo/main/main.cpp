#include "RenderWindow.hpp"
#include "entity.hpp"
#include "libraries.hpp"
#include "utilities.hpp"
#include "entity.hpp"
#include "main.hpp"
#include "GamePlay.hpp"

std::string P1hand[10];
std::string P2hand[10];
std::string deck[48];

SDL_Rect* selectedRect = NULL;
bool leftMouseButtonDown = false;
SDL_Point mousePos;

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

	std::string initialCards[6] = { "ZEROONE","ZEROZERO", "ONEONE", "ONEZERO", "ONEZERO2", "ZEROONE2" };

	for (int i = 0; i < 6; i++)
	{
		int index = rand() % 6;
		std::string temp = initialCards[i];
		initialCards[i] = initialCards[index];
		initialCards[index] = temp;
	}


	SDL_Texture* mainMenu = window.loadTexture("../assets/mainScreen.png");
	SDL_Texture* playmat = window.loadTexture("../assets/playmat.png");
	SDL_Texture* PlayerVisBlock = window.loadTexture("../assets/turnCoverer.png");
	SDL_Texture* PvC = window.loadTexture("../assets/PvC.png");
	SDL_Texture* PvCnot = window.loadTexture("../assets/PvCnot.png");
	SDL_Texture* PvP = window.loadTexture("../assets/PvP.png");
	SDL_Texture* PvPnot = window.loadTexture("../assets/PvPnot.png");

	SDL_Texture* BackCard = window.loadTexture("../assets/BackCard.png");
	SDL_Texture* DeckAsset = window.loadTexture("../assets/CardDeck.png");

	SDL_Texture* NotCard = window.loadTexture("../assets/NotCard.png");
	SDL_Texture* OneAndCard = window.loadTexture("../assets/OneAndCard.png");
	SDL_Texture* OneOrCard = window.loadTexture("../assets/OneOrCard.png");
	SDL_Texture* OneXorCard = window.loadTexture("../assets/OneXorCard.png");
	SDL_Texture* ZeroAndCard = window.loadTexture("../assets/ZeroAndCard.png");
	SDL_Texture* ZeroOrCard = window.loadTexture("../assets/ZeroOrCard.png");
	SDL_Texture* ZeroXorCard = window.loadTexture("../assets/ZeroXorCard.png");

	SDL_Texture* ZeroOne = window.loadTexture("../assets/StartingCardZeroOne.png");
	SDL_Texture* ZeroZero = window.loadTexture("../assets/StartingCardZeroZero.png");
	SDL_Texture* OneOne = window.loadTexture("../assets/StartingCardOneOne.png");
	SDL_Texture* OneZero = window.loadTexture("../assets/StartingCardOneZero.png");

	SDL_Renderer* renderer = SDL_CreateRenderer(window.getWindow(), -1, 0);

	ENTITY mainScreen(0, 0, mainMenu);
	ENTITY playmatScreen(0, 0, playmat);
	ENTITY playerBlockRect(0, 0, PlayerVisBlock);

	bool gameRunning = true;
	bool isP1turn = true;
	SDL_Event event;
	ENTITY PvCButton(360, 250, PvC);
	ENTITY PvCnotButton(377, 363, PvCnot);
	ENTITY PvPButton(385, 495, PvP);
	ENTITY PvPnotButton(396, 575, PvPnot);

	const float timeAdvanced = 0.01;
	float accumulator = 0.0;
	float currentTime = utils::hireTimeInSeconds();

	int tempCount = 0; //h

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
				SDL_Texture* INITCARD1 = NULL;
				SDL_Texture* INITCARD2 = NULL;
				SDL_Texture* INITCARD3 = NULL;
				SDL_Texture* INITCARD4 = NULL;
				SDL_Texture* INITCARD5 = NULL;

				SDL_Texture* LINE4CARD1 = NULL;
				SDL_Texture* LINE4CARD2 = NULL;
				SDL_Texture* LINE4CARD3 = NULL;
				SDL_Texture* LINE4CARD4 = NULL;


				SDL_Texture* LINE3CARD1 = NULL;
				SDL_Texture* LINE3CARD2 = NULL;
				SDL_Texture* LINE3CARD3 = NULL;

				SDL_Texture* LINE2CARD1 = NULL;
				SDL_Texture* LINE2CARD2 = NULL;

				SDL_Texture* LINE1CARD1 = NULL;

				SDL_Texture* CARD1_P1C = NULL;
				SDL_Texture* CARD2_P1C = NULL;
				SDL_Texture* CARD3_P1C = NULL;
				SDL_Texture* CARD4_P1C = NULL;

				SDL_Texture* CARD1_P2C = NULL;
				SDL_Texture* CARD2_P2C = NULL;
				SDL_Texture* CARD3_P2C = NULL;
				SDL_Texture* CARD4_P2C = NULL;


				for (int i = 0; i < 5; i++)
				{
					if (i == 0)
					{
						if (initialCards[i] == "ZEROONE" || initialCards[i] == "ZEROONE2")
						{
							INITCARD1 = ZeroOne;

						}
						else if (initialCards[i] == "ZEROZERO")
						{
							INITCARD1 = ZeroZero;
						}
						else if (initialCards[i] == "ZEROZERO")
						{
							INITCARD1 = ZeroZero;
						}
						else if (initialCards[i] == "ONEONE")
						{
							INITCARD1 = OneOne;
						}
						else if (initialCards[i] == "ONEZERO" || initialCards[i] == "ONEZERO2")
						{
							INITCARD1 = OneZero;
						}
					}

					if (i == 1)
					{
						if (initialCards[i] == "ZEROONE" || initialCards[i] == "ZEROONE2")
						{
							INITCARD2 = ZeroOne;
						}
						else if (initialCards[i] == "ZEROZERO")
						{
							INITCARD2 = ZeroZero;
						}
						else if (initialCards[i] == "ZEROZERO")
						{
							INITCARD2 = ZeroZero;
						}
						else if (initialCards[i] == "ONEONE")
						{
							INITCARD2 = OneOne;
						}
						else if (initialCards[i] == "ONEZERO" || initialCards[i] == "ONEZERO2")
						{
							INITCARD2 = OneZero;
						}
					}

					if (i == 2)
					{
						if (initialCards[i] == "ZEROONE" || initialCards[i] == "ZEROONE2")
						{
							INITCARD3 = ZeroOne;
						}
						else if (initialCards[i] == "ZEROZERO")
						{
							INITCARD3 = ZeroZero;
						}
						else if (initialCards[i] == "ZEROZERO")
						{
							INITCARD3 = ZeroZero;
						}
						else if (initialCards[i] == "ONEONE")
						{
							INITCARD3 = OneOne;
						}
						else if (initialCards[i] == "ONEZERO" || initialCards[i] == "ONEZERO2")
						{
							INITCARD3 = OneZero;
						}
					}

					if (i == 3)
					{
						if (initialCards[i] == "ZEROONE" || initialCards[i] == "ZEROONE2")
						{
							INITCARD4 = ZeroOne;
						}
						else if (initialCards[i] == "ZEROZERO")
						{
							INITCARD4 = ZeroZero;
						}
						else if (initialCards[i] == "ZEROZERO")
						{
							INITCARD4 = ZeroZero;
						}
						else if (initialCards[i] == "ONEONE")
						{
							INITCARD4 = OneOne;
						}
						else if (initialCards[i] == "ONEZERO" || initialCards[i] == "ONEZERO2")
						{
							INITCARD4 = OneZero;
						}
					}

					if (i == 4)
					{
						if (initialCards[i] == "ZEROONE" || initialCards[i] == "ZEROONE2")
						{
							INITCARD5 = ZeroOne;
						}
						else if (initialCards[i] == "ZEROZERO")
						{
							INITCARD5 = ZeroZero;
						}
						else if (initialCards[i] == "ZEROZERO")
						{
							INITCARD5 = ZeroZero;
						}
						else if (initialCards[i] == "ONEONE")
						{
							INITCARD5 = OneOne;
						}
						else if (initialCards[i] == "ONEZERO" || initialCards[i] == "ONEZERO2")
						{
							INITCARD5 = OneZero;
						}
					}

				}


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
						else if (deck[i] == "XOR1") //here
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

				ENTITY INIT1(360, 305, INITCARD1);
				ENTITY INIT2(487, 305, INITCARD2);
				ENTITY INIT3(614, 305, INITCARD3);
				ENTITY INIT4(745, 305, INITCARD4);
				ENTITY INIT5(873, 305, INITCARD5);

				ENTITY L4_CARD1(430, 420, LINE4CARD1);
				ENTITY L4_CARD2(560, 420, LINE4CARD2);
				ENTITY L4_CARD3(590, 420, LINE4CARD3);
				ENTITY L4_CARD4(800, 420, LINE4CARD4);
				ENTITY L3_CARD1(490, 485, LINE3CARD1);
				ENTITY L3_CARD2(620, 485, LINE3CARD2);
				ENTITY L3_CARD3(746, 465, LINE3CARD3);
				ENTITY L2_CARD1(560, 570, LINE2CARD1);
				ENTITY L2_CARD2(690, 570, LINE2CARD2);
				ENTITY L1_CARD1(620, 615, LINE1CARD1);

				
				static ENTITY CARD1_P1(75, 450, CARD1_P1C);
				static ENTITY CARD2_P1(75, 570, CARD2_P1C);
				static ENTITY CARD3_P1(160, 450, CARD3_P1C);
				static ENTITY CARD4_P1(160, 570, CARD4_P1C);

				if (CARD1_P1.isMouseClicked())
				{
					CARD1_P1.setDraggable(!CARD1_P1.getDrag());
				}
				//Check avilabilaty of CARD1 on line 4 card1
				if (CARD1_P1.getX() >= 425 && CARD1_P1.getX() <= 435 && CARD1_P1.getY() >= 415 && CARD1_P1.getY() <= 425)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if (!(INITCARD1 == ZeroZero && INITCARD1 == OneZero) || !(INITCARD2 == OneZero && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == OneOrCard)
					{
						if ((INITCARD1 != ZeroOne && INITCARD1 != ZeroZero) && (INITCARD2 == ZeroOne && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == OneZero || INITCARD2 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((INITCARD1 == OneZero || INITCARD1 == ZeroZero) || (INITCARD2 == OneZero && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (INITCARD1 != INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (INITCARD1 == INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
				}

				//Check avilabilaty of CARD1 on line 4 card2
				if (CARD1_P1.getX() >= 555 && CARD1_P1.getX() <= 665 && CARD1_P1.getY() >= 415 && CARD1_P1.getY() <= 425)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if (!(INITCARD2 == ZeroZero && INITCARD2 == OneZero) && !(INITCARD3 == OneZero && INITCARD3 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{ 
							LINE4CARD1 = ZeroOrCard;
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							//CARD1_P1 cahnges stoinost
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if  (!(INITCARD2 == ZeroOne || INITCARD2 == ZeroZero) && !(INITCARD3 == ZeroOne && INITCARD3 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((INITCARD2 == ZeroOne || INITCARD2 == OneOne) && (INITCARD3 == ZeroOne || INITCARD3 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) || (INITCARD3 == OneZero && INITCARD3 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (INITCARD2 != INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (INITCARD2 == INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
				}

				//Check avilabilaty of CARD1 on line 4 card3
				if (CARD1_P1.getX() >= 685 && CARD1_P1.getX() <= 695 && CARD1_P1.getY() >= 415 && CARD1_P1.getY() <= 425)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if (!(INITCARD3 == ZeroZero && INITCARD3 == OneZero) && !(INITCARD4 == OneZero && INITCARD4 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if  (!(INITCARD3 == ZeroOne || INITCARD3 == ZeroZero) && !(INITCARD4 == ZeroOne && INITCARD4 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD4 == OneZero || INITCARD4 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((INITCARD3 == OneZero || INITCARD3 == ZeroZero) || (INITCARD4 == OneZero && INITCARD4 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (INITCARD3 != INITCARD4)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (INITCARD3 == INITCARD4)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
				}

				//Check avilabilaty of CARD1 on line 4 card4
				if (CARD1_P1.getX() >= 795 && CARD1_P1.getX() <= 805 && CARD1_P1.getY() >= 415 && CARD1_P1.getY() <= 425)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if (!(INITCARD4 == ZeroZero && INITCARD5 == OneZero) && !(INITCARD2 == OneZero && INITCARD5 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if  (!(INITCARD4 == ZeroOne || INITCARD4 == ZeroZero) && !(INITCARD5 == ZeroOne && INITCARD5 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD5 == ZeroOne || INITCARD5 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((INITCARD4 == OneZero || INITCARD4 == ZeroZero) && (INITCARD5 == OneZero && INITCARD5 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (INITCARD4 != INITCARD5)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (INITCARD4 == INITCARD5)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
				}

				//Check avilabilaty of CARD1 on line 3 card1
				if (CARD1_P1.getX() >= 485 && CARD1_P1.getX() <= 495 && CARD1_P1.getY() >= 480 && CARD1_P1.getY() <= 490)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if (!(INITCARD1 == ZeroZero && INITCARD1 == OneZero) && !(INITCARD2 == OneZero && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if  (!(INITCARD1 == ZeroOne || INITCARD1 == ZeroZero) && !(INITCARD2 == ZeroOne && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == OneZero || INITCARD2 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((INITCARD1 == OneZero || INITCARD1 == ZeroZero) || (INITCARD2 == OneZero && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (INITCARD1 != INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (INITCARD1 == INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
				}

				//Check avilabilaty of CARD1 on line 3 card2
				if (CARD1_P1.getX() >= 615 && CARD1_P1.getX() <= 625 && CARD1_P1.getY() >= 480 && CARD1_P1.getY() <= 490)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if (!(INITCARD1 == ZeroZero && INITCARD1 == OneZero) && !(INITCARD2 == OneZero && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if  (!(INITCARD1 == ZeroOne || INITCARD1 == ZeroZero) && !(INITCARD2 == ZeroOne && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == OneZero || INITCARD2 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((INITCARD1 == OneZero || INITCARD1 == ZeroZero) || (INITCARD2 == OneZero && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (INITCARD1 != INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (INITCARD1 == INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
				}

				//Check avilabilaty of CARD1 on line 3 card3
				if (CARD1_P1.getX() >= 740 && CARD1_P1.getX() <= 750 && CARD1_P1.getY() >= 480 && CARD1_P1.getY() <= 490)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if (!(INITCARD1 == ZeroZero && INITCARD1 == OneZero) && !(INITCARD2 == OneZero && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if  (!(INITCARD1 == ZeroOne || INITCARD1 == ZeroZero) && !(INITCARD2 == ZeroOne && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == OneZero || INITCARD2 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((INITCARD1 == OneZero || INITCARD1 == ZeroZero) || (INITCARD2 == OneZero && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (INITCARD1 != INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (INITCARD1 == INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
				}

				//Check avilabilaty of CARD1 on line 2 card1
				if (CARD1_P1.getX() >= 555 && CARD1_P1.getX() <= 560 && CARD1_P1.getY() >= 565 && CARD1_P1.getY() <= 575)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if (!(INITCARD1 == ZeroZero && INITCARD1 == OneZero) && !(INITCARD2 == OneZero && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if  (!(INITCARD1 == ZeroOne || INITCARD1 == ZeroZero) && !(INITCARD2 == ZeroOne && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == OneZero || INITCARD2 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((INITCARD1 == OneZero || INITCARD1 == ZeroZero) || (INITCARD2 == OneZero && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (INITCARD1 != INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (INITCARD1 == INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
				}

				//Check avilabilaty of CARD1 on line 2 card2
				if (CARD1_P1.getX() >= 685 && CARD1_P1.getX() <= 695 && CARD1_P1.getY() >= 565 && CARD1_P1.getY() <= 575)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if (!(INITCARD1 == ZeroZero && INITCARD1 == OneZero) && !(INITCARD2 == OneZero && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if  (!(INITCARD1 == ZeroOne || INITCARD1 == ZeroZero) && !(INITCARD2 == ZeroOne && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == OneZero || INITCARD2 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((INITCARD1 == OneZero || INITCARD1 == ZeroZero) || (INITCARD2 == OneZero && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (INITCARD1 != INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (INITCARD1 == INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
				}

				//Check avilabilaty of CARD1 on line 1 card1
				if (CARD1_P1.getX() >= 615 && CARD1_P1.getX() <= 625 && CARD1_P1.getY() >= 615 && CARD1_P1.getY() <= 620)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if (!(INITCARD1 == ZeroZero && INITCARD1 == OneZero) && !(INITCARD2 == OneZero && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if  (!(INITCARD1 == ZeroOne || INITCARD1 == ZeroZero) && !(INITCARD2 == ZeroOne && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == OneZero || INITCARD2 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((INITCARD1 == OneZero || INITCARD1 == ZeroZero) || (INITCARD2 == OneZero && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (INITCARD1 != INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (INITCARD1 == INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}

					}
				}




				CARD1_P1.drag();

				//CARD3_P1.setDraggable(true);
				//CARD4_P1.setDraggable(true);

				ENTITY CARD1_P2(77, 40, CARD1_P2C);
				ENTITY CARD2_P2(77, 160, CARD2_P2C);
				ENTITY CARD3_P2(160, 40, CARD3_P2C);
				ENTITY CARD4_P2(160, 160, CARD4_P2C);

				ENTITY DRAWDECK(125, 303, DeckAsset);
				window.render(DRAWDECK);

				window.render(INIT1);
				window.render(INIT2);
				window.render(INIT3);
				window.render(INIT4);
				window.render(INIT5);

				window.render(CARD1_P1);
				window.render(CARD2_P1);
				window.render(CARD3_P1);
				window.render(CARD4_P1);

				window.render(CARD1_P2);
				window.render(CARD2_P2);
				window.render(CARD3_P2);
				window.render(CARD4_P2);

				window.render(playerBlockRect);

				if (CARD1_P1.getX() != 75)
				{
					isP1turn = false;
				}
				if (isP1turn == false)
				{
					playerBlockRect.setY(360);
					SDL_RenderCopy(renderer, PlayerVisBlock, NULL, NULL);
				}

				while (tempCount != 48) {
					std::cout << deck[tempCount] << std::endl;
					tempCount++;
				}
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
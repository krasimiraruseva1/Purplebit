#include "RenderWindow.hpp"
#include "entity.hpp"
#include "libraries.hpp" // include libraries
#include "utilities.hpp"
#include "entity.hpp"
// include header files
#include "main.hpp" 
#include <Windows.h>

// declare player cards in hand and deck
std::string P1hand[10]; 
std::string P2hand[10];
CARD deck[48];
int drawCount = 8;

// declares if left mouse button is pressed
SDL_Rect* selectedRect = NULL;
bool leftMouseButtonDown = false; 

// checks mouse position
SDL_Point mousePos; 

int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
) 
{
	// if program fails to initialize shows error
	if (SDL_Init(SDL_INIT_VIDEO) > 0) 
	{
		std::cout << "SDL Error: " << SDL_GetError() << std::endl; 
	}

	// if image fails to initialize shows error
	if (!(IMG_Init(IMG_INIT_PNG)))
	{
		std::cout << "IMG Error: " << SDL_GetError() << std::endl; 
	}

	// renders window named "Booleo" with size 1280x720
	RenderWindow window("Booleo", 1280, 720);	
	bool home = true;
	int buttonPressed = NULL;

	SDL_Texture* mainMenu = window.loadTexture("../assets/mainScreen.png"); // loads larger textures
	SDL_Texture* playmat = window.loadTexture("../assets/playmat.png");
	SDL_Texture* PlayerVisBlock = window.loadTexture("../assets/turnCoverer.png");
	SDL_Texture* PvC = window.loadTexture("../assets/PvC.png");
	SDL_Texture* PvCnot = window.loadTexture("../assets/PvCnot.png");
	SDL_Texture* PvP = window.loadTexture("../assets/PvP.png");
	SDL_Texture* PvPnot = window.loadTexture("../assets/PvPnot.png");
	SDL_Texture* ComingSoon = window.loadTexture("../assets/ComingSoonScreen.png");

	SDL_Texture* BackCard = window.loadTexture("../assets/BackCard.png"); // loads card textures
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

	SDL_Texture* PyrCardD = window.loadTexture("../assets/PyrCardD.png");
	SDL_Texture* PyrCardL = window.loadTexture("../assets/PyrCardL.png");
	SDL_Texture* PyrCardD_flip = window.loadTexture("../assets/PyrCardD_flip.png");
	SDL_Texture* PyrCardL_flip = window.loadTexture("../assets/PyrCardL_flip.png");

	// declares random number
	srand(time(0)); 

	// shuffles deck
	for (int i = 0; i < 48; i++)
	{
		if (i <= 7)
		{
			deck[i].name = "OR0";
			deck[i].texture = ZeroOrCard;
		}
		else if (i > 7 && i <= 15)
		{
			deck[i].name = "OR1";
			deck[i].texture = OneOrCard;
		}
		else if (i > 15 && i <= 23)
		{
			deck[i].name = "AND0";
			deck[i].texture = ZeroAndCard;
		}
		else if (i > 23 && i <= 31)
		{
			deck[i].name = "AND1";
			deck[i].texture = OneAndCard;
		}
		else if (i > 31 && i <= 39)
		{
			deck[i].name = "XOR0";
			deck[i].texture = ZeroXorCard;
		}
		else if (i > 39 && i <= 47)
		{
			deck[i].name = "XOR1";
			deck[i].texture = OneXorCard;
		}
	}
	for (int i = 0; i < 48; i++)
	{
		int index = rand() % 48;
		std::swap(deck[i], deck[index]);
	}

	std::string initialCards[6] = { "ZeroOne","ZeroZero", "OneOne", "OneZero", "OneZero2", "ZeroOne2" }; // declares initial cards

	// shuffles initial cards
	for (int i = 0; i < 6; i++) 
	{
		int index = rand() % 6;
		std::string temp = initialCards[i];
		initialCards[i] = initialCards[index];
		initialCards[index] = temp;
	}

	// declares entities to load proper texture with
	ENTITY mainScreen(0, 0, mainMenu);
	ENTITY playmatScreen(0, 0, playmat);
	ENTITY playerBlockRect(0, 0, PlayerVisBlock);
	ENTITY comingSoonScreen(0, 0, ComingSoon);

	// declares if game is running
	bool gameRunning = true;

	// declares proper player's turn
	bool isP1Turn = true; 

	// declares event (something on screen happening)
	SDL_Event event; 

	// declares button entities
	ENTITY PvPButton(385, 280, PvP); 
	ENTITY PvCButton(380, 350, PvC); 
	ENTITY PvCnotButton(385, 465, PvCnot);
	ENTITY PvPnotButton(396, 575, PvPnot);

	// declares ttools for game optimization
	const float timeAdvanced = 0.01; 
	float accumulator = 0.0;

	// declares current time
	float currentTime = utils::hireTimeInSeconds();

	// initializes game cycle
	while (gameRunning) 
	{
		// starts ticks
		int startTicks = SDL_GetTicks(); 
		float newTime = utils::hireTimeInSeconds();
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		// limits cpu usage
		while (accumulator >= timeAdvanced) 
		{

			// lets player stop program by closing window
			while (SDL_PollEvent(&event)) 
			{
				if (event.type == SDL_QUIT)
					gameRunning = false;
			}
			accumulator -= timeAdvanced;
		}
		const float alpha = accumulator / timeAdvanced;

		// clears window
		window.clear(); 
		if (home)
		{ 
			// prints home screen
			ShowHome(window, mainScreen, PvCButton, PvCnotButton, PvPButton, PvPnotButton, home, buttonPressed);
		}
		else
		{
			// clears window screen
			window.clear(); 

			//prints playmat
			window.render(playmatScreen); 
			if (buttonPressed == 1)
			{
				// declares initial card textures
				SDL_Texture* INITCARD1 = NULL; 
				SDL_Texture* INITCARD2 = NULL;
				SDL_Texture* INITCARD3 = NULL;
				SDL_Texture* INITCARD4 = NULL;
				SDL_Texture* INITCARD5 = NULL;

				SDL_Texture* RULECARD = window.loadTexture("../assets/RuleCard.png");

				SDL_Texture* LINE4CARD1 = PyrCardD;
				SDL_Texture* LINE4CARD2 = PyrCardD;
				SDL_Texture* LINE4CARD3 = PyrCardD;
				SDL_Texture* LINE4CARD4 = PyrCardD;
									   
				SDL_Texture* LINE3CARD1 = PyrCardL;
				SDL_Texture* LINE3CARD2 = PyrCardL;
				SDL_Texture* LINE3CARD3 = PyrCardL;
									   
				SDL_Texture* LINE2CARD1 = PyrCardD;
				SDL_Texture* LINE2CARD2 = PyrCardD;
									   
				SDL_Texture* LINE1CARD1 = PyrCardL;
									   
				SDL_Texture* LINE4CARD1f = PyrCardD_flip;
				SDL_Texture* LINE4CARD2f = PyrCardD_flip;
				SDL_Texture* LINE4CARD3f = PyrCardD_flip;
				SDL_Texture* LINE4CARD4f = PyrCardD_flip;
									   
				SDL_Texture* LINE3CARD1f = PyrCardL_flip;
				SDL_Texture* LINE3CARD2f = PyrCardL_flip;
				SDL_Texture* LINE3CARD3f = PyrCardL_flip;
									   
				SDL_Texture* LINE2CARD1f = PyrCardD_flip;
				SDL_Texture* LINE2CARD2f = PyrCardD_flip;
									   
				SDL_Texture* LINE1CARD1f = PyrCardL_flip;

				// declares cards in hand textures
				SDL_Texture* CARD1_P1C = NULL; 
				SDL_Texture* CARD2_P1C = NULL;
				SDL_Texture* CARD3_P1C = NULL;
				SDL_Texture* CARD4_P1C = NULL;

				SDL_Texture* CARD1_P2C = NULL;
				SDL_Texture* CARD2_P2C = NULL;
				SDL_Texture* CARD3_P2C = NULL;
				SDL_Texture* CARD4_P2C = NULL;

				// gives random value to deck cards
				for (int i = 0; i < 5; i++) 
				{
					if (i == 0)
					{
						if (initialCards[i] == "ZeroOne" || initialCards[i] == "ZeroOne2")
						{
							INITCARD1 = ZeroOne;

						}
						else if (initialCards[i] == "ZeroZero")
						{
							INITCARD1 = ZeroZero;
						}
						else if (initialCards[i] == "ZeroZero")
						{
							INITCARD1 = ZeroZero;
						}
						else if (initialCards[i] == "OneOne")
						{
							INITCARD1 = OneOne;
						}
						else if (initialCards[i] == "OneZero" || initialCards[i] == "OneZero2")
						{
							INITCARD1 = OneZero;
						}
					}

					if (i == 1)
					{
						if (initialCards[i] == "ZeroOne" || initialCards[i] == "ZeroOne2")
						{
							INITCARD2 = ZeroOne;
						}
						else if (initialCards[i] == "ZeroZero")
						{
							INITCARD2 = ZeroZero;
						}
						else if (initialCards[i] == "ZeroZero")
						{
							INITCARD2 = ZeroZero;
						}
						else if (initialCards[i] == "OneOne")
						{
							INITCARD2 = OneOne;
						}
						else if (initialCards[i] == "OneZero" || initialCards[i] == "OneZero2")
						{
							INITCARD2 = OneZero;
						}
					}

					if (i == 2)
					{
						if (initialCards[i] == "ZeroOne" || initialCards[i] == "ZeroOne2")
						{
							INITCARD3 = ZeroOne;
						}
						else if (initialCards[i] == "ZeroZero")
						{
							INITCARD3 = ZeroZero;
						}
						else if (initialCards[i] == "ZeroZero")
						{
							INITCARD3 = ZeroZero;
						}
						else if (initialCards[i] == "OneOne")
						{
							INITCARD3 = OneOne;
						}
						else if (initialCards[i] == "OneZero" || initialCards[i] == "OneZero2")
						{
							INITCARD3 = OneZero;
						}
					}

					if (i == 3)
					{
						if (initialCards[i] == "ZeroOne" || initialCards[i] == "ZeroOne2")
						{
							INITCARD4 = ZeroOne;
						}
						else if (initialCards[i] == "ZeroZero")
						{
							INITCARD4 = ZeroZero;
						}
						else if (initialCards[i] == "ZeroZero")
						{
							INITCARD4 = ZeroZero;
						}
						else if (initialCards[i] == "OneOne")
						{
							INITCARD4 = OneOne;
						}
						else if (initialCards[i] == "OneZero" || initialCards[i] == "OneZero2")
						{
							INITCARD4 = OneZero;
						}
					}

					if (i == 4)
					{
						if (initialCards[i] == "ZeroOne" || initialCards[i] == "ZeroOne2")
						{
							INITCARD5 = ZeroOne;
						}
						else if (initialCards[i] == "ZeroZero")
						{
							INITCARD5 = ZeroZero;
						}
						else if (initialCards[i] == "ZeroZero")
						{
							INITCARD5 = ZeroZero;
						}
						else if (initialCards[i] == "OneOne")
						{
							INITCARD5 = OneOne;
						}
						else if (initialCards[i] == "OneZero" || initialCards[i] == "OneZero2")
						{
							INITCARD5 = OneZero;
						}
					}

				}


				for (int i = 0; i < 8; i++)
				{
					if (i == 0)
					{
						if (deck[i].name == "OR0")
						{
							CARD1_P1C = ZeroOrCard;
							deck[i].texture = ZeroOrCard;
						}
						else if (deck[i].name == "OR1")
						{
							CARD1_P1C = OneOrCard;
						}
						else if (deck[i].name == "AND0")
						{
							CARD1_P1C = ZeroAndCard;

						}
						else if (deck[i].name == "AND1")
						{
							CARD1_P1C = OneAndCard;

						}
						else if (deck[i].name == "XOR0")
						{
							CARD1_P1C = ZeroXorCard;

						}
						else if (deck[i].name == "XOR1")
						{
							CARD1_P1C = OneOrCard;

						}



					}
					else if (i == 1)
					{
						if (deck[i].name == "OR0")
						{
							CARD2_P1C = ZeroOrCard;
						}
						else if (deck[i].name == "OR1")
						{
							CARD2_P1C = OneOrCard;
						}
						else if (deck[i].name == "AND0")
						{
							CARD2_P1C = ZeroAndCard;
						}
						else if (deck[i].name == "AND1")
						{
							CARD2_P1C = OneAndCard;
						}
						else if (deck[i].name == "XOR0")
						{
							CARD2_P1C = ZeroXorCard;
						}
						else if (deck[i].name == "XOR1")
						{
							CARD2_P1C = OneOrCard;
						}
					}

					else if (i == 2)
					{
						if (deck[i].name == "OR0")
						{
							CARD3_P1C = ZeroOrCard;
						}
						else if (deck[i].name == "OR1")
						{
							CARD3_P1C = OneOrCard;
						}
						else if (deck[i].name == "AND0")
						{
							CARD3_P1C = ZeroAndCard;
						}
						else if (deck[i].name == "AND1")
						{
							CARD3_P1C = OneAndCard;
						}
						else if (deck[i].name == "XOR0")
						{
							CARD3_P1C = ZeroXorCard;
						}
						else if (deck[i].name == "XOR1")
						{
							CARD3_P1C = OneOrCard;
						}
					}
					else if (i == 3)
					{
						if (deck[i].name == "OR0")
						{
							CARD4_P1C = ZeroOrCard;
						}
						else if (deck[i].name == "OR1")
						{
							CARD4_P1C = OneOrCard;
						}
						else if (deck[i].name == "AND0")
						{
							CARD4_P1C = ZeroAndCard;
						}
						else if (deck[i].name == "AND1")
						{
							CARD4_P1C = OneAndCard;
						}
						else if (deck[i].name == "XOR0")
						{
							CARD4_P1C = ZeroXorCard;
						}
						else if (deck[i].name == "XOR1")
						{
							CARD4_P1C = OneOrCard;
						}

					}
					for (int i = 4; i < 8; i++)
					{
						if (i == 4)
						{
							if (deck[i].name == "OR0")
							{
								CARD1_P2C = ZeroOrCard;
							}
							else if (deck[i].name == "OR1")
							{
								CARD1_P2C = OneOrCard;
							}
							else if (deck[i].name == "AND0")
							{
								CARD1_P2C = ZeroAndCard;
							}
							else if (deck[i].name == "AND1")
							{
								CARD1_P2C = OneAndCard;
							}
							else if (deck[i].name == "XOR0")
							{
								CARD1_P2C = ZeroXorCard;
							}
							else if (deck[i].name == "XOR1")
							{
								CARD1_P2C = OneOrCard;
							}
						}

						else if (i == 5)
						{
							if (deck[i].name == "OR0")
							{
								CARD2_P2C = ZeroOrCard;
							}
							else if (deck[i].name == "OR1")
							{
								CARD2_P2C = OneOrCard;
							}
							else if (deck[i].name == "AND0")
							{
								CARD2_P2C = ZeroAndCard;
							}
							else if (deck[i].name == "AND1")
							{
								CARD2_P2C = OneAndCard;
							}
							else if (deck[i].name == "XOR0")
							{
								CARD2_P2C = ZeroXorCard;
							}
							else if (deck[i].name == "XOR1")
							{
								CARD2_P2C = OneOrCard;
							}
						}

						else if (i == 6)
						{
							if (deck[i].name == "OR0")
							{
								CARD3_P2C = ZeroOrCard;
							}
							else if (deck[i].name == "OR1")
							{
								CARD3_P2C = OneOrCard;
							}
							else if (deck[i].name == "AND0")
							{
								CARD3_P2C = ZeroAndCard;
							}
							else if (deck[i].name == "AND1")
							{
								CARD3_P2C = OneAndCard;
							}
							else if (deck[i].name == "XOR0")
							{
								CARD3_P2C = ZeroXorCard;
							}
							else if (deck[i].name == "XOR1")
							{
								CARD3_P2C = OneOrCard;
							}
						}

						else if (i == 7)
						{
							if (deck[i].name == "OR0")
							{
								CARD4_P2C = ZeroOrCard;
							}
							else if (deck[i].name == "OR1")
							{
								CARD4_P2C = OneOrCard;
							}
							else if (deck[i].name == "AND0")
							{
								CARD4_P2C = ZeroAndCard;
							}
							else if (deck[i].name == "AND1")
							{
								CARD4_P2C = OneAndCard;
							}
							else if (deck[i].name == "XOR0")
							{
								CARD4_P2C = ZeroXorCard;
							}
							else if (deck[i].name == "XOR1")
							{
								CARD4_P2C = OneOrCard;
							}
						}
					}
				}

				// declares initial cards as entities 
				ENTITY INIT1(360, 305, INITCARD1); 
				ENTITY INIT2(487, 305, INITCARD2);
				ENTITY INIT3(614, 305, INITCARD3);
				ENTITY INIT4(745, 305, INITCARD4);
				ENTITY INIT5(873, 305, INITCARD5);

				ENTITY RuleCard(1050, 220, RULECARD);
				window.render(RuleCard);

				static ENTITY L4_CARD4(800, 420, LINE4CARD4);
				static ENTITY L4_CARD3(685, 420, LINE4CARD3);
				static ENTITY L4_CARD2(560, 420, LINE4CARD2);
				static ENTITY L4_CARD1(430, 420, LINE4CARD1);
				static ENTITY L3_CARD3(746, 485, LINE3CARD3);
				static ENTITY L3_CARD2(620, 485, LINE3CARD2);
				static ENTITY L3_CARD1(490, 485, LINE3CARD1);
				static ENTITY L2_CARD2(690, 570, LINE2CARD2);
				static ENTITY L2_CARD1(560, 570, LINE2CARD1);
				static ENTITY L1_CARD1(620, 615, LINE1CARD1);

				static ENTITY L4_CARD4f(800, 200, LINE4CARD4f);
				static ENTITY L4_CARD3f(685, 200, LINE4CARD3f);
				static ENTITY L4_CARD2f(560, 200, LINE4CARD2f);
				static ENTITY L4_CARD1f(430, 200, LINE4CARD1f);
				static ENTITY L3_CARD3f(745, 130, LINE3CARD3f);
				static ENTITY L3_CARD2f(620, 130, LINE3CARD2f);
				static ENTITY L3_CARD1f(490, 130, LINE3CARD1f);
				static ENTITY L2_CARD2f(685, 44, LINE2CARD2f);
				static ENTITY L2_CARD1f(555, 44, LINE2CARD1f);
				static ENTITY L1_CARD1f(620, 1, LINE1CARD1f);

				// declares player cards in hand as entities
				static ENTITY CARD1_P1(75, 450, CARD1_P1C); 
				static ENTITY CARD2_P1(75, 570, CARD2_P1C);
				static ENTITY CARD3_P1(160, 450, CARD3_P1C);
				static ENTITY CARD4_P1(160, 570, CARD4_P1C);

				static ENTITY CARD1_P2(77, 40, CARD1_P2C);
				static ENTITY CARD2_P2(77, 160, CARD2_P2C);
				static ENTITY CARD3_P2(160, 40, CARD3_P2C);
				static ENTITY CARD4_P2(160, 160, CARD4_P2C);

				// makes cards draggable depending on which player's turn it is
				if (isP1Turn)
				{
					// if mouse button is clicked/held
					if (CARD1_P1.isMouseClicked()) 
					{
						// make card draggable
						CARD1_P1.setDraggable(!CARD1_P1.getDrag()); 
						CARD1_P1.drag(); 
					}

					if (CARD2_P1.isMouseClicked())
					{
						CARD2_P1.setDraggable(!CARD1_P1.getDrag());
						CARD2_P1.drag();
					}

					if (CARD3_P1.isMouseClicked())
					{
						CARD3_P1.setDraggable(!CARD1_P1.getDrag());
						CARD3_P1.drag();
					}

					if (CARD4_P1.isMouseClicked())
					{
						CARD4_P1.setDraggable(!CARD1_P1.getDrag());
						CARD4_P1.drag();
					}
				}
				else 
				{
					// Check availability of card1 on line 4 card 1
					if (CARD1_P2.getX() >= 420 && CARD1_P2.getX() <= 440 && CARD1_P2.getY() >= 190 && CARD1_P2.getY() <= 210)
					{
						if (CARD1_P2C == ZeroOrCard)
							if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) || (INITCARD2 == ZeroOne || INITCARD2 == OneOne))
							{
								continue;
							}
							else
							{
								L4_CARD1f.setTexture(CARD1_P2.getTex());
								drawCount++;
								L4_CARD1f.setTexture(CARD1_P2.getTex());
								CARD1_P2.setTexture(deck[drawCount].texture);
								CARD1_P2.setX(77);
								CARD1_P2.setY(40);
								isP1Turn = true;
							}

					}
					if (CARD1_P2C == OneOrCard)
					{
						if ((INITCARD1 != ZeroOne && INITCARD1 != ZeroZero) && (INITCARD2 == ZeroOne && INITCARD2 == ZeroZero))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
					if (CARD1_P2C == ZeroAndCard)
					{
						if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneAndCard)
					{
						if ((INITCARD1 == OneZero || INITCARD1 == ZeroZero) || (INITCARD2 == OneZero || INITCARD2 == ZeroZero))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
					if (CARD1_P2C == ZeroXorCard)
					{
						if (((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne)) || ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) && (INITCARD1 == OneZero || INITCARD1 == ZeroZero)))
						{
							CARD1_P2.setX(L1_CARD1f.getX());
							CARD1_P2.setY(L1_CARD1f.getY());

						}
						else
						{
							L4_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}

					if (CARD1_P2C == OneXorCard)
					{
						if (((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne)) || ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) && (INITCARD1 == OneZero || INITCARD1 == ZeroZero)))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
				}

				// Check availability of card1 on line 4 card 2
				if (CARD1_P2.getX() >= 550 && CARD1_P2.getX() <= 570 && CARD1_P2.getY() >= 190 && CARD1_P2.getY() <= 20)
				{
					if (CARD1_P2C == ZeroOrCard)
					{
						if ((INITCARD3 == ZeroOne || INITCARD3 == OneOne) || (INITCARD4 == ZeroOne || INITCARD4 == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;

						}
					}
					if (CARD1_P2C == OneOrCard)
					{
						if (!(INITCARD2 == ZeroOne || INITCARD2 == ZeroZero) && !(INITCARD3 == ZeroOne && INITCARD3 == ZeroZero))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
					if (CARD1_P2C == ZeroAndCard)
					{
						if ((INITCARD2 == ZeroOne || INITCARD2 == OneOne) && (INITCARD3 == ZeroOne || INITCARD3 == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;


						}
					}
					if (CARD1_P2C == OneAndCard)
					{
						if ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) || (INITCARD3 == OneZero && INITCARD3 == ZeroZero))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}


					}
					if (CARD1_P2C == ZeroXorCard)
					{
						if (INITCARD2 != INITCARD2)
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneXorCard)
					{
						if (INITCARD2 == INITCARD2)
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
				}

				// Check availability of card1 on &line 4 card 3
				if (CARD1_P2.getX() >= 675 && CARD1_P2.getX() <= 695 && CARD1_P2.getY() >= 190 && CARD1_P2.getY() <= 210)
				{
					if (CARD1_P2C == ZeroOrCard)
					{
						if ((INITCARD2 == ZeroOne || INITCARD2 == OneOne) || (INITCARD3 == ZeroOne || INITCARD3 == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD3f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD3f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneOrCard)
					{
						if (!(INITCARD3 == ZeroOne || INITCARD3 == ZeroZero) && !(INITCARD4 == ZeroOne && INITCARD4 == ZeroZero))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD3f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD3f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
					if (CARD1_P2C == ZeroAndCard)
					{
						if ((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD4 == OneZero || INITCARD4 == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD3f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD3f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneAndCard)
					{
						if ((INITCARD3 == OneZero || INITCARD3 == ZeroZero) || (INITCARD4 == OneZero || INITCARD4 == ZeroZero))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD3f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD3f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
					if (CARD1_P2C == ZeroXorCard)
					{
						if (((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD3 == OneZero || INITCARD3 == ZeroZero) && (INITCARD4 == OneZero || INITCARD4 == ZeroZero)))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD3f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD3f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneXorCard)
					{
						if (!((((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD3 == ZeroOne || INITCARD3 == OneOne)) || ((INITCARD4 == OneZero || INITCARD4 == ZeroZero) && (INITCARD4 == OneZero || INITCARD4 == ZeroZero)))))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}//h
						else
						{
							L4_CARD3f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD3f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
				}

				// Check availability of card1 on line 4 card 4
				if (CARD1_P2.getX() >= 790 && CARD1_P2.getX() <= 810 && CARD1_P2.getY() >= 190 && CARD1_P2.getY() <= 210)
				{
					if (CARD1_P2C == ZeroOrCard)
					{
						if ((INITCARD4 == ZeroOne || INITCARD4 == OneOne) || (INITCARD5 == ZeroOne || INITCARD5 == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD4f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD4f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneOrCard)
					{
						if ((INITCARD4 != ZeroOne && INITCARD4 != OneOne) || (INITCARD5 != ZeroOne && INITCARD5 != OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD4f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD4f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
					if (CARD1_P2C == ZeroAndCard)
					{
						if ((INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD5 == ZeroOne || INITCARD5 == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD4f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD4f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneAndCard)
					{
						if ((INITCARD4 == OneZero || INITCARD4 == ZeroZero) && (INITCARD5 == OneZero && INITCARD5 == ZeroZero))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD4f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD4f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
					if (CARD1_P2C == ZeroXorCard)
					{
						if (((
							INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD5 == OneZero || INITCARD5 == ZeroZero) && (INITCARD5 == OneZero || INITCARD5 == ZeroZero)))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD4f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD4f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneXorCard)
					{
						if (!((((INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD5 == OneZero || INITCARD5 == ZeroZero) && (INITCARD5 == OneZero || INITCARD5 == ZeroZero)))))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD4f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD4f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
				}

				// Check availability of card1 on line 3 card 1
				if (CARD1_P2.getX() >= 485 && CARD1_P2.getX() <= 495 && CARD1_P2.getY() >= 480 && CARD1_P2.getY() <= 490)
				{
					if (CARD1_P2C == ZeroOrCard)
					{
						if ((L4_CARD1f.getTex() == ZeroOne || L4_CARD1f.getTex() == OneOne) || (L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneOrCard)
					{
						if ((L4_CARD1f.getTex() != ZeroOne && L4_CARD1f.getTex() != OneOne) || (L4_CARD2f.getTex() != ZeroOne && L4_CARD2f.getTex() != OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == ZeroAndCard)
					{
						if ((L4_CARD1f.getTex() == ZeroOne || L4_CARD1f.getTex() == OneOne) && (L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneAndCard)
					{
						if ((L4_CARD1f.getTex() == OneZero || L4_CARD1f.getTex() == ZeroZero) || (L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
					if (CARD1_P2C == ZeroXorCard)
					{
						if (((L4_CARD1f.getTex() == ZeroOne || L4_CARD1f.getTex() == OneOne) && (L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne)) || ((L4_CARD1f.getTex() == OneZero || L4_CARD1f.getTex() == ZeroZero) && (L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero)))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (!((((L4_CARD1f.getTex() == ZeroOne || L4_CARD1f.getTex() == OneOne) && (L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne)) || ((L4_CARD1f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero) && (L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero)))))
					{
						if (INITCARD1 == INITCARD2)
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
				}

				// Check availability of card1 on line 3 card 2
				if (CARD1_P2.getX() >= 615 && CARD1_P2.getX() <= 625 && CARD1_P2.getY() >= 480 && CARD1_P2.getY() <= 490)
				{
					if (CARD1_P2C == ZeroOrCard)
					{
						if ((L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne) || (L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneOrCard)
					{
						if ((L4_CARD2f.getTex() != ZeroOne && L4_CARD2f.getTex() != OneOne) || (L4_CARD3f.getTex() != ZeroOne && L4_CARD3f.getTex() != OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == ZeroAndCard)
					{
						if ((L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne) && (L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneAndCard)
					{
						if ((L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero) || (L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
					if (CARD1_P2C == ZeroXorCard)
					{
						if (((L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne) && (L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne)) || ((L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero) && (L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero)))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneXorCard)
					{
						if (!((((L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne) && (L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne)) || ((L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero) && (L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero)))))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
				}

				// Check availability of card1 on line 3 card 3
				if (CARD1_P2.getX() >= 740 && CARD1_P2.getX() <= 770 && CARD1_P2.getY() >= 480 && CARD1_P2.getY() <= 490)
				{
					if (CARD1_P2C == ZeroOrCard)
					{
						if ((L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne) && (L4_CARD4f.getTex() == ZeroOne || L4_CARD4f.getTex() == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
					}
					if (CARD1_P2C == OneOrCard)
					{
						if ((L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero) || (L4_CARD4f.getTex() == OneZero || L4_CARD4f.getTex() == ZeroZero))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD3f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD3f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == ZeroAndCard)
					{
						if ((L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne) && (L4_CARD4f.getTex() == ZeroOne || L4_CARD4f.getTex() == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD3f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD3f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneAndCard)
					{
						if ((L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero) || (L4_CARD4f.getTex() == OneZero || L4_CARD4f.getTex() == ZeroZero))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD3f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD3f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
					if (CARD1_P2C == ZeroXorCard)
					{
						if (((L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne) && (L4_CARD4f.getTex() == ZeroOne || L4_CARD4f.getTex() == OneOne)) || ((L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero) && (L4_CARD4f.getTex() == OneZero || L4_CARD4f.getTex() == ZeroZero)))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD3f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD3f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (!((((L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne) && (L4_CARD4f.getTex() == ZeroOne || L4_CARD4f.getTex() == OneOne)) || ((L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero) && (L4_CARD4f.getTex() == OneZero || L4_CARD4f.getTex() == ZeroZero)))))
					{
						if (INITCARD1 == INITCARD2)
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L3_CARD3f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L3_CARD3f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
				}

				// Check availability of card1 on line 2 card 1
				if (CARD1_P2.getX() >= 555 && CARD1_P2.getX() <= 560 && CARD1_P2.getY() >= 565 && CARD1_P2.getY() <= 577)
				{
					if (CARD1_P2C == ZeroOrCard)
					{
						if ((L3_CARD1f.getTex() == ZeroOne || L3_CARD1f.getTex() == OneOne) || (L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L2_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneOrCard)
					{
						if ((L3_CARD1f.getTex() != ZeroOne && L3_CARD1f.getTex() != OneOne) || (L3_CARD2f.getTex() != ZeroOne && L3_CARD2f.getTex() != OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L2_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
					if (CARD1_P2C == ZeroAndCard)
					{
						if ((L3_CARD1f.getTex() == ZeroOne || L3_CARD1f.getTex() == OneOne) && (L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L2_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneAndCard)
					{
						if ((L3_CARD1f.getTex() == OneZero || L3_CARD1f.getTex() == ZeroZero) || (L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L2_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == ZeroXorCard)
					{
						if (((L3_CARD1f.getTex() == ZeroOne || L3_CARD1f.getTex() == OneOne) && (L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne)) || ((L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero) && (L3_CARD1f.getTex() == OneZero || L3_CARD1f.getTex() == ZeroZero)))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L2_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneXorCard)
					{
						if (!((((L3_CARD1f.getTex() == ZeroOne || L3_CARD1f.getTex() == OneOne) && (L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne)) || ((L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero) && (L3_CARD1f.getTex() == OneZero || L3_CARD1f.getTex() == ZeroZero)))))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L2_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
				}

				// Check availability of card1 on line 2 card 2
				if (CARD1_P2.getX() >= 685 && CARD1_P2.getX() <= 695 && CARD1_P2.getY() >= 565 && CARD1_P2.getY() <= 577)
				{
					if (CARD1_P2C == ZeroOrCard)
					{
						if ((L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne) || (L3_CARD3f.getTex() == ZeroOne || L3_CARD3f.getTex() == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneOrCard)
					{
						if ((L3_CARD2f.getTex() != ZeroOne && L3_CARD2f.getTex() != OneOne) || (L3_CARD3f.getTex() != ZeroOne && L3_CARD3f.getTex() != OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == ZeroAndCard)
					{
						if ((L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne) && (L3_CARD3f.getTex() == ZeroOne || L3_CARD3f.getTex() == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneAndCard)
					{
						if ((L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero) || (L3_CARD3f.getTex() == OneZero || L3_CARD3f.getTex() == ZeroZero))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == ZeroXorCard)
					{
						if (((L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne) && (L3_CARD3f.getTex() == ZeroOne || L3_CARD3f.getTex() == OneOne)) || ((L3_CARD3f.getTex() == OneZero || L3_CARD3f.getTex() == ZeroZero) && (L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero)))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (!((((L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne) && (L3_CARD3f.getTex() == ZeroOne || L3_CARD3f.getTex() == OneOne)) || ((L3_CARD3f.getTex() == OneZero || L3_CARD3f.getTex() == ZeroZero) && (L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero)))))
					{
						if (INITCARD1 == INITCARD2)
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
				}

				// Check availability of card1 on line 1 card 1
				if (CARD1_P2.getX() >= 615 && CARD1_P2.getX() <= 625 && CARD1_P2.getY() >= 615 && CARD1_P2.getY() <= 620)
				{
					if (CARD1_P2C == ZeroOrCard)
					{
						if ((L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne) || (L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L1_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneOrCard)
					{
						if ((L2_CARD1f.getTex() != ZeroOne && L2_CARD1f.getTex() != OneOne) || (L2_CARD1f.getTex() != ZeroOne && L2_CARD1f.getTex() != OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L1_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == ZeroAndCard)
					{
						if ((L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne) && (L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L1_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneAndCard)
					{
						if ((L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero) || (L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L1_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == ZeroXorCard)
					{
						if (((L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne) && (L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne)) || ((L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero) && (L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero)))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L1_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}
					}
					if (CARD1_P2C == OneXorCard)
					{
						if (!((((L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne) && (L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne)) || ((L2_CARD2f.getTex() == OneZero || L2_CARD2f.getTex() == ZeroZero) && (L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero)))))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L1_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L2_CARD2f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

					}
				}
				

				// Check availability of CARD1 on line 4 card 1
				if (CARD1_P1.getX() >= 425 && CARD1_P1.getX() <= 435 && CARD1_P1.getY() >= 415 && CARD1_P1.getY() <= 425)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) || (INITCARD2 == ZeroOne || INITCARD2 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
					if (CARD1_P1C == OneOrCard)
					{
						if ((INITCARD1 != ZeroOne && INITCARD1 != ZeroZero) && (INITCARD2 == ZeroOne && INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((INITCARD1 == OneZero || INITCARD1 == ZeroZero) || (INITCARD2 == OneZero || INITCARD2 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne)) || ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) && (INITCARD1 == OneZero || INITCARD1 == ZeroZero)))
						{
							CARD1_P1.setX(L1_CARD1.getX());
							CARD1_P1.setY(L1_CARD1.getY());

						}
						else
						{
							L4_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}

					if  (CARD1_P1C == OneXorCard)
					{
						if (((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne)) || ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) && (INITCARD1 == OneZero || INITCARD1 == ZeroZero)))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
				}

				// Check availability of CARD1 on line 4 card 2
				if (CARD1_P1.getX() >= 555 && CARD1_P1.getX() <= 665 && CARD1_P1.getY() >= 415 && CARD1_P1.getY() <= 425)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if ((INITCARD3 == ZeroOne || INITCARD3 == OneOne) || (INITCARD4 == ZeroOne || INITCARD4 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;

						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if (!(INITCARD2 == ZeroOne || INITCARD2 == ZeroZero) && !(INITCARD3 == ZeroOne && INITCARD3 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((INITCARD2 == ZeroOne || INITCARD2 == OneOne) && (INITCARD3 == ZeroOne || INITCARD3 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;


						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) || (INITCARD3 == OneZero && INITCARD3 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}


					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (INITCARD2 != INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (INITCARD2 == INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
				}

				// Check availability of CARD1 on &LINE 4 card 3
				if (CARD1_P1.getX() >= 685 && CARD1_P1.getX() <= 695 && CARD1_P1.getY() >= 415 && CARD1_P1.getY() <= 425)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if ((INITCARD2 == ZeroOne || INITCARD2 == OneOne) || (INITCARD3 == ZeroOne || INITCARD3 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD3.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD3.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if (!(INITCARD3 == ZeroOne || INITCARD3 == ZeroZero) && !(INITCARD4 == ZeroOne && INITCARD4 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD3.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD3.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD4 == OneZero || INITCARD4 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD3.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD3.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((INITCARD3 == OneZero || INITCARD3 == ZeroZero) || (INITCARD4 == OneZero || INITCARD4 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD3.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD3.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD3 == OneZero || INITCARD3 == ZeroZero) && (INITCARD4 == OneZero || INITCARD4 == ZeroZero)))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD3.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD3.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (!((((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD3 == ZeroOne || INITCARD3 == OneOne)) || ((INITCARD4 == OneZero || INITCARD4 == ZeroZero) && (INITCARD4 == OneZero || INITCARD4 == ZeroZero)))))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}	
						else
						{
							L4_CARD3.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD3.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
				}

				// Check availability of CARD1 on line 4 card 4
				if (CARD1_P1.getX() >= 795 && CARD1_P1.getX() <= 805 && CARD1_P1.getY() >= 415 && CARD1_P1.getY() <= 425)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if ((INITCARD4 == ZeroOne || INITCARD4 == OneOne) || (INITCARD5 == ZeroOne || INITCARD5 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD4.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD4.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if ((INITCARD4 != ZeroOne && INITCARD4 != OneOne) || (INITCARD5 != ZeroOne && INITCARD5 != OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD4.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD4.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD5 == ZeroOne || INITCARD5 == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD4.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD4.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((INITCARD4 == OneZero || INITCARD4 == ZeroZero) && (INITCARD5 == OneZero && INITCARD5 == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD4.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD4.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (((INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD5 == OneZero || INITCARD5 == ZeroZero) && (INITCARD5 == OneZero || INITCARD5 == ZeroZero)))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD4.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD4.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (!((((INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD5 == OneZero || INITCARD5 == ZeroZero) && (INITCARD5 == OneZero || INITCARD5 == ZeroZero)))))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L4_CARD4.setTexture(CARD1_P1.getTex());
							drawCount++;
							L4_CARD4.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
				}

				// Check availability of CARD1 on line 3 card 1
				if (CARD1_P1.getX() >= 485 && CARD1_P1.getX() <= 495 && CARD1_P1.getY() >= 480 && CARD1_P1.getY() <= 490)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if ((L4_CARD1.getTex() == ZeroOne || L4_CARD1.getTex() == OneOne) || (L4_CARD2.getTex() == ZeroOne || L4_CARD2.getTex() == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if ((L4_CARD1.getTex() != ZeroOne && L4_CARD1.getTex() != OneOne) || (L4_CARD2.getTex() != ZeroOne && L4_CARD2.getTex() != OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((L4_CARD1.getTex() == ZeroOne || L4_CARD1.getTex() == OneOne) && (L4_CARD2.getTex() == OneZero || L4_CARD2.getTex() == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((L4_CARD1.getTex() == OneZero || L4_CARD1.getTex() == ZeroZero) || (L4_CARD2.getTex() == OneZero || L4_CARD2.getTex() == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (((L4_CARD1.getTex() == ZeroOne || L4_CARD1.getTex() == OneOne) && ( L4_CARD2.getTex() == ZeroOne ||  L4_CARD2.getTex() == OneOne)) || 
							(( L4_CARD1.getTex() == OneZero || L4_CARD1.getTex() == ZeroZero) && (L4_CARD2.getTex() == OneZero || L4_CARD2.getTex() == ZeroZero)))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (!((((L4_CARD1.getTex() == ZeroOne || L4_CARD1.getTex() == OneOne) && (L4_CARD2.getTex() == ZeroOne || L4_CARD2.getTex() == OneOne)) || 
						((L4_CARD1.getTex() == OneZero || L4_CARD2.getTex() == ZeroZero) && (L4_CARD2.getTex() == OneZero || L4_CARD2.getTex() == ZeroZero)))))
					{
						if (INITCARD1 == INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
				}

				// Check availability of CARD1 on line 3 card 2
				if (CARD1_P1.getX() >= 615 && CARD1_P1.getX() <= 625 && CARD1_P1.getY() >= 480 && CARD1_P1.getY() <= 490)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if  ((L4_CARD2.getTex() == ZeroOne || L4_CARD2.getTex() == OneOne) || (L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD2.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if ((L4_CARD2.getTex() != ZeroOne && L4_CARD2.getTex() != OneOne) || (L4_CARD3.getTex() != ZeroOne && L4_CARD3.getTex() != OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD2.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((L4_CARD2.getTex() == ZeroOne || L4_CARD2.getTex() == OneOne) && (L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD2.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((L4_CARD2.getTex() == OneZero || L4_CARD2.getTex() == ZeroZero) || (L4_CARD3.getTex() == OneZero || L4_CARD3.getTex() == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD2.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (((L4_CARD2.getTex() == ZeroOne || L4_CARD2.getTex() == OneOne) && (L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne)) || ((L4_CARD2.getTex() == OneZero || L4_CARD2.getTex() == ZeroZero) && (L4_CARD3.getTex() == OneZero || L4_CARD3.getTex() == ZeroZero)))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD2.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (!((((L4_CARD2.getTex() == ZeroOne || L4_CARD2.getTex() == OneOne) && (L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne)) || ((L4_CARD2.getTex() == OneZero || L4_CARD2.getTex() == ZeroZero) && (L4_CARD3.getTex() == OneZero || L4_CARD3.getTex() == ZeroZero)))))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD2.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
				}

				// Check availability of CARD1 on line 3 card 3
				if (CARD1_P1.getX() >= 740 && CARD1_P1.getX() <= 750 && CARD1_P1.getY() >= 480 && CARD1_P1.getY() <= 490)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if ((L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne) && (L4_CARD4.getTex() == ZeroOne || L4_CARD4.getTex() == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if ((L4_CARD3.getTex() == OneZero || L4_CARD3.getTex() == ZeroZero) || (L4_CARD4.getTex() == OneZero || L4_CARD4.getTex() == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD3.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD3.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne) && (L4_CARD4.getTex() == ZeroOne || L4_CARD4.getTex() == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD3.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD3.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((L4_CARD3.getTex() == OneZero || L4_CARD3.getTex() == ZeroZero) || (L4_CARD4.getTex() == OneZero || L4_CARD4.getTex() == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD3.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD3.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (((L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne) && (L4_CARD4.getTex() == ZeroOne || L4_CARD4.getTex() == OneOne)) || ((L4_CARD3.getTex() == OneZero || L4_CARD3.getTex() == ZeroZero) && (L4_CARD4.getTex() == OneZero || L4_CARD4.getTex() == ZeroZero)))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD3.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD3.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (!((((L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne) && (L4_CARD4.getTex() == ZeroOne || L4_CARD4.getTex() == OneOne)) || ((L4_CARD3.getTex() == OneZero || L4_CARD3.getTex() == ZeroZero) && (L4_CARD4.getTex() == OneZero || L4_CARD4.getTex() == ZeroZero)))))
					{
						if (INITCARD1 == INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L3_CARD3.setTexture(CARD1_P1.getTex());
							drawCount++;
							L3_CARD3.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
				}

				// Check availability of CARD1 on line 2 card 1
				if (CARD1_P1.getX() >= 555 && CARD1_P1.getX() <= 560 && CARD1_P1.getY() >= 565 && CARD1_P1.getY() <= 575)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if ((L3_CARD1.getTex() == ZeroOne || L3_CARD1.getTex() == OneOne) || (L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L2_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if ((L3_CARD1.getTex() != ZeroOne && L3_CARD1.getTex() != OneOne) || (L3_CARD2.getTex() != ZeroOne && L3_CARD2.getTex() != OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L2_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((L3_CARD1.getTex() == ZeroOne || L3_CARD1.getTex() == OneOne) && (L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L2_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((L3_CARD1.getTex() == OneZero || L3_CARD1.getTex() == ZeroZero) || (L3_CARD2.getTex() == OneZero || L3_CARD2.getTex() == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L2_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (((L3_CARD1.getTex() == ZeroOne || L3_CARD1.getTex() == OneOne) && (L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne)) || ((L3_CARD2.getTex() == OneZero || L3_CARD2.getTex() == ZeroZero) && (L3_CARD1.getTex() == OneZero || L3_CARD1.getTex() == ZeroZero)))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L2_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (!((((L3_CARD1.getTex() == ZeroOne || L3_CARD1.getTex() == OneOne) && (L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne)) || ((L3_CARD2.getTex() == OneZero || L3_CARD2.getTex() == ZeroZero) && (L3_CARD1.getTex() == OneZero || L3_CARD1.getTex() == ZeroZero)))))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L2_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD1.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}

					}
				}

				// Check availability of CARD1 on line 2 card 2
				if (CARD1_P1.getX() >= 685 && CARD1_P1.getX() <= 695 && CARD1_P1.getY() >= 565 && CARD1_P1.getY() <= 575)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if ((L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne) || (L3_CARD3.getTex() == ZeroOne || L3_CARD3.getTex() == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L2_CARD2.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if ((L3_CARD2.getTex() != ZeroOne && L3_CARD2.getTex() != OneOne) || (L3_CARD3.getTex() != ZeroOne && L3_CARD3.getTex() != OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L2_CARD2.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne) && (L3_CARD3.getTex() == ZeroOne || L3_CARD3.getTex() == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L2_CARD2.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((L3_CARD2.getTex() == OneZero || L3_CARD2.getTex() == ZeroZero) || (L3_CARD3.getTex() == OneZero || L3_CARD3.getTex() == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L2_CARD2.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (((L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne) && (L3_CARD3.getTex() == ZeroOne || L3_CARD3.getTex() == OneOne)) || ((L3_CARD3.getTex() == OneZero || L3_CARD3.getTex() == ZeroZero) && (L3_CARD2.getTex() == OneZero || L3_CARD2.getTex() == ZeroZero)))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L2_CARD2.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (!((((L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne) && (L3_CARD3.getTex() == ZeroOne || L3_CARD3.getTex() == OneOne)) || ((L3_CARD3.getTex() == OneZero || L3_CARD3.getTex() == ZeroZero) && (L3_CARD2.getTex() == OneZero || L3_CARD2.getTex() == ZeroZero)))))
					{
						if (INITCARD1 == INITCARD2)
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L2_CARD2.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
				}

				// Check availability of CARD1 on line 1 card 1
				if (CARD1_P1.getX() >= 615 && CARD1_P1.getX() <= 625 && CARD1_P1.getY() >= 615 && CARD1_P1.getY() <= 620)
				{
					if (CARD1_P1C == ZeroOrCard)
					{
						if ((L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne) || (L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L1_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneOrCard)
					{
						if ((L2_CARD1.getTex() != ZeroOne && L2_CARD1.getTex() != OneOne) || (L2_CARD1.getTex() != ZeroOne && L2_CARD1.getTex() != OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L1_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == ZeroAndCard)
					{
						if ((L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne) && (L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L1_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneAndCard)
					{
						if ((L2_CARD1.getTex() == OneZero || L2_CARD1.getTex() == ZeroZero) || (L2_CARD1.getTex() == OneZero || L2_CARD1.getTex() == ZeroZero))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L1_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == ZeroXorCard)
					{
						if (((L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne) && (L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne)) || ((L2_CARD1.getTex() == OneZero || L2_CARD1.getTex() == ZeroZero) && (L2_CARD1.getTex() == OneZero || L2_CARD1.getTex() == ZeroZero)))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							L1_CARD1.setTexture(CARD1_P1.getTex());
							drawCount++;
							L2_CARD2.setTexture(CARD1_P1.getTex());
							CARD1_P1.setTexture(deck[drawCount].texture);
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
							isP1Turn = false;
						}
					}
					if (CARD1_P1C == OneXorCard)
					{
						if (!((((L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne) && (L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne)) || ((L2_CARD2.getTex() == OneZero || L2_CARD2.getTex() == ZeroZero) && (L2_CARD1.getTex() == OneZero || L2_CARD1.getTex() == ZeroZero)))))
						{
							CARD1_P1.setX(75);
							CARD1_P1.setY(450);
						}
						else
						{
							// Check availability of card1 on line 4 card 1
							if (CARD1_P2.getX() >= 420 && CARD1_P2.getX() <= 440 && CARD1_P2.getY() >= 190 && CARD1_P2.getY() <= 210)
							{
								if (CARD1_P2C == ZeroOrCard)
									if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) || (INITCARD2 == ZeroOne || INITCARD2 == OneOne))
									{
										CARD1_P2.setX(77);
										CARD1_P2.setY(40);
									}
									else
									{
										L4_CARD1f.setTexture(CARD1_P2.getTex());
										drawCount++;
										L4_CARD1f.setTexture(CARD1_P2.getTex());
										CARD1_P2.setTexture(deck[drawCount].texture);
										CARD1_P2.setX(77);
										CARD1_P2.setY(40);
										isP1Turn = true;
									}

							}
							if (CARD1_P2C == OneOrCard)
							{
								if ((INITCARD1 != ZeroOne && INITCARD1 != ZeroZero) && (INITCARD2 == ZeroOne && INITCARD2 == ZeroZero))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
							if (CARD1_P2C == ZeroAndCard)
							{
								if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneAndCard)
							{
								if ((INITCARD1 == OneZero || INITCARD1 == ZeroZero) || (INITCARD2 == OneZero || INITCARD2 == ZeroZero))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
							if (CARD1_P2C == ZeroXorCard)
							{
								if (((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne)) || ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) && (INITCARD1 == OneZero || INITCARD1 == ZeroZero)))
								{
									CARD1_P2.setX(L1_CARD1f.getX());
									CARD1_P2.setY(L1_CARD1f.getY());

								}
								else
								{
									L4_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}

							if (CARD1_P2C == OneXorCard)
							{
								if (((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne)) || ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) && (INITCARD1 == OneZero || INITCARD1 == ZeroZero)))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
						}

						// Check availability of card1 on line 4 card 2
						if (CARD1_P2.getX() >= 550 && CARD1_P2.getX() <= 570 && CARD1_P2.getY() >= 190 && CARD1_P2.getY() <= 20)
						{
							if (CARD1_P2C == ZeroOrCard)
							{
								if ((INITCARD3 == ZeroOne || INITCARD3 == OneOne) || (INITCARD4 == ZeroOne || INITCARD4 == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;

								}
							}
							if (CARD1_P2C == OneOrCard)
							{
								if (!(INITCARD2 == ZeroOne || INITCARD2 == ZeroZero) && !(INITCARD3 == ZeroOne && INITCARD3 == ZeroZero))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
							if (CARD1_P2C == ZeroAndCard)
							{
								if ((INITCARD2 == ZeroOne || INITCARD2 == OneOne) && (INITCARD3 == ZeroOne || INITCARD3 == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;


								}
							}
							if (CARD1_P2C == OneAndCard)
							{
								if ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) || (INITCARD3 == OneZero && INITCARD3 == ZeroZero))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}


							}
							if (CARD1_P2C == ZeroXorCard)
							{
								if (INITCARD2 != INITCARD2)
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneXorCard)
							{
								if (INITCARD2 == INITCARD2)
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
						}

						// Check availability of card1 on &line 4 card 3
						if (CARD1_P2.getX() >= 675 && CARD1_P2.getX() <= 695 && CARD1_P2.getY() >= 190 && CARD1_P2.getY() <= 210)
						{
							if (CARD1_P2C == ZeroOrCard)
							{
								if ((INITCARD2 == ZeroOne || INITCARD2 == OneOne) || (INITCARD3 == ZeroOne || INITCARD3 == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD3f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD3f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneOrCard)
							{
								if (!(INITCARD3 == ZeroOne || INITCARD3 == ZeroZero) && !(INITCARD4 == ZeroOne && INITCARD4 == ZeroZero))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD3f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD3f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
							if (CARD1_P2C == ZeroAndCard)
							{
								if ((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD4 == OneZero || INITCARD4 == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD3f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD3f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneAndCard)
							{
								if ((INITCARD3 == OneZero || INITCARD3 == ZeroZero) || (INITCARD4 == OneZero || INITCARD4 == ZeroZero))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD3f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD3f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
							if (CARD1_P2C == ZeroXorCard)
							{
								if (((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD3 == OneZero || INITCARD3 == ZeroZero) && (INITCARD4 == OneZero || INITCARD4 == ZeroZero)))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD3f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD3f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneXorCard)
							{
								if (!((((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD3 == ZeroOne || INITCARD3 == OneOne)) || ((INITCARD4 == OneZero || INITCARD4 == ZeroZero) && (INITCARD4 == OneZero || INITCARD4 == ZeroZero)))))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}//h
								else
								{
									L4_CARD3f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD3f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
						}

						// Check availability of card1 on line 4 card 4
						if (CARD1_P2.getX() >= 790 && CARD1_P2.getX() <= 810 && CARD1_P2.getY() >= 190 && CARD1_P2.getY() <= 210)
						{
							if (CARD1_P2C == ZeroOrCard)
							{
								if ((INITCARD4 == ZeroOne || INITCARD4 == OneOne) || (INITCARD5 == ZeroOne || INITCARD5 == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD4f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD4f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneOrCard)
							{
								if ((INITCARD4 != ZeroOne && INITCARD4 != OneOne) || (INITCARD5 != ZeroOne && INITCARD5 != OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD4f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD4f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
							if (CARD1_P2C == ZeroAndCard)
							{
								if ((INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD5 == ZeroOne || INITCARD5 == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD4f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD4f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneAndCard)
							{
								if ((INITCARD4 == OneZero || INITCARD4 == ZeroZero) && (INITCARD5 == OneZero && INITCARD5 == ZeroZero))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD4f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD4f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
							if (CARD1_P2C == ZeroXorCard)
							{
								if (((
									INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD5 == OneZero || INITCARD5 == ZeroZero) && (INITCARD5 == OneZero || INITCARD5 == ZeroZero)))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD4f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD4f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneXorCard)
							{
								if (!((((INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD5 == OneZero || INITCARD5 == ZeroZero) && (INITCARD5 == OneZero || INITCARD5 == ZeroZero)))))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L4_CARD4f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L4_CARD4f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
						}

						// Check availability of card1 on line 3 card 1
						if (CARD1_P2.getX() >= 485 && CARD1_P2.getX() <= 495 && CARD1_P2.getY() >= 480 && CARD1_P2.getY() <= 490)
						{
							if (CARD1_P2C == ZeroOrCard)
							{
								if ((L4_CARD1f.getTex() == ZeroOne || L4_CARD1f.getTex() == OneOne) || (L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneOrCard)
							{
								if ((L4_CARD1f.getTex() != ZeroOne && L4_CARD1f.getTex() != OneOne) || (L4_CARD2f.getTex() != ZeroOne && L4_CARD2f.getTex() != OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == ZeroAndCard)
							{
								if ((L4_CARD1f.getTex() == ZeroOne || L4_CARD1f.getTex() == OneOne) && (L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneAndCard)
							{
								if ((L4_CARD1f.getTex() == OneZero || L4_CARD1f.getTex() == ZeroZero) || (L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
							if (CARD1_P2C == ZeroXorCard)
							{
								if (((L4_CARD1f.getTex() == ZeroOne || L4_CARD1f.getTex() == OneOne) && (L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne)) || ((L4_CARD1f.getTex() == OneZero || L4_CARD1f.getTex() == ZeroZero) && (L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero)))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (!((((L4_CARD1f.getTex() == ZeroOne || L4_CARD1f.getTex() == OneOne) && (L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne)) || ((L4_CARD1f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero) && (L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero)))))
							{
								if (INITCARD1 == INITCARD2)
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
						}

						// Check availability of card1 on line 3 card 2
						if (CARD1_P2.getX() >= 615 && CARD1_P2.getX() <= 625 && CARD1_P2.getY() >= 480 && CARD1_P2.getY() <= 490)
						{
							if (CARD1_P2C == ZeroOrCard)
							{
								if ((L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne) || (L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneOrCard)
							{
								if ((L4_CARD2f.getTex() != ZeroOne && L4_CARD2f.getTex() != OneOne) || (L4_CARD3f.getTex() != ZeroOne && L4_CARD3f.getTex() != OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == ZeroAndCard)
							{
								if ((L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne) && (L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneAndCard)
							{
								if ((L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero) || (L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
							if (CARD1_P2C == ZeroXorCard)
							{
								if (((L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne) && (L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne)) || ((L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero) && (L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero)))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneXorCard)
							{
								if (!((((L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne) && (L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne)) || ((L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero) && (L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero)))))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
						}

						// Check availability of card1 on line 3 card 3
						if (CARD1_P2.getX() >= 740 && CARD1_P2.getX() <= 770 && CARD1_P2.getY() >= 480 && CARD1_P2.getY() <= 490)
						{
							if (CARD1_P2C == ZeroOrCard)
							{
								if ((L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne) && (L4_CARD4f.getTex() == ZeroOne || L4_CARD4f.getTex() == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
							}
							if (CARD1_P2C == OneOrCard)
							{
								if ((L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero) || (L4_CARD4f.getTex() == OneZero || L4_CARD4f.getTex() == ZeroZero))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD3f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD3f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == ZeroAndCard)
							{
								if ((L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne) && (L4_CARD4f.getTex() == ZeroOne || L4_CARD4f.getTex() == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD3f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD3f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneAndCard)
							{
								if ((L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero) || (L4_CARD4f.getTex() == OneZero || L4_CARD4f.getTex() == ZeroZero))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD3f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD3f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
							if (CARD1_P2C == ZeroXorCard)
							{
								if (((L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne) && (L4_CARD4f.getTex() == ZeroOne || L4_CARD4f.getTex() == OneOne)) || ((L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero) && (L4_CARD4f.getTex() == OneZero || L4_CARD4f.getTex() == ZeroZero)))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD3f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD3f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (!((((L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne) && (L4_CARD4f.getTex() == ZeroOne || L4_CARD4f.getTex() == OneOne)) || ((L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero) && (L4_CARD4f.getTex() == OneZero || L4_CARD4f.getTex() == ZeroZero)))))
							{
								if (INITCARD1 == INITCARD2)
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L3_CARD3f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L3_CARD3f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
						}

						// Check availability of card1 on line 2 card 1
						if (CARD1_P2.getX() >= 555 && CARD1_P2.getX() <= 560 && CARD1_P2.getY() >= 565 && CARD1_P2.getY() <= 577)
						{
							if (CARD1_P2C == ZeroOrCard)
							{
								if ((L3_CARD1f.getTex() == ZeroOne || L3_CARD1f.getTex() == OneOne) || (L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L2_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneOrCard)
							{
								if ((L3_CARD1f.getTex() != ZeroOne && L3_CARD1f.getTex() != OneOne) || (L3_CARD2f.getTex() != ZeroOne && L3_CARD2f.getTex() != OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L2_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
							if (CARD1_P2C == ZeroAndCard)
							{
								if ((L3_CARD1f.getTex() == ZeroOne || L3_CARD1f.getTex() == OneOne) && (L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L2_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneAndCard)
							{
								if ((L3_CARD1f.getTex() == OneZero || L3_CARD1f.getTex() == ZeroZero) || (L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L2_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == ZeroXorCard)
							{
								if (((L3_CARD1f.getTex() == ZeroOne || L3_CARD1f.getTex() == OneOne) && (L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne)) || ((L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero) && (L3_CARD1f.getTex() == OneZero || L3_CARD1f.getTex() == ZeroZero)))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L2_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneXorCard)
							{
								if (!((((L3_CARD1f.getTex() == ZeroOne || L3_CARD1f.getTex() == OneOne) && (L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne)) || ((L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero) && (L3_CARD1f.getTex() == OneZero || L3_CARD1f.getTex() == ZeroZero)))))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L2_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD1f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
						}

						// Check availability of card1 on line 2 card 2
						if (CARD1_P2.getX() >= 685 && CARD1_P2.getX() <= 695 && CARD1_P2.getY() >= 565 && CARD1_P2.getY() <= 577)
						{
							if (CARD1_P2C == ZeroOrCard)
							{
								if ((L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne) || (L3_CARD3f.getTex() == ZeroOne || L3_CARD3f.getTex() == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneOrCard)
							{
								if ((L3_CARD2f.getTex() != ZeroOne && L3_CARD2f.getTex() != OneOne) || (L3_CARD3f.getTex() != ZeroOne && L3_CARD3f.getTex() != OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == ZeroAndCard)
							{
								if ((L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne) && (L3_CARD3f.getTex() == ZeroOne || L3_CARD3f.getTex() == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneAndCard)
							{
								if ((L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero) || (L3_CARD3f.getTex() == OneZero || L3_CARD3f.getTex() == ZeroZero))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == ZeroXorCard)
							{
								if (((L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne) && (L3_CARD3f.getTex() == ZeroOne || L3_CARD3f.getTex() == OneOne)) || ((L3_CARD3f.getTex() == OneZero || L3_CARD3f.getTex() == ZeroZero) && (L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero)))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (!((((L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne) && (L3_CARD3f.getTex() == ZeroOne || L3_CARD3f.getTex() == OneOne)) || ((L3_CARD3f.getTex() == OneZero || L3_CARD3f.getTex() == ZeroZero) && (L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero)))))
							{
								if (INITCARD1 == INITCARD2)
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
						}

						// Check availability of card1 on line 1 card 1
						if (CARD1_P2.getX() >= 615 && CARD1_P2.getX() <= 625 && CARD1_P2.getY() >= 615 && CARD1_P2.getY() <= 620)
						{
							if (CARD1_P2C == ZeroOrCard)
							{
								if ((L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne) || (L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L1_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneOrCard)
							{
								if ((L2_CARD1f.getTex() != ZeroOne && L2_CARD1f.getTex() != OneOne) || (L2_CARD1f.getTex() != ZeroOne && L2_CARD1f.getTex() != OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L1_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == ZeroAndCard)
							{
								if ((L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne) && (L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L1_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneAndCard)
							{
								if ((L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero) || (L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L1_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == ZeroXorCard)
							{
								if (((L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne) && (L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne)) || ((L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero) && (L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero)))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L1_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}
							}
							if (CARD1_P2C == OneXorCard)
							{
								if (!((((L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne) && (L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne)) || ((L2_CARD2f.getTex() == OneZero || L2_CARD2f.getTex() == ZeroZero) && (L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero)))))
								{
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
								}
								else
								{
									L1_CARD1f.setTexture(CARD1_P2.getTex());
									drawCount++;
									L2_CARD2f.setTexture(CARD1_P2.getTex());
									CARD1_P2.setTexture(deck[drawCount].texture);
									CARD1_P2.setX(77);
									CARD1_P2.setY(40);
									isP1Turn = true;
								}

							}
						
						}

					}
				}

				ENTITY DRAWDECK(125, 303, DeckAsset);
				window.render(DRAWDECK);

				window.render(INIT1);
				window.render(INIT2);
				window.render(INIT3);
				window.render(INIT4);
				window.render(INIT5);

				window.render(L4_CARD1);
				window.render(L4_CARD2);
				window.render(L4_CARD3);
				window.render(L4_CARD4);

				window.render(L3_CARD1);
				window.render(L3_CARD2);
				window.render(L3_CARD3);

				window.render(L2_CARD1);
				window.render(L2_CARD2);

				window.render(L1_CARD1);

				window.render(L4_CARD1f);
				window.render(L4_CARD2f);
				window.render(L4_CARD3f);
				window.render(L4_CARD4f);

				window.render(L3_CARD1f);
				window.render(L3_CARD2f);
				window.render(L3_CARD3f);

				window.render(L2_CARD1f);
				window.render(L2_CARD2f);

				window.render(L1_CARD1f);

				window.render(CARD1_P1);
				window.render(CARD2_P1);
				window.render(CARD3_P1);
				window.render(CARD4_P1);

				window.render(CARD1_P2);
				window.render(CARD2_P2);
				window.render(CARD3_P2);
				window.render(CARD4_P2);

				
				window.render(RuleCard);
				
				SDL_RenderPresent(window.getRenderer());
			}
			else if (buttonPressed == 4)
			{
				window.render(comingSoonScreen);
			
			}
			else if (buttonPressed == 3)

			{
			SDL_Texture* INITCARD1 = NULL; // declares initial card textures
			SDL_Texture* INITCARD2 = NULL;
			SDL_Texture* INITCARD3 = NULL;
			SDL_Texture* INITCARD4 = NULL;
			SDL_Texture* INITCARD5 = NULL;

			SDL_Texture* RULECARD = window.loadTexture("../assets/RuleCard.png");

			SDL_Texture* LINE4CARD1 = PyrCardD;
			SDL_Texture* LINE4CARD2 = PyrCardD;
			SDL_Texture* LINE4CARD3 = PyrCardD;
			SDL_Texture* LINE4CARD4 = PyrCardD;

			SDL_Texture* LINE3CARD1 = PyrCardL;
			SDL_Texture* LINE3CARD2 = PyrCardL;
			SDL_Texture* LINE3CARD3 = PyrCardL;

			SDL_Texture* LINE2CARD1 = PyrCardD;
			SDL_Texture* LINE2CARD2 = PyrCardD;

			SDL_Texture* LINE1CARD1 = PyrCardL;

			SDL_Texture* LINE4CARD1f = PyrCardD_flip;
			SDL_Texture* LINE4CARD2f = PyrCardD_flip;
			SDL_Texture* LINE4CARD3f = PyrCardD_flip;
			SDL_Texture* LINE4CARD4f = PyrCardD_flip;

			SDL_Texture* LINE3CARD1f = PyrCardL_flip;
			SDL_Texture* LINE3CARD2f = PyrCardL_flip;
			SDL_Texture* LINE3CARD3f = PyrCardL_flip;

			SDL_Texture* LINE2CARD1f = PyrCardD_flip;
			SDL_Texture* LINE2CARD2f = PyrCardD_flip;

			SDL_Texture* LINE1CARD1f = PyrCardL_flip;

			SDL_Texture* CARD1_P1C = NULL; // declares cards in hand textures
			SDL_Texture* CARD2_P1C = NULL;
			SDL_Texture* CARD3_P1C = NULL;
			SDL_Texture* CARD4_P1C = NULL;

			SDL_Texture* CARD1_P2C = NULL;
			SDL_Texture* CARD2_P2C = NULL;
			SDL_Texture* CARD3_P2C = NULL;
			SDL_Texture* CARD4_P2C = NULL;


			for (int i = 0; i < 5; i++) // gives random value to deck cards
			{
				if (i == 0)
				{
					if (initialCards[i] == "ZeroOne" || initialCards[i] == "ZeroOne2")
					{
						INITCARD1 = ZeroOne;

					}
					else if (initialCards[i] == "ZeroZero")
					{
						INITCARD1 = ZeroZero;
					}
					else if (initialCards[i] == "ZeroZero")
					{
						INITCARD1 = ZeroZero;
					}
					else if (initialCards[i] == "OneOne")
					{
						INITCARD1 = OneOne;
					}
					else if (initialCards[i] == "OneZero" || initialCards[i] == "OneZero2")
					{
						INITCARD1 = OneZero;
					}
				}

				if (i == 1)
				{
					if (initialCards[i] == "ZeroOne" || initialCards[i] == "ZeroOne2")
					{
						INITCARD2 = ZeroOne;
					}
					else if (initialCards[i] == "ZeroZero")
					{
						INITCARD2 = ZeroZero;
					}
					else if (initialCards[i] == "ZeroZero")
					{
						INITCARD2 = ZeroZero;
					}
					else if (initialCards[i] == "OneOne")
					{
						INITCARD2 = OneOne;
					}
					else if (initialCards[i] == "OneZero" || initialCards[i] == "OneZero2")
					{
						INITCARD2 = OneZero;
					}
				}

				if (i == 2)
				{
					if (initialCards[i] == "ZeroOne" || initialCards[i] == "ZeroOne2")
					{
						INITCARD3 = ZeroOne;
					}
					else if (initialCards[i] == "ZeroZero")
					{
						INITCARD3 = ZeroZero;
					}
					else if (initialCards[i] == "ZeroZero")
					{
						INITCARD3 = ZeroZero;
					}
					else if (initialCards[i] == "OneOne")
					{
						INITCARD3 = OneOne;
					}
					else if (initialCards[i] == "OneZero" || initialCards[i] == "OneZero2")
					{
						INITCARD3 = OneZero;
					}
				}

				if (i == 3)
				{
					if (initialCards[i] == "ZeroOne" || initialCards[i] == "ZeroOne2")
					{
						INITCARD4 = ZeroOne;
					}
					else if (initialCards[i] == "ZeroZero")
					{
						INITCARD4 = ZeroZero;
					}
					else if (initialCards[i] == "ZeroZero")
					{
						INITCARD4 = ZeroZero;
					}
					else if (initialCards[i] == "OneOne")
					{
						INITCARD4 = OneOne;
					}
					else if (initialCards[i] == "OneZero" || initialCards[i] == "OneZero2")
					{
						INITCARD4 = OneZero;
					}
				}

				if (i == 4)
				{
					if (initialCards[i] == "ZeroOne" || initialCards[i] == "ZeroOne2")
					{
						INITCARD5 = ZeroOne;
					}
					else if (initialCards[i] == "ZeroZero")
					{
						INITCARD5 = ZeroZero;
					}
					else if (initialCards[i] == "ZeroZero")
					{
						INITCARD5 = ZeroZero;
					}
					else if (initialCards[i] == "OneOne")
					{
						INITCARD5 = OneOne;
					}
					else if (initialCards[i] == "OneZero" || initialCards[i] == "OneZero2")
					{
						INITCARD5 = OneZero;
					}
				}

			}


			for (int i = 0; i < 8; i++)
			{
				if (i == 0)
				{
					if (deck[i].name == "OR0")
					{
						CARD1_P1C = ZeroOrCard;
						deck[i].texture = ZeroOrCard;
					}
					else if (deck[i].name == "OR1")
					{
						CARD1_P1C = OneOrCard;
					}
					else if (deck[i].name == "AND0")
					{
						CARD1_P1C = ZeroAndCard;

					}
					else if (deck[i].name == "AND1")
					{
						CARD1_P1C = OneAndCard;

					}
					else if (deck[i].name == "XOR0")
					{
						CARD1_P1C = ZeroXorCard;

					}
					else if (deck[i].name == "XOR1")
					{
						CARD1_P1C = OneOrCard;

					}



				}
				else if (i == 1)
				{
					if (deck[i].name == "OR0")
					{
						CARD2_P1C = ZeroOrCard;
					}
					else if (deck[i].name == "OR1")
					{
						CARD2_P1C = OneOrCard;
					}
					else if (deck[i].name == "AND0")
					{
						CARD2_P1C = ZeroAndCard;
					}
					else if (deck[i].name == "AND1")
					{
						CARD2_P1C = OneAndCard;
					}
					else if (deck[i].name == "XOR0")
					{
						CARD2_P1C = ZeroXorCard;
					}
					else if (deck[i].name == "XOR1")
					{
						CARD2_P1C = OneOrCard;
					}
				}

				else if (i == 2)
				{
					if (deck[i].name == "OR0")
					{
						CARD3_P1C = ZeroOrCard;
					}
					else if (deck[i].name == "OR1")
					{
						CARD3_P1C = OneOrCard;
					}
					else if (deck[i].name == "AND0")
					{
						CARD3_P1C = ZeroAndCard;
					}
					else if (deck[i].name == "AND1")
					{
						CARD3_P1C = OneAndCard;
					}
					else if (deck[i].name == "XOR0")
					{
						CARD3_P1C = ZeroXorCard;
					}
					else if (deck[i].name == "XOR1")
					{
						CARD3_P1C = OneOrCard;
					}
				}
				else if (i == 3)
				{
					if (deck[i].name == "OR0")
					{
						CARD4_P1C = ZeroOrCard;
					}
					else if (deck[i].name == "OR1")
					{
						CARD4_P1C = OneOrCard;
					}
					else if (deck[i].name == "AND0")
					{
						CARD4_P1C = ZeroAndCard;
					}
					else if (deck[i].name == "AND1")
					{
						CARD4_P1C = OneAndCard;
					}
					else if (deck[i].name == "XOR0")
					{
						CARD4_P1C = ZeroXorCard;
					}
					else if (deck[i].name == "XOR1")
					{
						CARD4_P1C = OneOrCard;
					}

				}
				for (int i = 4; i < 8; i++)
				{
					if (i == 4)
					{
						if (deck[i].name == "OR0")
						{
							CARD1_P2C = ZeroOrCard;
						}
						else if (deck[i].name == "OR1")
						{
							CARD1_P2C = OneOrCard;
						}
						else if (deck[i].name == "AND0")
						{
							CARD1_P2C = ZeroAndCard;
						}
						else if (deck[i].name == "AND1")
						{
							CARD1_P2C = OneAndCard;
						}
						else if (deck[i].name == "XOR0")
						{
							CARD1_P2C = ZeroXorCard;
						}
						else if (deck[i].name == "XOR1")
						{
							CARD1_P2C = OneOrCard;
						}
					}

					else if (i == 5)
					{
						if (deck[i].name == "OR0")
						{
							CARD2_P2C = ZeroOrCard;
						}
						else if (deck[i].name == "OR1")
						{
							CARD2_P2C = OneOrCard;
						}
						else if (deck[i].name == "AND0")
						{
							CARD2_P2C = ZeroAndCard;
						}
						else if (deck[i].name == "AND1")
						{
							CARD2_P2C = OneAndCard;
						}
						else if (deck[i].name == "XOR0")
						{
							CARD2_P2C = ZeroXorCard;
						}
						else if (deck[i].name == "XOR1")
						{
							CARD2_P2C = OneOrCard;
						}
					}

					else if (i == 6)
					{
						if (deck[i].name == "OR0")
						{
							CARD3_P2C = ZeroOrCard;
						}
						else if (deck[i].name == "OR1")
						{
							CARD3_P2C = OneOrCard;
						}
						else if (deck[i].name == "AND0")
						{
							CARD3_P2C = ZeroAndCard;
						}
						else if (deck[i].name == "AND1")
						{
							CARD3_P2C = OneAndCard;
						}
						else if (deck[i].name == "XOR0")
						{
							CARD3_P2C = ZeroXorCard;
						}
						else if (deck[i].name == "XOR1")
						{
							CARD3_P2C = OneOrCard;
						}
					}

					else if (i == 7)
					{
						if (deck[i].name == "OR0")
						{
							CARD4_P2C = ZeroOrCard;
						}
						else if (deck[i].name == "OR1")
						{
							CARD4_P2C = OneOrCard;
						}
						else if (deck[i].name == "AND0")
						{
							CARD4_P2C = ZeroAndCard;
						}
						else if (deck[i].name == "AND1")
						{
							CARD4_P2C = OneAndCard;
						}
						else if (deck[i].name == "XOR0")
						{
							CARD4_P2C = ZeroXorCard;
						}
						else if (deck[i].name == "XOR1")
						{
							CARD4_P2C = OneOrCard;
						}
					}
				}
			}

			ENTITY INIT1(360, 305, INITCARD1); // declares initial cards as entities 
			ENTITY INIT2(487, 305, INITCARD2);
			ENTITY INIT3(614, 305, INITCARD3);
			ENTITY INIT4(745, 305, INITCARD4);
			ENTITY INIT5(873, 305, INITCARD5);

			ENTITY RuleCard(1050, 220, RULECARD);

			static ENTITY L4_CARD4(800, 420, LINE4CARD4);
			static ENTITY L4_CARD3(685, 420, LINE4CARD3);
			static ENTITY L4_CARD2(560, 420, LINE4CARD2);
			static ENTITY L4_CARD1(430, 420, LINE4CARD1);
			static ENTITY L3_CARD3(746, 485, LINE3CARD3);
			static ENTITY L3_CARD2(620, 485, LINE3CARD2);
			static ENTITY L3_CARD1(490, 485, LINE3CARD1);
			static ENTITY L2_CARD2(690, 570, LINE2CARD2);
			static ENTITY L2_CARD1(560, 570, LINE2CARD1);
			static ENTITY L1_CARD1(620, 615, LINE1CARD1);

			static ENTITY L4_CARD4f(800, 200, LINE4CARD4f);
			static ENTITY L4_CARD3f(685, 200, LINE4CARD3f);
			static ENTITY L4_CARD2f(560, 200, LINE4CARD2f);
			static ENTITY L4_CARD1f(430, 200, LINE4CARD1f);
			static ENTITY L3_CARD3f(745, 130, LINE3CARD3f);
			static ENTITY L3_CARD2f(620, 130, LINE3CARD2f);
			static ENTITY L3_CARD1f(490, 130, LINE3CARD1f);
			static ENTITY L2_CARD2f(685, 44, LINE2CARD2f);
			static ENTITY L2_CARD1f(555, 44, LINE2CARD1f);
			static ENTITY L1_CARD1f(620, 1, LINE1CARD1f);

			static ENTITY CARD1_P1(75, 450, CARD1_P1C); // declares player cards in hand as entities
			static ENTITY CARD2_P1(75, 570, CARD2_P1C);
			static ENTITY CARD3_P1(160, 450, CARD3_P1C);
			static ENTITY CARD4_P1(160, 570, CARD4_P1C);

			static ENTITY CARD1_P2(77, 40, CARD1_P2C);
			static ENTITY CARD2_P2(77, 160, CARD2_P2C);
			static ENTITY CARD3_P2(160, 40, CARD3_P2C);
			static ENTITY CARD4_P2(160, 160, CARD4_P2C);

			if (isP1Turn) // makes cards draggable depending on which player's turn it is
			{
				playerBlockRect.setY(0);
				if (CARD1_P1.isMouseClicked()) // if mouse button is clicked/held
				{
					CARD1_P1.setDraggable(!CARD1_P1.getDrag()); // make card draggable
					CARD1_P1.drag(); // drag card
				}

				if (CARD2_P1.isMouseClicked())
				{
					CARD2_P1.setDraggable(!CARD1_P1.getDrag());
					CARD2_P1.drag();
				}

				if (CARD3_P1.isMouseClicked())
				{
					CARD3_P1.setDraggable(!CARD1_P1.getDrag());
					CARD3_P1.drag();
				}

				if (CARD4_P1.isMouseClicked())
				{
					CARD4_P1.setDraggable(!CARD1_P1.getDrag());
					CARD4_P1.drag();
				}
			}
			else
			{
				playerBlockRect.setY(360);

				if (CARD1_P2.isMouseClicked())
				{
					CARD1_P2.setDraggable(!CARD1_P2.getDrag());
					CARD1_P2.drag();
				}

				if (CARD2_P2.isMouseClicked())
				{
					CARD2_P2.setDraggable(!CARD2_P2.getDrag());
					CARD2_P2.drag();
				}

				if (CARD3_P2.isMouseClicked())
				{
					CARD3_P2.setDraggable(!CARD3_P2.getDrag());
					CARD3_P2.drag();
				}

				if (CARD4_P2.isMouseClicked())
				{
					CARD4_P2.setDraggable(!CARD4_P2.getDrag());
					CARD4_P2.drag();
				}
			}

			// Check availability of CARD1 on line 4 card 1
			if (CARD1_P1.getX() >= 425 && CARD1_P1.getX() <= 435 && CARD1_P1.getY() >= 415 && CARD1_P1.getY() <= 425)
			{
				if (CARD1_P1C == ZeroOrCard)
				{
					if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) || (INITCARD2 == ZeroOne || INITCARD2 == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
				if (CARD1_P1C == OneOrCard)
				{
					if ((INITCARD1 != ZeroOne && INITCARD1 != ZeroZero) && (INITCARD2 == ZeroOne && INITCARD2 == ZeroZero))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
				if (CARD1_P1C == ZeroAndCard)
				{
					if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneAndCard)
				{
					if ((INITCARD1 == OneZero || INITCARD1 == ZeroZero) || (INITCARD2 == OneZero || INITCARD2 == ZeroZero))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
				if (CARD1_P1C == ZeroXorCard)
				{
					if (((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne)) || ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) && (INITCARD1 == OneZero || INITCARD1 == ZeroZero)))
					{
						CARD1_P1.setX(L1_CARD1.getX());
						CARD1_P1.setY(L1_CARD1.getY());

					}
					else
					{
						L4_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}

				if (CARD1_P1C == OneXorCard)
				{
					if (((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne)) || ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) && (INITCARD1 == OneZero || INITCARD1 == ZeroZero)))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
			}

			// Check availability of CARD1 on line 4 card 2
			if (CARD1_P1.getX() >= 555 && CARD1_P1.getX() <= 665 && CARD1_P1.getY() >= 415 && CARD1_P1.getY() <= 425)
			{
				if (CARD1_P1C == ZeroOrCard)
				{
					if ((INITCARD3 == ZeroOne || INITCARD3 == OneOne) || (INITCARD4 == ZeroOne || INITCARD4 == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;

					}
				}
				if (CARD1_P1C == OneOrCard)
				{
					if (!(INITCARD2 == ZeroOne || INITCARD2 == ZeroZero) && !(INITCARD3 == ZeroOne && INITCARD3 == ZeroZero))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
				if (CARD1_P1C == ZeroAndCard)
				{
					if ((INITCARD2 == ZeroOne || INITCARD2 == OneOne) && (INITCARD3 == ZeroOne || INITCARD3 == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;


					}
				}
				if (CARD1_P1C == OneAndCard)
				{
					if ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) || (INITCARD3 == OneZero && INITCARD3 == ZeroZero))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}


				}
				if (CARD1_P1C == ZeroXorCard)
				{
					if (INITCARD2 != INITCARD2)
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneXorCard)
				{
					if (INITCARD2 == INITCARD2)
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
			}

			// Check availability of CARD1 on &LINE 4 card 3
			if (CARD1_P1.getX() >= 685 && CARD1_P1.getX() <= 695 && CARD1_P1.getY() >= 415 && CARD1_P1.getY() <= 425)
			{
				if (CARD1_P1C == ZeroOrCard)
				{
					if ((INITCARD2 == ZeroOne || INITCARD2 == OneOne) || (INITCARD3 == ZeroOne || INITCARD3 == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD3.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD3.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneOrCard)
				{
					if (!(INITCARD3 == ZeroOne || INITCARD3 == ZeroZero) && !(INITCARD4 == ZeroOne && INITCARD4 == ZeroZero))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD3.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD3.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
				if (CARD1_P1C == ZeroAndCard)
				{
					if ((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD4 == OneZero || INITCARD4 == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD3.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD3.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneAndCard)
				{
					if ((INITCARD3 == OneZero || INITCARD3 == ZeroZero) || (INITCARD4 == OneZero || INITCARD4 == ZeroZero))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD3.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD3.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
				if (CARD1_P1C == ZeroXorCard)
				{
					if (((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD3 == OneZero || INITCARD3 == ZeroZero) && (INITCARD4 == OneZero || INITCARD4 == ZeroZero)))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD3.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD3.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneXorCard)
				{
					if (!((((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD3 == ZeroOne || INITCARD3 == OneOne)) || ((INITCARD4 == OneZero || INITCARD4 == ZeroZero) && (INITCARD4 == OneZero || INITCARD4 == ZeroZero)))))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD3.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD3.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
			}

			// Check availability of CARD1 on line 4 card 4
			if (CARD1_P1.getX() >= 795 && CARD1_P1.getX() <= 805 && CARD1_P1.getY() >= 415 && CARD1_P1.getY() <= 425)
			{
				if (CARD1_P1C == ZeroOrCard)
				{
					if ((INITCARD4 == ZeroOne || INITCARD4 == OneOne) || (INITCARD5 == ZeroOne || INITCARD5 == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD4.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD4.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneOrCard)
				{
					if ((INITCARD4 != ZeroOne && INITCARD4 != OneOne) || (INITCARD5 != ZeroOne && INITCARD5 != OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD4.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD4.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
				if (CARD1_P1C == ZeroAndCard)
				{
					if ((INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD5 == ZeroOne || INITCARD5 == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD4.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD4.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneAndCard)
				{
					if ((INITCARD4 == OneZero || INITCARD4 == ZeroZero) && (INITCARD5 == OneZero && INITCARD5 == ZeroZero))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD4.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD4.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
				if (CARD1_P1C == ZeroXorCard)
				{
					if (((INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD5 == OneZero || INITCARD5 == ZeroZero) && (INITCARD5 == OneZero || INITCARD5 == ZeroZero)))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD4.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD4.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneXorCard)
				{
					if (!((((INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD5 == OneZero || INITCARD5 == ZeroZero) && (INITCARD5 == OneZero || INITCARD5 == ZeroZero)))))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L4_CARD4.setTexture(CARD1_P1.getTex());
						drawCount++;
						L4_CARD4.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
			}

			// Check availability of CARD1 on line 3 card 1
			if (CARD1_P1.getX() >= 485 && CARD1_P1.getX() <= 495 && CARD1_P1.getY() >= 480 && CARD1_P1.getY() <= 490)
			{
				if (CARD1_P1C == ZeroOrCard)
				{
					if ((L4_CARD1.getTex() == ZeroOne || L4_CARD1.getTex() == OneOne) || (L4_CARD2.getTex() == ZeroOne || L4_CARD2.getTex() == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneOrCard)
				{
					if ((L4_CARD1.getTex() != ZeroOne && L4_CARD1.getTex() != OneOne) || (L4_CARD2.getTex() != ZeroOne && L4_CARD2.getTex() != OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == ZeroAndCard)
				{
					if ((L4_CARD1.getTex() == ZeroOne || L4_CARD1.getTex() == OneOne) && (L4_CARD2.getTex() == OneZero || L4_CARD2.getTex() == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneAndCard)
				{
					if ((L4_CARD1.getTex() == OneZero || L4_CARD1.getTex() == ZeroZero) || (L4_CARD2.getTex() == OneZero || L4_CARD2.getTex() == ZeroZero))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
				if (CARD1_P1C == ZeroXorCard)
				{
					if (((L4_CARD1.getTex() == ZeroOne || L4_CARD1.getTex() == OneOne) && (L4_CARD2.getTex() == ZeroOne || L4_CARD2.getTex() == OneOne)) ||
						((L4_CARD1.getTex() == OneZero || L4_CARD1.getTex() == ZeroZero) && (L4_CARD2.getTex() == OneZero || L4_CARD2.getTex() == ZeroZero)))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (!((((L4_CARD1.getTex() == ZeroOne || L4_CARD1.getTex() == OneOne) && (L4_CARD2.getTex() == ZeroOne || L4_CARD2.getTex() == OneOne)) ||
					((L4_CARD1.getTex() == OneZero || L4_CARD2.getTex() == ZeroZero) && (L4_CARD2.getTex() == OneZero || L4_CARD2.getTex() == ZeroZero)))))
				{
					if (INITCARD1 == INITCARD2)
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
			}

			// Check availability of CARD1 on line 3 card 2
			if (CARD1_P1.getX() >= 615 && CARD1_P1.getX() <= 625 && CARD1_P1.getY() >= 480 && CARD1_P1.getY() <= 490)
			{
				if (CARD1_P1C == ZeroOrCard)
				{
					if ((L4_CARD2.getTex() == ZeroOne || L4_CARD2.getTex() == OneOne) || (L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD2.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneOrCard)
				{
					if ((L4_CARD2.getTex() != ZeroOne && L4_CARD2.getTex() != OneOne) || (L4_CARD3.getTex() != ZeroOne && L4_CARD3.getTex() != OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD2.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == ZeroAndCard)
				{
					if ((L4_CARD2.getTex() == ZeroOne || L4_CARD2.getTex() == OneOne) && (L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD2.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneAndCard)
				{
					if ((L4_CARD2.getTex() == OneZero || L4_CARD2.getTex() == ZeroZero) || (L4_CARD3.getTex() == OneZero || L4_CARD3.getTex() == ZeroZero))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD2.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
				if (CARD1_P1C == ZeroXorCard)
				{
					if (((L4_CARD2.getTex() == ZeroOne || L4_CARD2.getTex() == OneOne) && (L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne)) || ((L4_CARD2.getTex() == OneZero || L4_CARD2.getTex() == ZeroZero) && (L4_CARD3.getTex() == OneZero || L4_CARD3.getTex() == ZeroZero)))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD2.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneXorCard)
				{
					if (!((((L4_CARD2.getTex() == ZeroOne || L4_CARD2.getTex() == OneOne) && (L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne)) || ((L4_CARD2.getTex() == OneZero || L4_CARD2.getTex() == ZeroZero) && (L4_CARD3.getTex() == OneZero || L4_CARD3.getTex() == ZeroZero)))))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD2.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
			}

			// Check availability of CARD1 on line 3 card 3
			if (CARD1_P1.getX() >= 740 && CARD1_P1.getX() <= 750 && CARD1_P1.getY() >= 480 && CARD1_P1.getY() <= 490)
			{
				if (CARD1_P1C == ZeroOrCard)
				{
					if ((L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne) && (L4_CARD4.getTex() == ZeroOne || L4_CARD4.getTex() == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
				}
				if (CARD1_P1C == OneOrCard)
				{
					if ((L4_CARD3.getTex() == OneZero || L4_CARD3.getTex() == ZeroZero) || (L4_CARD4.getTex() == OneZero || L4_CARD4.getTex() == ZeroZero))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD3.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD3.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == ZeroAndCard)
				{
					if ((L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne) && (L4_CARD4.getTex() == ZeroOne || L4_CARD4.getTex() == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD3.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD3.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneAndCard)
				{
					if ((L4_CARD3.getTex() == OneZero || L4_CARD3.getTex() == ZeroZero) || (L4_CARD4.getTex() == OneZero || L4_CARD4.getTex() == ZeroZero))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD3.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD3.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
				if (CARD1_P1C == ZeroXorCard)
				{
					if (((L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne) && (L4_CARD4.getTex() == ZeroOne || L4_CARD4.getTex() == OneOne)) || ((L4_CARD3.getTex() == OneZero || L4_CARD3.getTex() == ZeroZero) && (L4_CARD4.getTex() == OneZero || L4_CARD4.getTex() == ZeroZero)))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD3.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD3.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (!((((L4_CARD3.getTex() == ZeroOne || L4_CARD3.getTex() == OneOne) && (L4_CARD4.getTex() == ZeroOne || L4_CARD4.getTex() == OneOne)) || ((L4_CARD3.getTex() == OneZero || L4_CARD3.getTex() == ZeroZero) && (L4_CARD4.getTex() == OneZero || L4_CARD4.getTex() == ZeroZero)))))
				{
					if (INITCARD1 == INITCARD2)
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L3_CARD3.setTexture(CARD1_P1.getTex());
						drawCount++;
						L3_CARD3.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
			}

			// Check availability of CARD1 on line 2 card 1
			if (CARD1_P1.getX() >= 555 && CARD1_P1.getX() <= 560 && CARD1_P1.getY() >= 565 && CARD1_P1.getY() <= 575)
			{
				if (CARD1_P1C == ZeroOrCard)
				{
					if ((L3_CARD1.getTex() == ZeroOne || L3_CARD1.getTex() == OneOne) || (L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L2_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneOrCard)
				{
					if ((L3_CARD1.getTex() != ZeroOne && L3_CARD1.getTex() != OneOne) || (L3_CARD2.getTex() != ZeroOne && L3_CARD2.getTex() != OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L2_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
				if (CARD1_P1C == ZeroAndCard)
				{
					if ((L3_CARD1.getTex() == ZeroOne || L3_CARD1.getTex() == OneOne) && (L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L2_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneAndCard)
				{
					if ((L3_CARD1.getTex() == OneZero || L3_CARD1.getTex() == ZeroZero) || (L3_CARD2.getTex() == OneZero || L3_CARD2.getTex() == ZeroZero))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L2_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == ZeroXorCard)
				{
					if (((L3_CARD1.getTex() == ZeroOne || L3_CARD1.getTex() == OneOne) && (L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne)) || ((L3_CARD2.getTex() == OneZero || L3_CARD2.getTex() == ZeroZero) && (L3_CARD1.getTex() == OneZero || L3_CARD1.getTex() == ZeroZero)))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L2_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneXorCard)
				{
					if (!((((L3_CARD1.getTex() == ZeroOne || L3_CARD1.getTex() == OneOne) && (L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne)) || ((L3_CARD2.getTex() == OneZero || L3_CARD2.getTex() == ZeroZero) && (L3_CARD1.getTex() == OneZero || L3_CARD1.getTex() == ZeroZero)))))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L2_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD1.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
			}

			// Check availability of CARD1 on line 2 card 2
			if (CARD1_P1.getX() >= 685 && CARD1_P1.getX() <= 695 && CARD1_P1.getY() >= 565 && CARD1_P1.getY() <= 575)
			{
				if (CARD1_P1C == ZeroOrCard)
				{
					if ((L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne) || (L3_CARD3.getTex() == ZeroOne || L3_CARD3.getTex() == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L2_CARD2.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneOrCard)
				{
					if ((L3_CARD2.getTex() != ZeroOne && L3_CARD2.getTex() != OneOne) || (L3_CARD3.getTex() != ZeroOne && L3_CARD3.getTex() != OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L2_CARD2.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == ZeroAndCard)
				{
					if ((L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne) && (L3_CARD3.getTex() == ZeroOne || L3_CARD3.getTex() == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L2_CARD2.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneAndCard)
				{
					if ((L3_CARD2.getTex() == OneZero || L3_CARD2.getTex() == ZeroZero) || (L3_CARD3.getTex() == OneZero || L3_CARD3.getTex() == ZeroZero))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L2_CARD2.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == ZeroXorCard)
				{
					if (((L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne) && (L3_CARD3.getTex() == ZeroOne || L3_CARD3.getTex() == OneOne)) || ((L3_CARD3.getTex() == OneZero || L3_CARD3.getTex() == ZeroZero) && (L3_CARD2.getTex() == OneZero || L3_CARD2.getTex() == ZeroZero)))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L2_CARD2.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (!((((L3_CARD2.getTex() == ZeroOne || L3_CARD2.getTex() == OneOne) && (L3_CARD3.getTex() == ZeroOne || L3_CARD3.getTex() == OneOne)) || ((L3_CARD3.getTex() == OneZero || L3_CARD3.getTex() == ZeroZero) && (L3_CARD2.getTex() == OneZero || L3_CARD2.getTex() == ZeroZero)))))
				{
					if (INITCARD1 == INITCARD2)
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L2_CARD2.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
			}

			// Check availability of CARD1 on line 1 card 1
			if (CARD1_P1.getX() >= 615 && CARD1_P1.getX() <= 625 && CARD1_P1.getY() >= 615 && CARD1_P1.getY() <= 620)
			{
				if (CARD1_P1C == ZeroOrCard)
				{
					if ((L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne) || (L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L1_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneOrCard)
				{
					if ((L2_CARD1.getTex() != ZeroOne && L2_CARD1.getTex() != OneOne) || (L2_CARD1.getTex() != ZeroOne && L2_CARD1.getTex() != OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L1_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == ZeroAndCard)
				{
					if ((L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne) && (L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L1_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneAndCard)
				{
					if ((L2_CARD1.getTex() == OneZero || L2_CARD1.getTex() == ZeroZero) || (L2_CARD1.getTex() == OneZero || L2_CARD1.getTex() == ZeroZero))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L1_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == ZeroXorCard)
				{
					if (((L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne) && (L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne)) || ((L2_CARD1.getTex() == OneZero || L2_CARD1.getTex() == ZeroZero) && (L2_CARD1.getTex() == OneZero || L2_CARD1.getTex() == ZeroZero)))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L1_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}
				}
				if (CARD1_P1C == OneXorCard)
				{
					if (!((((L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne) && (L2_CARD1.getTex() == ZeroOne || L2_CARD1.getTex() == OneOne)) || ((L2_CARD2.getTex() == OneZero || L2_CARD2.getTex() == ZeroZero) && (L2_CARD1.getTex() == OneZero || L2_CARD1.getTex() == ZeroZero)))))
					{
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
					}
					else
					{
						L1_CARD1.setTexture(CARD1_P1.getTex());
						drawCount++;
						L2_CARD2.setTexture(CARD1_P1.getTex());
						CARD1_P1.setTexture(deck[drawCount].texture);
						CARD1_P1.setX(75);
						CARD1_P1.setY(450);
						isP1Turn = false;
					}

				}
				// P2 

				// Check availability of card1 on line 4 card 1
				if (CARD1_P2.getX() >= 420 && CARD1_P2.getX() <= 440 && CARD1_P2.getY() >= 190 && CARD1_P2.getY() <= 210)
				{
					if (CARD1_P2C == ZeroOrCard)
						if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) || (INITCARD2 == ZeroOne || INITCARD2 == OneOne))
						{
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
						}
						else
						{
							L4_CARD1f.setTexture(CARD1_P2.getTex());
							drawCount++;
							L4_CARD1f.setTexture(CARD1_P2.getTex());
							CARD1_P2.setTexture(deck[drawCount].texture);
							CARD1_P2.setX(77);
							CARD1_P2.setY(40);
							isP1Turn = true;
						}

				}
				if (CARD1_P2C == OneOrCard)
				{
					if ((INITCARD1 != ZeroOne && INITCARD1 != ZeroZero) && (INITCARD2 == ZeroOne && INITCARD2 == ZeroZero))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
				if (CARD1_P2C == ZeroAndCard)
				{
					if ((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneAndCard)
				{
					if ((INITCARD1 == OneZero || INITCARD1 == ZeroZero) || (INITCARD2 == OneZero || INITCARD2 == ZeroZero))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
				if (CARD1_P2C == ZeroXorCard)
				{
					if (((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne)) || ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) && (INITCARD1 == OneZero || INITCARD1 == ZeroZero)))
					{
						CARD1_P2.setX(L1_CARD1f.getX());
						CARD1_P2.setY(L1_CARD1f.getY());

					}
					else
					{
						L4_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}

				if (CARD1_P2C == OneXorCard)
				{
					if (((INITCARD1 == ZeroOne || INITCARD1 == OneOne) && (INITCARD2 == ZeroOne || INITCARD2 == OneOne)) || ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) && (INITCARD1 == OneZero || INITCARD1 == ZeroZero)))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
			}

			// Check availability of card1 on line 4 card 2
			if (CARD1_P2.getX() >= 550 && CARD1_P2.getX() <= 570 && CARD1_P2.getY() >= 190 && CARD1_P2.getY() <= 20)
			{
				if (CARD1_P2C == ZeroOrCard)
				{
					if ((INITCARD3 == ZeroOne || INITCARD3 == OneOne) || (INITCARD4 == ZeroOne || INITCARD4 == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;

					}
				}
				if (CARD1_P2C == OneOrCard)
				{
					if (!(INITCARD2 == ZeroOne || INITCARD2 == ZeroZero) && !(INITCARD3 == ZeroOne && INITCARD3 == ZeroZero))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
				if (CARD1_P2C == ZeroAndCard)
				{
					if ((INITCARD2 == ZeroOne || INITCARD2 == OneOne) && (INITCARD3 == ZeroOne || INITCARD3 == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;


					}
				}
				if (CARD1_P2C == OneAndCard)
				{
					if ((INITCARD2 == OneZero || INITCARD2 == ZeroZero) || (INITCARD3 == OneZero && INITCARD3 == ZeroZero))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}


				}
				if (CARD1_P2C == ZeroXorCard)
				{
					if (INITCARD2 != INITCARD2)
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneXorCard)
				{
					if (INITCARD2 == INITCARD2)
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
			}

			// Check availability of card1 on &line 4 card 3
			if (CARD1_P2.getX() >= 675 && CARD1_P2.getX() <= 695 && CARD1_P2.getY() >= 190 && CARD1_P2.getY() <= 210)
			{
				if (CARD1_P2C == ZeroOrCard)
				{
					if ((INITCARD2 == ZeroOne || INITCARD2 == OneOne) || (INITCARD3 == ZeroOne || INITCARD3 == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD3f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD3f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneOrCard)
				{
					if (!(INITCARD3 == ZeroOne || INITCARD3 == ZeroZero) && !(INITCARD4 == ZeroOne && INITCARD4 == ZeroZero))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD3f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD3f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
				if (CARD1_P2C == ZeroAndCard)
				{
					if ((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD4 == OneZero || INITCARD4 == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD3f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD3f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneAndCard)
				{
					if ((INITCARD3 == OneZero || INITCARD3 == ZeroZero) || (INITCARD4 == OneZero || INITCARD4 == ZeroZero))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD3f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD3f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
				if (CARD1_P2C == ZeroXorCard)
				{
					if (((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD3 == OneZero || INITCARD3 == ZeroZero) && (INITCARD4 == OneZero || INITCARD4 == ZeroZero)))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD3f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD3f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneXorCard)
				{
					if (!((((INITCARD3 == ZeroOne || INITCARD3 == OneOne) && (INITCARD3 == ZeroOne || INITCARD3 == OneOne)) || ((INITCARD4 == OneZero || INITCARD4 == ZeroZero) && (INITCARD4 == OneZero || INITCARD4 == ZeroZero)))))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}//h
					else
					{
						L4_CARD3f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD3f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
			}

			// Check availability of card1 on line 4 card 4
			if (CARD1_P2.getX() >= 790 && CARD1_P2.getX() <= 810 && CARD1_P2.getY() >= 190 && CARD1_P2.getY() <= 210)
			{
				if (CARD1_P2C == ZeroOrCard)
				{
					if ((INITCARD4 == ZeroOne || INITCARD4 == OneOne) || (INITCARD5 == ZeroOne || INITCARD5 == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD4f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD4f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneOrCard)
				{
					if ((INITCARD4 != ZeroOne && INITCARD4 != OneOne) || (INITCARD5 != ZeroOne && INITCARD5 != OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD4f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD4f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
				if (CARD1_P2C == ZeroAndCard)
				{
					if ((INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD5 == ZeroOne || INITCARD5 == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD4f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD4f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneAndCard)
				{
					if ((INITCARD4 == OneZero || INITCARD4 == ZeroZero) && (INITCARD5 == OneZero && INITCARD5 == ZeroZero))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD4f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD4f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
				if (CARD1_P2C == ZeroXorCard)
				{
					if (((
						INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD5 == OneZero || INITCARD5 == ZeroZero) && (INITCARD5 == OneZero || INITCARD5 == ZeroZero)))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD4f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD4f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneXorCard)
				{
					if (!((((INITCARD4 == ZeroOne || INITCARD4 == OneOne) && (INITCARD4 == ZeroOne || INITCARD4 == OneOne)) || ((INITCARD5 == OneZero || INITCARD5 == ZeroZero) && (INITCARD5 == OneZero || INITCARD5 == ZeroZero)))))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L4_CARD4f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L4_CARD4f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
			}

			// Check availability of card1 on line 3 card 1
			if (CARD1_P2.getX() >= 485 && CARD1_P2.getX() <= 495 && CARD1_P2.getY() >= 480 && CARD1_P2.getY() <= 490)
			{
				if (CARD1_P2C == ZeroOrCard)
				{
					if ((L4_CARD1f.getTex() == ZeroOne || L4_CARD1f.getTex() == OneOne) || (L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneOrCard)
				{
					if ((L4_CARD1f.getTex() != ZeroOne && L4_CARD1f.getTex() != OneOne) || (L4_CARD2f.getTex() != ZeroOne && L4_CARD2f.getTex() != OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == ZeroAndCard)
				{
					if ((L4_CARD1f.getTex() == ZeroOne || L4_CARD1f.getTex() == OneOne) && (L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneAndCard)
				{
					if ((L4_CARD1f.getTex() == OneZero || L4_CARD1f.getTex() == ZeroZero) || (L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
				if (CARD1_P2C == ZeroXorCard)
				{
					if (((L4_CARD1f.getTex() == ZeroOne || L4_CARD1f.getTex() == OneOne) && (L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne)) || ((L4_CARD1f.getTex() == OneZero || L4_CARD1f.getTex() == ZeroZero) && (L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero)))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (!((((L4_CARD1f.getTex() == ZeroOne || L4_CARD1f.getTex() == OneOne) && (L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne)) || ((L4_CARD1f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero) && (L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero)))))
				{
					if (INITCARD1 == INITCARD2)
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
			}

			// Check availability of card1 on line 3 card 2
			if (CARD1_P2.getX() >= 615 && CARD1_P2.getX() <= 625 && CARD1_P2.getY() >= 480 && CARD1_P2.getY() <= 490)
			{
				if (CARD1_P2C == ZeroOrCard)
				{
					if ((L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne) || (L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneOrCard)
				{
					if ((L4_CARD2f.getTex() != ZeroOne && L4_CARD2f.getTex() != OneOne) || (L4_CARD3f.getTex() != ZeroOne && L4_CARD3f.getTex() != OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == ZeroAndCard)
				{
					if ((L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne) && (L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneAndCard)
				{
					if ((L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero) || (L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
				if (CARD1_P2C == ZeroXorCard)
				{
					if (((L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne) && (L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne)) || ((L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero) && (L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero)))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneXorCard)
				{
					if (!((((L4_CARD2f.getTex() == ZeroOne || L4_CARD2f.getTex() == OneOne) && (L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne)) || ((L4_CARD2f.getTex() == OneZero || L4_CARD2f.getTex() == ZeroZero) && (L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero)))))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
			}

			// Check availability of card1 on line 3 card 3
			if (CARD1_P2.getX() >= 740 && CARD1_P2.getX() <= 770 && CARD1_P2.getY() >= 480 && CARD1_P2.getY() <= 490)
			{
				if (CARD1_P2C == ZeroOrCard)
				{
					if ((L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne) && (L4_CARD4f.getTex() == ZeroOne || L4_CARD4f.getTex() == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
				}
				if (CARD1_P2C == OneOrCard)
				{
					if ((L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero) || (L4_CARD4f.getTex() == OneZero || L4_CARD4f.getTex() == ZeroZero))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD3f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD3f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == ZeroAndCard)
				{
					if ((L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne) && (L4_CARD4f.getTex() == ZeroOne || L4_CARD4f.getTex() == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD3f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD3f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneAndCard)
				{
					if ((L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero) || (L4_CARD4f.getTex() == OneZero || L4_CARD4f.getTex() == ZeroZero))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD3f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD3f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
				if (CARD1_P2C == ZeroXorCard)
				{
					if (((L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne) && (L4_CARD4f.getTex() == ZeroOne || L4_CARD4f.getTex() == OneOne)) || ((L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero) && (L4_CARD4f.getTex() == OneZero || L4_CARD4f.getTex() == ZeroZero)))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD3f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD3f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (!((((L4_CARD3f.getTex() == ZeroOne || L4_CARD3f.getTex() == OneOne) && (L4_CARD4f.getTex() == ZeroOne || L4_CARD4f.getTex() == OneOne)) || ((L4_CARD3f.getTex() == OneZero || L4_CARD3f.getTex() == ZeroZero) && (L4_CARD4f.getTex() == OneZero || L4_CARD4f.getTex() == ZeroZero)))))
				{
					if (INITCARD1 == INITCARD2)
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L3_CARD3f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L3_CARD3f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
			}

			// Check availability of card1 on line 2 card 1
			if (CARD1_P2.getX() >= 555 && CARD1_P2.getX() <= 560 && CARD1_P2.getY() >= 565 && CARD1_P2.getY() <= 577)
			{
				if (CARD1_P2C == ZeroOrCard)
				{
					if ((L3_CARD1f.getTex() == ZeroOne || L3_CARD1f.getTex() == OneOne) || (L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L2_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneOrCard)
				{
					if ((L3_CARD1f.getTex() != ZeroOne && L3_CARD1f.getTex() != OneOne) || (L3_CARD2f.getTex() != ZeroOne && L3_CARD2f.getTex() != OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L2_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
				if (CARD1_P2C == ZeroAndCard)
				{
					if ((L3_CARD1f.getTex() == ZeroOne || L3_CARD1f.getTex() == OneOne) && (L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L2_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneAndCard)
				{
					if ((L3_CARD1f.getTex() == OneZero || L3_CARD1f.getTex() == ZeroZero) || (L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L2_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == ZeroXorCard)
				{
					if (((L3_CARD1f.getTex() == ZeroOne || L3_CARD1f.getTex() == OneOne) && (L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne)) || ((L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero) && (L3_CARD1f.getTex() == OneZero || L3_CARD1f.getTex() == ZeroZero)))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L2_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneXorCard)
				{
					if (!((((L3_CARD1f.getTex() == ZeroOne || L3_CARD1f.getTex() == OneOne) && (L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne)) || ((L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero) && (L3_CARD1f.getTex() == OneZero || L3_CARD1f.getTex() == ZeroZero)))))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L2_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD1f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
			}

			// Check availability of card1 on line 2 card 2
			if (CARD1_P2.getX() >= 685 && CARD1_P2.getX() <= 695 && CARD1_P2.getY() >= 565 && CARD1_P2.getY() <= 577)
			{
				if (CARD1_P2C == ZeroOrCard)
				{
					if ((L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne) || (L3_CARD3f.getTex() == ZeroOne || L3_CARD3f.getTex() == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneOrCard)
				{
					if ((L3_CARD2f.getTex() != ZeroOne && L3_CARD2f.getTex() != OneOne) || (L3_CARD3f.getTex() != ZeroOne && L3_CARD3f.getTex() != OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == ZeroAndCard)
				{
					if ((L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne) && (L3_CARD3f.getTex() == ZeroOne || L3_CARD3f.getTex() == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneAndCard)
				{
					if ((L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero) || (L3_CARD3f.getTex() == OneZero || L3_CARD3f.getTex() == ZeroZero))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == ZeroXorCard)
				{
					if (((L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne) && (L3_CARD3f.getTex() == ZeroOne || L3_CARD3f.getTex() == OneOne)) || ((L3_CARD3f.getTex() == OneZero || L3_CARD3f.getTex() == ZeroZero) && (L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero)))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (!((((L3_CARD2f.getTex() == ZeroOne || L3_CARD2f.getTex() == OneOne) && (L3_CARD3f.getTex() == ZeroOne || L3_CARD3f.getTex() == OneOne)) || ((L3_CARD3f.getTex() == OneZero || L3_CARD3f.getTex() == ZeroZero) && (L3_CARD2f.getTex() == OneZero || L3_CARD2f.getTex() == ZeroZero)))))
				{
					if (INITCARD1 == INITCARD2)
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
			}

			// Check availability of card1 on line 1 card 1
			if (CARD1_P2.getX() >= 615 && CARD1_P2.getX() <= 625 && CARD1_P2.getY() >= 615 && CARD1_P2.getY() <= 620)
			{
				if (CARD1_P2C == ZeroOrCard)
				{
					if ((L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne) || (L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L1_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneOrCard)
				{
					if ((L2_CARD1f.getTex() != ZeroOne && L2_CARD1f.getTex() != OneOne) || (L2_CARD1f.getTex() != ZeroOne && L2_CARD1f.getTex() != OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L1_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == ZeroAndCard)
				{
					if ((L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne) && (L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L1_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneAndCard)
				{
					if ((L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero) || (L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L1_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == ZeroXorCard)
				{
					if (((L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne) && (L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne)) || ((L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero) && (L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero)))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L1_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}
				}
				if (CARD1_P2C == OneXorCard)
				{
					if (!((((L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne) && (L2_CARD1f.getTex() == ZeroOne || L2_CARD1f.getTex() == OneOne)) || ((L2_CARD2f.getTex() == OneZero || L2_CARD2f.getTex() == ZeroZero) && (L2_CARD1f.getTex() == OneZero || L2_CARD1f.getTex() == ZeroZero)))))
					{
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
					}
					else
					{
						L1_CARD1f.setTexture(CARD1_P2.getTex());
						drawCount++;
						L2_CARD2f.setTexture(CARD1_P2.getTex());
						CARD1_P2.setTexture(deck[drawCount].texture);
						CARD1_P2.setX(77);
						CARD1_P2.setY(40);
						isP1Turn = true;
					}

				}
			}

			ENTITY DRAWDECK(125, 303, DeckAsset);
			window.render(DRAWDECK);

			window.render(INIT1);
			window.render(INIT2);
			window.render(INIT3);
			window.render(INIT4);
			window.render(INIT5);

			window.render(L4_CARD1);
			window.render(L4_CARD2);
			window.render(L4_CARD3);
			window.render(L4_CARD4);

			window.render(L3_CARD1);
			window.render(L3_CARD2);
			window.render(L3_CARD3);

			window.render(L2_CARD1);
			window.render(L2_CARD2);

			window.render(L1_CARD1);

			window.render(L4_CARD1f);
			window.render(L4_CARD2f);
			window.render(L4_CARD3f);
			window.render(L4_CARD4f);

			window.render(L3_CARD1f);
			window.render(L3_CARD2f);
			window.render(L3_CARD3f);

			window.render(L2_CARD1f);
			window.render(L2_CARD2f);

			window.render(L1_CARD1f);

			window.render(CARD1_P1);
			window.render(CARD2_P1);
			window.render(CARD3_P1);
			window.render(CARD4_P1);

			window.render(CARD1_P2);
			window.render(CARD2_P2);
			window.render(CARD3_P2);
			window.render(CARD4_P2);

			window.render(playerBlockRect);
			window.render(RuleCard);
			SDL_RenderPresent(window.getRenderer());
			}
			else if (buttonPressed == 4)
			{
				window.render(comingSoonScreen);
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
			home = false; // link to PvC
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
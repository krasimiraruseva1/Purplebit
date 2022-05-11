#include "GamePlay.hpp"
#include "entity.hpp"
#include "renderWindow.hpp"
#include <cstdlib>
#include <ctime> 



/*void GamePlay::swap(int* a, int* b)
{
    int* temp = a;
     a = b;
    b = temp;
}
GamePlay gameplay;

void GamePlay::shuffle(int card[], int n)
{
    // Initialize seed randomly
    srand(time(0));

    for (int i = 0; i < n; i++)
    {
        // Random for remaining positions.
        int r = i + (rand() % (48 - i));

        gameplay.swap(card[i], card[r]);
    }
}
*/
//void GamePlay::InitGame()
//{
//    int deck[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8,
//                  9, 10, 11, 12, 13, 14, 15,
//                  16, 17, 18, 19, 20, 21, 22,
//                  23, 24, 25, 26, 27, 28, 29,
//                  30, 31, 32, 33, 34, 35, 36,
//                  37, 38, 39, 40, 41, 42, 43,
//                  44, 45, 46, 47};
//
//
//
//    shuffle(deck, 48);
//
//    int P1hand[3];
//    int P2hand[3];
//
//    for (int i = 0; i <= 4; i++)
//    {
//        P1hand[i] = deck[i];
//        for (int j = i; i <= 48; i++)
//            deck[i] = deck[i + 1];
//        deck[48 - 1] = 0;
//    }
//
//    for (int i = 4; i <= 8; i++)
//    {
//        P2hand[i] = deck[i];
//        for (int j = i; i <= 48; i++)
//            deck[i] = deck[i + 1];
//        deck[48 - 1] = 0;
//        std::cout << P2hand[i] << " ";
//    }
//
//  
//
//}

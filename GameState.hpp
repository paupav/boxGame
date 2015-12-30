#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include "Cliff.hpp"
#include "Fireball.hpp"

class Cliff;
class Fireball;


class GameState
{
    public:
        GameState(sf::RenderWindow &windoww, Cliff &clifff, GameState &gameStatee);
        void run();
        void basicMovement(sf::Vector2f &objectPos, sf::Vector2i &desiredPos, bool keepMoving);
        sf::Vector2f pos1;
        sf::Sprite player1;

        std::vector<Fireball> fireballs;
        std::vector<int> deletedFireballs;
        int numberOfFireballs;

    private:

        void changeScale();
        void resourceManagment();
        void characterControlls();
        void createFireball();
        void updateDraws();
        void deleteFireballs();
        void collisions();
        bool collided(sf::Sprite obj1, sf::Sprite obj2);
        bool collided(sf::Vector2i pos1, sf::Vector2i pos2, int size1, int size2);

        bool allowR, allowL, allowU, allowD;

        void loadMap();

        int numberOfBoxes;

        sf::Texture backgroundT, boxT, player1T, fireballT;
        sf::Sprite backgroundS, box[100];


        double tanges = 0;

        sf::Sprite createBox(float posX, float posY);

        sf::RenderWindow *window;
        Cliff *cliff;
        GameState *gameState;


};


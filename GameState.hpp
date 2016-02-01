#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include "Cliff.hpp"
#include "Fireball.hpp"
#include "Player.hpp"

class Cliff;
class Fireball;
class Player;


class GameState
{
    public:
        GameState(sf::RenderWindow &windoww, Cliff &clifff, GameState &gameStatee);
        void run();
        void basicMovement(sf::Vector2f &objectPos, sf::Vector2i &desiredPos, bool keepMoving);
        sf::Vector2f desiredPos;

        std::vector<Fireball> fireballs;
        std::vector<Player> players;
        std::vector<int> deletedFireballs;
        int numberOfFireballs;

        sf::Texture playerT;

    private:

        void changeScale();
        void resourceManagment();
        void characterControlls();
        void createFireball();
        void updateDraws();
        void deleteFireballs();
        void collisions();
        void handleNetwork();

        void createPlayer();
        bool collided(sf::Sprite obj1, sf::Sprite obj2);
        bool collided(sf::Vector2i pos1, sf::Vector2i pos2, int size1, int size2);


        void loadMap();

        int numberOfBoxes;
        int numberOfClients, numberOfPlayers;
        int myID;
        int currentPlayer;
        bool join;

        sf::Texture backgroundT, boxT, fireballT;
        sf::Sprite backgroundS, box[100];
        sf::UdpSocket socket;
        sf::IpAddress ip;
        unsigned short port;


        float tangesLocal = 0;

        sf::Sprite createBox(float posX, float posY);

        sf::RenderWindow *window;
        Cliff *cliff;
        GameState *gameState;


};


#pragma once
#include "GameState.hpp"
#include "Cliff.hpp"

class GameState;
class Cliff;

class Fireball
{
public:
    Fireball(GameState &gameStatee, Cliff &clifff, sf::Texture &texture, std::vector<Fireball> &fireballss);
    sf::Vector2f startingPos;
    sf::Vector2i directionPos;
    float rotation;
    sf::Sprite fireballS;
    void updatePosition();
    bool collided;
    std::vector<Fireball>::iterator it;


private:

    sf::Vector2f newPos;
    GameState *gameState;
    Cliff *cliff;
    sf::Texture *fireballT;
    std::vector<Fireball> *fireballs;

};

#pragma once
#include <SFML/Graphics.hpp>
#include "Cliff.hpp"
#include "GameState.hpp"
class Cliff;
class GameState;
class Player
{
public:
    Player(Cliff &clifff, GameState &gameStatee, sf::Texture &texture);
    void updateSprite();

    sf::Vector2f getPos();
    float getRot();

    sf::Sprite playerS;
    sf::Vector2f pos;
    float tanges;


private:

    Cliff *cliff;
    GameState *gameState;
};

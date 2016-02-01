#include "Player.hpp"
#include "Cliff.hpp"
#include <iostream> // delete later

Player::Player(Cliff &clifff, GameState &gameStatee, sf::Texture &texture)
{
    cliff = &clifff;
    gameState = &gameStatee;
    playerS.setTexture(texture);
    playerS.setOrigin(playerS.getLocalBounds().width/2, playerS.getLocalBounds().height/2);
    playerS.setScale(BOXCONST*2.5*cliff->scalingFactor.x, BOXCONST*2.5*cliff->scalingFactor.y);
    playerS.setPosition(200, 200);
}

sf::Vector2f Player::getPos()
{
    return pos;
}

float Player::getRot()
{
    return playerS.getRotation() *RAD;
}

void Player::updateSprite()
{
    playerS.setPosition(pos);
    playerS.setRotation(tanges - 90);
}




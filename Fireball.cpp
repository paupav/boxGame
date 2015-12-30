#include "Fireball.hpp"

//Fireball
Fireball::Fireball(GameState &gameStatee, Cliff &clifff, sf::Texture &texture, std::vector<Fireball> &fireballss)
{
    gameState = &gameStatee;
    cliff = &clifff;
    fireballs = &fireballss;
    fireballT = &texture;
    it = fireballs->end();
    directionPos.x = sf::Mouse::getPosition(cliff->window).x;
    directionPos.y = sf::Mouse::getPosition(cliff->window).y;
    fireballS.setTexture(*fireballT);
    fireballS.setOrigin(fireballS.getLocalBounds().width/2, fireballS.getLocalBounds().height/2);
    fireballS.setScale(BOXCONST *1.5 * cliff->scalingFactor.x, BOXCONST * 1.5 * cliff->scalingFactor.y);
    fireballS.setRotation(gameState->player1.getRotation()-90);
    startingPos.x = gameState->pos1.x;
    startingPos.y = gameState->pos1.y;
    fireballS.setPosition(startingPos.x, gameState->pos1.y);
    collided = false;


}

void Fireball::updatePosition()
{
    if(!collided)
    {
        gameState->basicMovement(startingPos, directionPos, true);
        fireballS.setPosition(startingPos.x, startingPos.y);
    }
    if(collided)
        fireballS.setPosition(cliff->res.x+ fireballS.getLocalBounds().width, cliff->res.y + fireballS.getLocalBounds().height);

}

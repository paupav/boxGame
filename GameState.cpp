#include "GameState.hpp"
#include <iostream> //delete
#include <SFML/Graphics.hpp>
#include <cmath>
#include <math.h>
#include <fstream>
#include <sstream>

#define RAD 57.29

bool delBool;

GameState::GameState(sf::RenderWindow &windoww, Cliff &clifff, GameState &gameStatee)
{
    window = &windoww;
    cliff = &clifff;
    gameState = &gameStatee;

    resourceManagment();
    changeScale();
    player1.setOrigin(player1.getLocalBounds().width/2, player1.getLocalBounds().height/2);
    pos1.x = 200;
    pos1.y = 200;

    numberOfBoxes = -1;

    loadMap();



}

//Cliff of GameState
void GameState::run()
{
    characterControlls();
    deleteFireballs();
    collisions();
    updateDraws();
}

void GameState::characterControlls()
{

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) )
        pos1.x -= 4 * cliff->scalingFactor.x;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) )
        pos1.x += 4 * cliff->scalingFactor.x;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) )
        pos1.y -= 4 * cliff->scalingFactor.y;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) )
        pos1.y += 4 * cliff->scalingFactor.y;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        cliff->changeState(cliff->menu);


    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        createFireball();

    tanges = atan2(pos1.y - sf::Mouse::getPosition(cliff->window).y, pos1.x - sf::Mouse::getPosition(cliff->window).x)* RAD;
    player1.setRotation(tanges - 90);

    player1.setPosition(pos1.x, pos1.y);

}

void GameState::changeScale()
{
    cliff->createWindow();
    backgroundS.scale(cliff->res.x/backgroundS.getGlobalBounds().width, cliff->res.y/backgroundS.getGlobalBounds().height);
    player1.setScale(BOXCONST*2.5*cliff->scalingFactor.x, BOXCONST*2.5*cliff->scalingFactor.y);

}

//load textures and sprites
void GameState::resourceManagment()
{
    if(!boxT.loadFromFile("box.png") )
        cliff->changeState(cliff->close);
    if(!player1T.loadFromFile("player1.jpg"))
        cliff->changeState(cliff->close);
    if(!backgroundT.loadFromFile("lvl1b.jpg"))
        cliff->changeState(cliff->close);
    if(!fireballT.loadFromFile("fireball.png"))
        cliff->changeState(cliff->close);

    backgroundS.setTexture(backgroundT);
    player1.setTexture(player1T);



}

//Box
sf::Sprite GameState::createBox(float posX, float posY)
{
    sf::Sprite boxModel;
    boxModel.setTexture(boxT);
    boxModel.setPosition(posX, posY);
    boxModel.setScale(BOXCONST * cliff->scalingFactor.x, BOXCONST * cliff->scalingFactor.y);
    boxModel.setOrigin(boxModel.getLocalBounds().width/2, boxModel.getLocalBounds().height/2);
    return boxModel;
}



void GameState::updateDraws()
{

    window->clear(sf::Color::Black);
    window->draw(player1);

    numberOfFireballs = fireballs.size();

    while(numberOfFireballs > 0)
    {
        --numberOfFireballs;
        fireballs.at(numberOfFireballs).updatePosition();
        window->draw(fireballs.at(numberOfFireballs).fireballS);
    }
    for(int i = numberOfBoxes-1; i >= 0; i--)
        window->draw(box[i]);
    window->display();
}

void GameState::basicMovement(sf::Vector2f &objectPos, sf::Vector2i &desiredPos, bool keepMoving)
{
    float a, b, c, h;
    sf::Vector2f helpVec, vec;
    helpVec.x = desiredPos.x;
    helpVec.y = desiredPos.y;
    a = desiredPos.x - objectPos.x;
    b = desiredPos.y - objectPos.y;
    c = sqrt(a*a + b*b);

    vec = helpVec - objectPos;
    vec.x = vec.x/c;
    vec.y = vec.y/c;

    if(desiredPos.x > objectPos.x && !(objectPos.x < desiredPos.x+5 && objectPos.x > desiredPos.x-5) || keepMoving)
    {
        objectPos.x+=(5 * a/c)* cliff->scalingFactor.x;
        if(keepMoving)
            desiredPos.x+=10 * vec.x;
    }
    if(desiredPos.y > objectPos.y && !(objectPos.y < desiredPos.y+5 && objectPos.y > desiredPos.y-5) || keepMoving)
    {
        objectPos.y+=(5 * b/c)* cliff->scalingFactor.y;
        if(keepMoving)
            desiredPos.y+=10  * vec.y;
    }
    if(desiredPos.x < objectPos.x && !(objectPos.x < desiredPos.x+5 && objectPos.x > desiredPos.x-5) || keepMoving)
    {
        objectPos.x+= (5 * a/c)* cliff->scalingFactor.x;
        if(keepMoving)
            desiredPos.x+=10 * vec.x;
    }
    if(desiredPos.y < objectPos.y && !(objectPos.y < desiredPos.y+5 && objectPos.y > desiredPos.y-5) || keepMoving)
    {
        objectPos.y+=(5 * b/c)* cliff->scalingFactor.y;
        if(keepMoving)
            desiredPos.y+=10  * vec.y;
    }


}

void GameState::createFireball()
{
    Fireball fireball(*gameState, *cliff, fireballT, fireballs);

    if(deletedFireballs.empty())
            fireballs.push_back(fireball);
    else if(!deletedFireballs.empty())
    {
        //reusing fireball
        fireballs.at(deletedFireballs.back()) = fireball;
        deletedFireballs.pop_back();
    }


}

void GameState::deleteFireballs()
{
    //frees up memory
    int i = 0;
    if(deletedFireballs.empty())
        while(i < fireballs.size())
        {
            if((fireballs.at(i).startingPos.x > cliff->res.x || fireballs.at(i).startingPos.x < 0
            || fireballs.at(i).startingPos.y > cliff->res.y || fireballs.at(i).startingPos.y < 0) ||
            fireballs.at(i).collided == true)
                deletedFireballs.push_back(i);


            i++;
        }
}

void GameState::loadMap()
{
    std::ifstream file("paulo.boxMap");
    std::istringstream iss ();
    std::string line;
    sf::Vector2f boxPos;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            std::istringstream iss (line);
            float a, b;

            if (!(iss >> a >> b)) { break; }
                box[numberOfBoxes++] = createBox(a *cliff->scalingFactor.x , b *cliff->scalingFactor.y);

        }
        file.close();
    }
}

void GameState::collisions()
{
    int i = fireballs.size() -1;
    int y;

    //fireball - box collision
    while(i >= 0)
    {
        y = numberOfBoxes;
        while(y >= 0)
        {
            if(!fireballs.at(i).collided)
                fireballs.at(i).collided = cliff->collided(box[y], fireballs.at(i).fireballS);
            y--;
        }
        i--;

    }

    //player - box collision tofix
    y = numberOfBoxes - 1;
    /*while(y >= 0)
    {
        if(box[i].getPosition.x - box[i].getSize.width/2 < pos1.x + player1.getLocalBounds().width && )
        {

        }
        y--;
    }*/

}

/*bool GameState::collided(sf::Vector2i pos1, sf::Vector2i pos2, float size1, float size2);
{

}*/

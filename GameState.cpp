#include "GameState.hpp"
#include "Player.hpp"
#include "Cliff.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <cmath>
#include <math.h>
#include <fstream>
#include <sstream>




GameState::GameState(sf::RenderWindow &windoww, Cliff &clifff, GameState &gameStatee)
{
    window = &windoww;
    cliff = &clifff;
    gameState = &gameStatee;
    playerT.loadFromFile("player1.jpg");
    //createPlayer(); //local player


    resourceManagment();
    changeScale();

    numberOfBoxes = -1;
    numberOfPlayers = -1;
    loadMap();
    socket.bind(sf::Socket::AnyPort);

    join = false;
    ip = sf::IpAddress::getLocalAddress();
    port = 5400;



}

void GameState::createPlayer()
{
    Player player(*cliff, *gameState, playerT);
    players.push_back(player);
    std::cout << "New player created under number: " << players.size()-1 << std::endl;
}

//Cliff of GameState
void GameState::run()
{
    handleNetwork();
    characterControlls();
    deleteFireballs();
    collisions();
    updateDraws();
}

void GameState::characterControlls()
{

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) )
        players.at(myID).pos.x -= 4 * cliff->scalingFactor.x;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) )
        players.at(myID).pos.x += 4 * cliff->scalingFactor.x;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) )
        players.at(myID).pos.y -= 4 * cliff->scalingFactor.y;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) )
        players.at(myID).pos.y += 4 * cliff->scalingFactor.y;

    //delete later - use events
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        cliff->changeState(cliff->menu);


    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        createFireball();

    //tanges for local player fix
    tangesLocal = atan2(players.at(myID).pos.y - sf::Mouse::getPosition(cliff->window).y, players.at(myID).pos.x - sf::Mouse::getPosition(cliff->window).x)* RAD;

}

void GameState::changeScale()
{
    cliff->createWindow();
    backgroundS.scale(cliff->res.x/backgroundS.getGlobalBounds().width, cliff->res.y/backgroundS.getGlobalBounds().height);

}

//load textures and sprites
void GameState::resourceManagment()
{
    if(!boxT.loadFromFile("box.png") )
        cliff->changeState(cliff->close);
    if(!playerT.loadFromFile("player1.jpg"))
        cliff->changeState(cliff->close);
    if(!backgroundT.loadFromFile("lvl1b.jpg"))
        cliff->changeState(cliff->close);
    if(!fireballT.loadFromFile("fireball.png"))
        cliff->changeState(cliff->close);

    backgroundS.setTexture(backgroundT);

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

    numberOfFireballs = fireballs.size();
    players.at(myID).tanges = tangesLocal;
    players.at(myID).updateSprite();

    for(int i = numberOfPlayers; i >= 0; i--)
    {
        players.at(i).updateSprite();
    }



    while(numberOfFireballs > 0)
    {
        --numberOfFireballs;
        fireballs.at(numberOfFireballs).updatePosition();
        window->draw(fireballs.at(numberOfFireballs).fireballS);
    }
    for(int i = numberOfBoxes-1; i >= 0; i--)
        window->draw(box[i]);
    for(int i = numberOfPlayers; i >= 0; i--)
        window->draw(players.at(i).playerS);
    window->display();
}

//clean up
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
        objectPos.x+=(4 * a/c)* cliff->scalingFactor.x;
        if(keepMoving)
            desiredPos.x+=10 * vec.x;
    }
    if(desiredPos.y > objectPos.y && !(objectPos.y < desiredPos.y+5 && objectPos.y > desiredPos.y-5) || keepMoving)
    {
        objectPos.y+=(4 * b/c)* cliff->scalingFactor.y;
        if(keepMoving)
            desiredPos.y+=10  * vec.y;
    }
    if(desiredPos.x < objectPos.x && !(objectPos.x < desiredPos.x+5 && objectPos.x > desiredPos.x-5) || keepMoving)
    {
        objectPos.x+= (4 * a/c)* cliff->scalingFactor.x;
        if(keepMoving)
            desiredPos.x+=10 * vec.x;
    }
    if(desiredPos.y < objectPos.y && !(objectPos.y < desiredPos.y+5 && objectPos.y > desiredPos.y-5) || keepMoving)
    {
        objectPos.y+=(4 * b/c)* cliff->scalingFactor.y;
        if(keepMoving)
            desiredPos.y+=10  * vec.y;
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
//################################################ FIREBALL ##########################################
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

void GameState::handleNetwork()
{
    sf::Packet sPacket, rPacket;
    if(!join)
    {
        sPacket << join;// sending anything just to make server notice me
        socket.send(sPacket, ip, port) != sf::Socket::Done;
        socket.receive(rPacket, ip, port);
        rPacket >> myID >> numberOfClients;
        std::cout << "received ID" << myID <<std::endl;
        join = true;
        while(numberOfClients > numberOfPlayers)
        {
            createPlayer();
            numberOfPlayers++;
        }
    }

    sPacket << myID << players.at(myID).pos.x << players.at(myID).pos.y << players.at(myID).tanges; //fix
    std::cout << "Sent: " << myID << " pos: " << players.at(myID).pos.x << ", " << players.at(myID).pos.y  << std::endl;
    socket.send(sPacket, ip, port);
    if(socket.receive(rPacket, ip, port) == sf::Socket::Done) //fix
    {
        sf::Vector2f pos;
        float rotation = 0;
        sf::Packet cPacket;
        cPacket = rPacket;
        //if(cPacket >> numberOfClients >> currentPlayer >> pos.x >> pos.y >> rotation)
            rPacket >> numberOfClients >> currentPlayer >> pos.x >> pos.y >> tangesLocal; //fix
        while(numberOfClients > numberOfPlayers)
        {
            createPlayer();
            numberOfPlayers++;
        }
        //std::cout << "Received: " << currentPlayer << " Pos: " << pos.x << ", " <<pos.y << std::endl;
        //set positions for players
        if(currentPlayer != myID)
        {
            players.at(currentPlayer).pos = pos;
            std::cout << "Player: " << currentPlayer << "Has pos: " << pos.x << ", " << pos.y << std::endl;
            //players.at(currentPlayer).setRotation(rotation * RAD);
        }

    }


}

#include "Cliff.hpp"
#include "Icon.hpp"
#include <iostream>
Cliff::Cliff(Cliff &clifff)
{

    States.push_back(init);
    scalingFactor.x = 0.5;
    scalingFactor.y = 0.5;
    icon.loadFromFile("boxIcon.png");
    createWindow();
    cliff = &clifff;

    sf::Mouse::setPosition(res, window);

}


void Cliff::run()
{
    changeState(menu); //delete later

    InitState initState;
    MenuState menuState(window, *cliff);
    GameState gameState(window, *cliff, gameState);
    EditorState editorState(window, *cliff);

    while(window.isOpen())
    {
    switch (States.back())
    {
        case 0:
            window.close();
        break;
        case 1: //Init state
            initState.run();
        break;
        case 2: //Menu state
            menuState.run();
        break;
        case 3: //Game state
            gameState.run();
        break;
        case 4: //Pause state
        break;
        case 5: //Editor state
            editorState.run();
        break;


    }

    }

}

void Cliff::changeState(int requestedState)
{

    if(requestedState == pause && States.back() != game) // app can only be paused while in "game" state
        return;
    if(requestedState != States.back() && requestedState > States.back())
        States.push_back(requestedState);
    if(requestedState < States.back())
        while(requestedState != States.back())
            States.pop_back();

}



void Cliff::createWindow()
{
    res.x = 1920 * scalingFactor.x;
    res.y = 1080 * scalingFactor.y;
    gameRes.x = res.x - (76 *scalingFactor.x);
    gameRes.y = res.y - (0 *scalingFactor.y);
    settings.antialiasingLevel = 16;
    window.create(sf::VideoMode(res.x, res.y), "Game", sf::Style::Titlebar, settings);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(true);
    window.setIcon(32, 32, icon.getPixelsPtr());

}

bool Cliff::collided(sf::Sprite obj1, sf::Sprite obj2)
{
            //fireball position  <= box position right side
    if(obj2.getPosition().x - obj2.getGlobalBounds().width/2 <= obj1.getPosition().x + obj1.getGlobalBounds().width/2
    //fireball position  >= box position left side
    && obj2.getPosition().x + obj2.getGlobalBounds().width/2 >= obj1.getPosition().x - obj1.getGlobalBounds().width/2
    //fireball position  <= box position bot side
    && obj2.getPosition().y - obj2.getGlobalBounds().height/2 <= obj1.getPosition().y + obj1.getGlobalBounds().height/2
    //fireball position  >= box position top side
    && obj2.getPosition().y + obj2.getGlobalBounds().height/2 >= obj1.getPosition().y - obj1.getGlobalBounds().height/2)
    {
        return true;
    }

}



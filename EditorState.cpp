#include <SFML/Graphics.hpp>
#include "EditorState.hpp"
#include <iostream> //delete
#include <sstream>
#include <string>
#include <fstream>


EditorState::EditorState(sf::RenderWindow &windoww, Cliff &clifff)
{
    window = &windoww;
    cliff = &clifff;
    boxT.loadFromFile("box.png");


    hRow = 26;
    vRow = 24;

    grid = true;
    builderAssistant = true;

    font.loadFromFile("stellar.otf");
    gridT.setFont(font);
    gridT.setString("Grid    ");
    gridT.setCharacterSize(20*cliff->scalingFactor.x);
    gridT.setOrigin(0, gridT.getLocalBounds().height/2);
    gridT.setPosition(cliff->res.x - gridT.getLocalBounds().width, 50 * cliff->scalingFactor.y);
    gridT.setColor(sf::Color::Green);
    builderAssistantT.setFont(font);
    builderAssistantT.setString("Builder \n Assist ");
    builderAssistantT.setCharacterSize(20*cliff->scalingFactor.x);
    builderAssistantT.setOrigin(0, builderAssistantT.getLocalBounds().height/2);
    builderAssistantT.setPosition(cliff->res.x - builderAssistantT.getLocalBounds().width, 220 * cliff->scalingFactor.y);
    builderAssistantT.setColor(sf::Color::Green);
    mapNameT.setFont(font);
    mapNameT.setCharacterSize(20*cliff->scalingFactor.x);



    placeHolder.setSize(sf::Vector2f(boxT.getSize().x * BOXCONST  * cliff->scalingFactor.x-1, boxT.getSize().y* BOXCONST  * cliff->scalingFactor.y-1));
    placeHolder.setOrigin(placeHolder.getLocalBounds().width/2, placeHolder.getLocalBounds().height/2);
    placeHolder.setOutlineThickness(1);
    placeHolder.setOutlineColor(sf::Color(100, 250, 50));
    placeHolder.setFillColor(sf::Color::Transparent);


    //visual grid
    for(int i = 0; i < 25; i++)
    {
        //vertical grid
        gridV[i].setSize(sf::Vector2f(1, cliff->res.y));
        gridV[i].setFillColor(sf::Color(128, 128, 128));
        gridV[i].setPosition(boxT.getSize().x * BOXCONST  * cliff->scalingFactor.x * i, 0);
    }
    for(int i = 0; i < 23; i++)
    {
        //horizontal grid
        gridH[i].setSize(sf::Vector2f(cliff->res.x - (76* cliff->scalingFactor.y), 1));
        gridH[i].setFillColor(sf::Color(128, 128, 128));
        gridH[i].setPosition(0, boxT.getSize().y * BOXCONST  * cliff->scalingFactor.y * i);
    }
}

void EditorState::run()
{
if(builderAssistant)
{
    for(int i = vRow; i >= 0; i--)
        if(sf::Mouse::getPosition(cliff->window).x > 0 * i && sf::Mouse::getPosition(cliff->window).x < placeHolder.getLocalBounds().width * i)
            placeHolder.setPosition((placeHolder.getLocalBounds().width-1) * i - placeHolder.getGlobalBounds().width/2, placeHolder.getPosition().y);

    for(int i = hRow; i >= 0; i--)
        if(sf::Mouse::getPosition(cliff->window).y > 0 * i && sf::Mouse::getPosition(cliff->window).y < placeHolder.getLocalBounds().height * i)
            placeHolder.setPosition(placeHolder.getPosition().x ,(placeHolder.getLocalBounds().height-1) * i - placeHolder.getGlobalBounds().height/2);
}
else
    placeHolder.setPosition(sf::Mouse::getPosition(cliff->window).x, sf::Mouse::getPosition(cliff->window).y);

    handleEvents();
    window->clear(sf::Color::Black);
    for(int i = numberOfBoxes; i >= 0; i--)
        window->draw(boxS[i]);
    if(grid)
    {
        for(int i = 0; i < 25; i++)
            window->draw(gridV[i]);
        for(int i = 0; i < 23; i++)
            window->draw(gridH[i]);
    }


    if(sf::Mouse::getPosition(cliff->window).x <= cliff->gameRes.x)
        window->draw(placeHolder);
    window->draw(gridT);
    window->draw(builderAssistantT);
    window->draw(mapNameT);
    window->display();

    mapNameT.setString(mapName);

}

void EditorState::createBox()
{
    if(delBoxes.empty())
    {
        boxS[numberOfBoxes].setTexture(boxT);
        boxS[numberOfBoxes].setScale(BOXCONST * cliff->scalingFactor.x, BOXCONST  * cliff->scalingFactor.y);
        boxS[numberOfBoxes].setOrigin(boxS[numberOfBoxes].getLocalBounds().width/2, boxS[numberOfBoxes].getLocalBounds().height/2);
        if(builderAssistant)
            boxS[numberOfBoxes].setPosition(placeHolder.getPosition().x, placeHolder.getPosition().y);
        if(!builderAssistant)
            boxS[numberOfBoxes].setPosition(sf::Mouse::getPosition(cliff->window).x, sf::Mouse::getPosition(cliff->window).y);
        numberOfBoxes++;
    }
    else
    {
        boxS[delBoxes.back()].setTexture(boxT);
        boxS[delBoxes.back()].setScale(BOXCONST * cliff->scalingFactor.x, BOXCONST  * cliff->scalingFactor.y);
        boxS[delBoxes.back()].setOrigin(boxS[delBoxes.back()].getLocalBounds().width/2, boxS[delBoxes.back()].getLocalBounds().height/2);
        if(builderAssistant)
            boxS[delBoxes.back()].setPosition(placeHolder.getPosition().x, placeHolder.getPosition().y);
        if(!builderAssistant)
            boxS[delBoxes.back()].setPosition(sf::Mouse::getPosition(cliff->window).x, sf::Mouse::getPosition(cliff->window).y);
        delBoxes.pop_back();
    }

}

void EditorState::deleteBox()
{
    for(int i = numberOfBoxes-1; i >= 0; i--)
        if(collided(boxS[i], placeHolder))
        {
            delBoxes.push_back(i);
            boxS[i].setPosition(cliff->res.x + boxS[i].getGlobalBounds().height, cliff->res.y + boxS[i].getGlobalBounds().width);
        }

}


void EditorState::handleEvents()
{
    while(cliff->window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
        case sf::Keyboard::Escape:
            cliff->window.close();
        break;
        case sf::Event::MouseButtonReleased:
            if(event.key.code == sf::Mouse::Left)
            {
                if(sf::Mouse::getPosition(cliff->window).x <= cliff->gameRes.x)
                    createBox();
                else
                {
                    if(collided(builderAssistantT))
                    {
                        if(builderAssistant)
                        {
                            builderAssistant = false;
                            builderAssistantT.setColor(sf::Color::White);
                        }
                        else
                        {
                            builderAssistant = true;
                            builderAssistantT.setColor(sf::Color::Green);
                        }
                    }
                    if(collided(gridT))
                    {
                        if(grid)
                        {
                            grid = false;
                            gridT.setColor(sf::Color::White);
                        }
                        else
                        {
                            grid = true;
                            gridT.setColor(sf::Color::Green);
                        }
                    }
                }
            }
            if(event.key.code == sf::Mouse::Right)
                deleteBox();
        break;
        case sf::Event::KeyReleased:
            if(event.key.code == sf::Keyboard::Escape)
            {
                saveMap();
                cliff->changeState(cliff->menu);
            }
        break;

        case sf::Event::TextEntered:
            if (event.text.unicode == '\b')
                mapName.erase(mapName.size() - 1, 1);
            else if (event.text.unicode < 128)
            {
                mapName += static_cast<char>(event.text.unicode);
            }
        }



    }
}


bool EditorState::collided(sf::Sprite obj1, sf::RectangleShape obj2)
{
            //fireball position  <= box position right side
    if(obj2.getPosition().x - obj2.getLocalBounds().width/2 + 1 < obj1.getPosition().x
    //fireball position  >= box position left side
    && obj2.getPosition().x + obj2.getLocalBounds().width/2 - 1 > obj1.getPosition().x
    //fireball position  <= box position bot side
    && obj2.getPosition().y - obj2.getLocalBounds().height/2 + 1 < obj1.getPosition().y
    //fireball position  >= box position top side
    && obj2.getPosition().y + obj2.getLocalBounds().height/2 - 1 > obj1.getPosition().y )
    {
        return true;
    }

}

bool EditorState::collided(sf::Text obj2)
{
    if(obj2.getPosition().y - obj2.getLocalBounds().height/2 < sf::Mouse::getPosition(cliff->window).y
    && obj2.getPosition().y + obj2.getLocalBounds().height/2 > sf::Mouse::getPosition(cliff->window).y)
        return true;
}
void EditorState::saveMap()
{
    for(int i = numberOfBoxes; i >= 0; i--)
    {
        //saving coordinates with scaling factor 1
        boxPosSS << boxS[i].getPosition().x * (1/ cliff->scalingFactor.x);
        boxPosSS << " ";
        boxPosSS << boxS[i].getPosition().y * (1/ cliff->scalingFactor.y);
        boxPosSS << "\n";
    }
    mapName.erase(mapName.size()-1, 2);

    mapName += ".boxMap";
    std::ofstream boxPos(mapName);
    boxPos << boxPosSS.str();
    std::cout << "boxPoss: " << boxPosSS.str() << std::endl;

}

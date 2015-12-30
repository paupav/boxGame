#include "MenuState.hpp"
#include "Cliff.hpp"
#include <iostream>

MenuState::MenuState(sf::RenderWindow &windoww, Cliff &clifff)
{
    window = &windoww;
    cliff = &clifff;
    font.loadFromFile("menu.ttf");
    gameT.setFont(font);
    editorT.setFont(font);
    settingsT.setFont(font);
    exitT.setFont(font);
    selectorT.setFont(font);

    charSize = 100 * cliff->scalingFactor.x;
    gameT.setCharacterSize(charSize);
    editorT.setCharacterSize(charSize);
    settingsT.setCharacterSize(charSize);
    exitT.setCharacterSize(charSize);
    selectorT.setCharacterSize(charSize);

    gameT.setString("Start  game");
    editorT.setString("Level editor");
    settingsT.setString("Settings");
    exitT.setString("Exit");
    selectorT.setString("->                                <-");

    gameT.setOrigin(gameT.getLocalBounds().width/2, gameT.getLocalBounds().height/2);
    editorT.setOrigin(editorT.getLocalBounds().width/2, editorT.getLocalBounds().height/2);
    settingsT.setOrigin(settingsT.getLocalBounds().width/2, settingsT.getLocalBounds().height/2);
    exitT.setOrigin(exitT.getLocalBounds().width/2, exitT.getLocalBounds().height/2);
    selectorT.setOrigin(selectorT.getLocalBounds().width/2, selectorT.getLocalBounds().height/2);

    gameT.setPosition(cliff->res.x/2, cliff->res.y/2 - game * charSize);
    editorT.setPosition(cliff->res.x/2, cliff->res.y/2 - editor * charSize);
    settingsT.setPosition(cliff->res.x/2 , cliff->res.y/2 - settings * charSize);
    exitT.setPosition(cliff->res.x/2, cliff->res.y/2 - exit * charSize);
}

void MenuState::run()
{
    if(selector > game)
        selector = exit;
    if(selector < exit)
        selector = game;

    selectorT.setPosition(cliff->res.x/2, cliff->res.y/2  - selector * charSize);
    while(window->pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::KeyReleased:
                if(event.key.code == sf::Keyboard::S)
                    selector--;
                if(event.key.code == sf::Keyboard::W)
                    selector++;
                if(event.key.code == sf::Keyboard::Return)
                    if(selector == game)
                        cliff->changeState(cliff->game);
                    else if(selector == editor)
                        cliff->changeState(cliff->editor);
                    else if(selector == exit)
                        cliff->changeState(cliff->close);
            break;
        }
    }
    window->clear(sf::Color::Black);
    window->draw(gameT);
    window->draw(editorT);
    window->draw(settingsT);
    window->draw(exitT);
    window->draw(selectorT);
    window->display();
}


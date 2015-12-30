#pragma once
#include <SFML/Graphics.hpp>
#include "Cliff.hpp"
class Cliff;
class MenuState
{
public:
    MenuState(sf::RenderWindow &windoww, Cliff &clifff);
    void run();
private:
    int charSize;
    enum selections {exit, settings, editor, game};
    sf::Font font;
    sf::Text gameT, editorT, settingsT, exitT, selectorT;
    sf::RenderWindow *window;
    sf::Event event;
    Cliff *cliff;

    int selector = game;
};

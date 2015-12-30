#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include <fstream>
#include "Cliff.hpp"

class EditorState
{
public:
    EditorState(sf::RenderWindow &windoww, Cliff &clifff);
    void run();

private:
    void createBox();
    void deleteBox();
    void handleEvents();
    void saveMap();
    sf::Event event;
    bool grid;
    bool builderAssistant;
    bool collided(sf::Sprite obj1, sf::RectangleShape obj2);
    bool collided(sf::Text obj2);
    std::vector<int> delBoxes;
    std::string mapName;
    std::ostringstream boxPosSS;

    int vRow, hRow;
    sf::Font font;
    sf::Text gridT, builderAssistantT, mapNameT;

    sf::RenderWindow *window;
    Cliff *cliff;
    sf::Texture boxT;
    sf::Sprite boxS[1000];
    sf::RectangleShape placeHolder, gridV[25], gridH[23];
    int numberOfBoxes = 0;

};

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Cliff.hpp"
#include <stack>
#include <string>
#include "InitState.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"
#include "EditorState.hpp"
#define BOXCONST 0.15
#define RAD 57.29

class Cliff
{
public:
    Cliff(Cliff &clifff);
    void run();

    void changeState(int requestedState);
    void createWindow();
    bool collided(sf::Sprite obj1, sf::Sprite obj2);

    sf::Vector2i res;
    sf::Vector2i gameRes;
    sf::Vector2f scalingFactor;

    enum lStates {close, init, menu, game, pause, editor};

    sf::RenderWindow window;

private:
    sf::ContextSettings settings;
    sf::Image icon;
    Cliff *cliff;

    std::vector<int> States;


};


#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class Rope {
public:
    Rope(Player player1, Player player2);
    void draw(sf::RenderWindow &window);
    void update(Player player_1, Player player_2);

public:
    sf::Vector2f start;
    sf::Vector2f end;
private:
    sf::Vertex body[2];
};
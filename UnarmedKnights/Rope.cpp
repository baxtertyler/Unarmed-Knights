#include "Rope.h"

Rope::Rope(Player player_1, Player player_2)
{
    this->start = player_1.getPosition();
    this->end = player_2.getPosition();
    this->body[0] = sf::Vertex(start);
    this->body[1] = sf::Vertex(end);
}

void Rope::draw(sf::RenderWindow& window)
{
    window.draw(body, 2, sf::Lines);
}

void Rope::update(Player player_1, Player player_2)
{
    start = player_1.getPosition();
    end = player_2.getPosition();
    this->body[0] = sf::Vertex(start);
    this->body[1] = sf::Vertex(end);
}
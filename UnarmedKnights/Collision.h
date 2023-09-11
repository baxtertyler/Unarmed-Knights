#pragma once
#include <SFML/Graphics.hpp>

class Collision {
public:
    Collision(sf::RectangleShape& body);
    bool checkCollision(Collision other, float push);
    void move(float dx, float dy) {body.move(dx, dy);}
    sf::Vector2f getPosition() {return body.getPosition();}
    sf::Vector2f getHalfsize() {return body.getSize() / 2.0f;}
private:
    sf::RectangleShape& body;
};

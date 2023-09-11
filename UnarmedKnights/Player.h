#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Collision.h"

class Player{
public:
    Player(sf::Texture* texture, sf::Vector2u image_count, sf::Vector2f position,
           float switch_time, float speed, int who);
    void update(float delta_time, int rope_length, sf::Vector2f view_center);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() {return body.getPosition();}
    Collision getCollision() {return Collision(body);}

private:
    sf::RectangleShape body;
    Animation animation;
    unsigned int row;
    float speed;
    bool face_right;
    int who;
};
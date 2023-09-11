#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Collision.h"
#include "Player.h"
#include "Rope.h"

class Enemy {
public:
    Enemy(sf::Texture* texture, sf::Vector2u image_count, sf::Vector2f start_pos,
          float switch_time, float speed, sf::Vector2f target, int who);
    void update(float delta_time, sf::Vector2f new_target);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() {return body.getPosition();}
    Collision getCollision() {return Collision(body);}
    bool checkSlice(Rope rope);
    bool checkHit(sf::Vector2f player1, sf::Vector2f player2);
private:
    sf::RectangleShape body;
    sf::Vector2f target;
    float speed;
    bool face_right;
public:
    Animation animation;
    std::string state = "none";
    int who;
    unsigned int row;
    sf::Clock time_dead;
};
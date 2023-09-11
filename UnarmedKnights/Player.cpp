#include "Player.h"
#include <iostream>

Player::Player(sf::Texture* texture, sf::Vector2u image_count, sf::Vector2f start_pos,
               float switch_time, float speed, int who) :
        animation(texture, image_count, switch_time)
{
    this->speed = speed;
    row = 0;
    face_right = true;
    this->who = who;
    body.setSize(sf::Vector2f(80.0f, 90.0f));
    body.setOrigin(body.getSize() / 2.0f);
    body.setPosition(start_pos);
    body.setTexture(texture);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(body);
}

void Player::update(float delta_time, int rope_length, sf::Vector2f view_center) {
    float dx = getPosition().x - view_center.x;
    float dy = getPosition().y - view_center.y;
    float dz = sqrt(pow(dx, 2) + pow(dy, 2));
    if (dz > (rope_length/2.0f) + 25)
    {
        sf::Vector2f movement(-1 * dx/dz, -1 * dy/dz);
        animation.update(row, delta_time, face_right);
        body.setTextureRect(animation.uv_rect);
        body.move(movement);
        return;
    }
    if (dz < (rope_length/2.0f) - 25)
    {
        sf::Vector2f movement(dx/dz,  dy/dz);
        animation.update(row, delta_time, face_right);
        body.setTextureRect(animation.uv_rect);
        body.move(movement);
        return;
    }

    sf::Vector2f movement(0.0f, 0.0f);
    if (who == 1)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            movement.x -= speed * delta_time;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            movement.x += speed * delta_time;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            movement.y -= speed * delta_time;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            movement.y += speed * delta_time;
    }
    if (who == 2)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            movement.x -= speed * delta_time;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            movement.x += speed * delta_time;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            movement.y -= speed * delta_time;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            movement.y += speed * delta_time;
    }

    if (movement.x == 0) {
        if (movement.y < 0)
            row = 3;
        else
            row = 0;
    }
    else
    {
        row = 2;
        if (movement.x > 0.0f)
            face_right = true;
        else
            face_right = false;
    }
    animation.update(row, delta_time, face_right);
    body.setTextureRect(animation.uv_rect);
    body.move(movement);
}
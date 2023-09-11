#include "Enemy.h"
#include <iostream>
#include <cmath>

Enemy::Enemy(sf::Texture* texture, sf::Vector2u image_count, sf::Vector2f start_pos,
             float switch_time, float speed, sf::Vector2f target, int who):
        animation(texture, image_count, switch_time)
{
    this->speed = speed;
    row = 0;
    this->who = who;
    this->target = target;
    face_right = true;
    body.setSize(sf::Vector2f(80.0f, 80.0f));
    body.setOrigin(body.getSize() / 2.0f);
    body.setPosition(start_pos);
    body.setTexture(texture);
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(body);
}

bool Enemy::checkHit(sf::Vector2f player1, sf::Vector2f player2)
{
    sf::Vector2f enemy_p1 = getPosition() - player1;
    sf::Vector2f enemy_p2 = getPosition() - player2;
    float d1 = sqrt(pow(enemy_p1.x, 2) + pow(enemy_p1.y, 2));
    float d2 = sqrt(pow(enemy_p2.x, 2) + pow(enemy_p2.y, 2));
    if (d1 < 40 or d2 < 40)
        return true;
    return false;
}

void Enemy::update(float delta_time, sf::Vector2f new_target) {
    sf::Vector2f movement(0.0f, 0.0f);
    if (state == "none")
    {
        float x_change = new_target.x - this->getPosition().x;
        float y_change = new_target.y - this->getPosition().y;
        float z_change = sqrt(pow(x_change, 2) + pow(y_change, 2));

        movement.x += speed * delta_time * x_change/z_change;
        movement.y += speed * delta_time * y_change/z_change;
    }
    row = 0;
    animation.image_count.x = 5;
    if (state == "attack")
    {
        animation.image_count.x = 5;
        body.setSize(body.getSize() - sf::Vector2f(0.05f, 0.05f));
        if (body.getSize().x <= 0)
        {
            state = "gone";
        }
        body.setOrigin(body.getOrigin() - sf::Vector2f(0.025f, 0.025f));
        row = 1;
    }
    if (state == "dead")
    {
        animation.image_count.x = 8;
        row = 2;
        if (animation.current_image.x == 7)
        {
            state = "gone";
        }
    }
    if (movement.x > 0)
    {
        face_right = true;
    } else if (movement.x < 0){
        face_right = false;
    }

    animation.update(row, delta_time, face_right);
    body.setTextureRect(animation.uv_rect);
    body.move(movement);
}

bool Enemy::checkSlice(Rope rope) {
    sf::Vector2f i = rope.start;
    float dx = rope.end.x - rope.start.x;
    float dy = rope.end.y - rope.start.y;
    float dz = sqrt(pow(dx, 2) + pow(dy, 2));
    if (i.x <= rope.end.x and i.y <= rope.end.y)
    {
        while (i.x <= rope.end.x and i.y <= rope.end.y)
        {
            if(getPosition().x + 1 > i.x && getPosition().x - 1 < i.x &&
               getPosition().y + 1 > i.y && getPosition().y - 1 < i.y) {
                state = "dead";
                return true;
            }
            else
            {
                i.x += 2 * dx/dz;
                i.y += 2 * dy/dz;
            }
        }
    }
    else if (i.x <= rope.end.x and i.y > rope.end.y)
    {
        while (i.x <= rope.end.x and i.y > rope.end.y)
        {
            if(getPosition().x + 1 > i.x && getPosition().x - 1 < i.x &&
               getPosition().y + 1 > i.y && getPosition().y - 1 < i.y) {
                state = "dead";
                return true;
            }
            else
            {
                i.x += 2 * dx/dz;
                i.y += 2 * dy/dz;
            }
        }
    }
    else if (i.x > rope.end.x and i.y <= rope.end.y)
    {
        while (i.x > rope.end.x and i.y <= rope.end.y)
        {
            if(getPosition().x + 1 > i.x && getPosition().x - 1 < i.x &&
               getPosition().y + 1 > i.y && getPosition().y - 1 < i.y) {
                state = "dead";
                return true;
            }
            else
            {
                i.x += 2 * dx/dz;
                i.y += 2 * dy/dz;
            }
        }
    }
    else if (i.x > rope.end.x and i.y > rope.end.y)
    {
        while (i.x > rope.end.x and i.y > rope.end.y)
        {
            if(getPosition().x + 1 > i.x && getPosition().x - 1 < i.x &&
               getPosition().y + 1 > i.y && getPosition().y - 1 < i.y) {
                state = "dead";
                return true;
            }
            else
            {
                i.x += 2 * dx/dz;
                i.y += 2 * dy/dz;
            }
        }
    }
    return false;
}
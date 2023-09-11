#pragma once
#include <SFML/Graphics.hpp>

class Animation{
public:
    Animation(sf::Texture* texture, sf::Vector2u image_count, float switch_time);
    void update(int row, float delta_time, bool face_right);
public:
    sf::IntRect uv_rect;
    sf::Vector2u image_count;
private:
    float total_time;
public:
    float switch_time;
    sf::Vector2u current_image;
};
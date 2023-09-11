#include "Collision.h"

Collision::Collision(sf::RectangleShape& body):
        body(body)
{

}

bool Collision::checkCollision(Collision other, float push) {
    sf::Vector2f other_pos = other.getPosition();
    sf::Vector2f other_half_size = other.getHalfsize();
    sf::Vector2f this_pos = getPosition();
    sf::Vector2f this_half_size = getHalfsize();

    float delta_x = other_pos.x - this_pos.x;
    float delta_y = other_pos.y - this_pos.y;

    float intersect_x = abs(delta_x) - (other_half_size.x + this_half_size.x);
    float intersect_y = abs(delta_y) - (other_half_size.y + this_half_size.y);

    if (intersect_x < 0.0f && intersect_y < 0.0f)
    {
        push = std::min(std::max(push, 0.0f), 1.0f);

        if(intersect_x > intersect_y)
        {
            if (delta_x > 0.0f)
            {
                move(intersect_x * (1.0f-push), 0.0f);
                other.move(-intersect_x * push, 0.0f);
            }
            else
            {
                move(-intersect_x * (1.0f-push), 0.0f);
                other.move(intersect_x * push, 0.0f);
            }
        }
        else
        {
            if (delta_y > 0.0f)
            {
                move(0.0f, intersect_y * (1.0f-push));
                other.move(0.0f,-intersect_y * push);
            }
            else
            {
                move(0.0f, -intersect_y * (1.0f-push));
                other.move(0.0f, intersect_y * push);
            }
        }
        return true;
    }
    return false;
}
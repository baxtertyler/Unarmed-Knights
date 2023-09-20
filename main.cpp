#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <random>
#include "Player.h"
#include "Platform.h"
#include "Rope.h"
#include "Enemy.h"

static const float VIEW_HEIGHT = 700.0f;
static const float VIEW_WIDTH = 1000.0f;

int main()
{
    // window and window view
    sf::RenderWindow window(
            sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT),
            "UNARMED KNIGHTS",
            sf::Style::Close | sf::Style::Titlebar);
    sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT));

    // text on screen
    sf::Text text;
    sf::Font font;
    font.loadFromFile("../Textures/font.ttf");
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(30);

    // textures
    sf::Texture player_texture_1;
    sf::Texture player_texture_2;
    sf::Texture enemy_texture;
    sf::Texture bg_texture;
    sf::Texture wall_texture;
    sf::Texture rock_texture;
    sf::Texture scoreboard_texture;
    sf::Texture sword_texture;
    sf::Texture heart_texture;
    sf::Texture start_screen_texture;
    player_texture_1.loadFromFile("../Textures/knight1.png");
    player_texture_2.loadFromFile("../Textures/knight2.png");
    enemy_texture.loadFromFile("../Textures/monster.png");
    bg_texture.loadFromFile("../Textures/bg.png");
    wall_texture.loadFromFile("../Textures/wall.png");
    rock_texture.loadFromFile("../Textures/rock.png");
    scoreboard_texture.loadFromFile("../Textures/scoreboard.png");
    sword_texture.loadFromFile("../Textures/sword.png");
    heart_texture.loadFromFile("../Textures/heart.png");

    // gameplay variables
    int rope_length = 300;
    int num_enemies = 1;
    int enemies_killed = 0;
    int score = 0;
    int lives = 10;
    int area_dist_from_center = 2000;
    int time_for_extra_enemy = 30;
    int enemies_for_multiplier = 8;
    int enemies_killed_save = enemies_killed;
    int num_obstacles = 50;
    bool ss = true;
    bool over_start = false;
    bool end_screen = false;
    std::default_random_engine randomizer;

    // sprites
    Player player_1(&player_texture_1,
                    sf::Vector2u(3.0f, 4.0f),sf::Vector2f(-100.0f, 0.0f),
                    0.3f,100.0f,1);
    Player player_2(&player_texture_2,
                    sf::Vector2u(3.0f, 4.0f),sf::Vector2f(100.0f, 0.0f),
                    0.3f,100.0f,2);
    Rope rope(player_1, player_2);
    sf::Sprite scoreboard;
    scoreboard.setTexture(scoreboard_texture);
    sf::Sprite sword;
    sword.setOrigin(0,0);
    sword.setTexture(sword_texture);

    // sprite vectors
    std::vector<Enemy> enemies;
    std::vector<Platform> walls;
    for (int i = -area_dist_from_center; i < area_dist_from_center+130; i += 100) {
        walls.emplace_back(&wall_texture,
        sf::Vector2f(100.0f, 100.0f), sf::Vector2f(i, area_dist_from_center+100));
        walls.emplace_back(&wall_texture,
                           sf::Vector2f(100.0f, 100.0f), sf::Vector2f(i, -area_dist_from_center));
    }
    for (int i = -area_dist_from_center; i < area_dist_from_center+130; i += 100) {
        walls.emplace_back(&wall_texture,
                           sf::Vector2f(100.0f, 100.0f), sf::Vector2f(-area_dist_from_center, i));
        walls.emplace_back(&wall_texture,
                           sf::Vector2f(100.0f, 100.0f), sf::Vector2f(area_dist_from_center+100, i));
    }
    std::vector<Platform> rocks;
    for (int i = 0; i < num_obstacles; i++) {
        float rock_size_1 = rand() % 40 + 80;
        float rock_size_2 = rand() % 20 + 40;
        float rock_x = (rand() % (2*area_dist_from_center-200)) - area_dist_from_center + 100;
        float rock_y = (rand() % (2*area_dist_from_center-200)) - area_dist_from_center + 100;
        float wall_size = rand() % 20 + 90;
        float wall_x = (rand() % (2*area_dist_from_center-200)) - area_dist_from_center + 100;
        float wall_y = (rand() % (2*area_dist_from_center-200)) - area_dist_from_center + 100;
        rocks.emplace_back(&rock_texture,
                           sf::Vector2f(rock_size_1, rock_size_2), sf::Vector2f(rock_x, rock_y));
        walls.emplace_back(&wall_texture,
                           sf::Vector2f(wall_size, wall_size), sf::Vector2f(wall_x, wall_y));
    }
    std::vector<sf::Sprite> bgs;
    for (int i = -area_dist_from_center; i < area_dist_from_center; i += 512){
        for (int j = -area_dist_from_center; j < area_dist_from_center; j += 512){
            sf::Sprite bg;
            bg.setTexture(bg_texture);
            bg.setPosition(i, j);
            bgs.emplace_back(bg);
        }
    }
    std::vector<sf::Sprite> hearts;
    for (int i = 0; i < lives; i++)
    {
        sf::Sprite heart;
        heart.setTexture(heart_texture);
        heart.setScale(sf::Vector2f(0.1, 0.1));
        hearts.emplace_back(heart);
    }

    // time variables
    float delta_time;
    float multiplier = 1.0f;
    sf::Clock clock;
    sf::Clock new_enemy_clock;
    sf::Clock score_clock;

    // main loop
    while(window.isOpen() and lives > 0)
    {
        while(ss)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (mousePos.x > 383 and mousePos.x < 608 and mousePos.y > 480 and mousePos.y < 540) {
                over_start = true;
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    ss = false;
                    break;
                }
            } else
                over_start = false;
            if(over_start)
                start_screen_texture.loadFromFile("../Textures/startyes.png");
            else
                start_screen_texture.loadFromFile("../Textures/startno.png");
            sf::Sprite start_screen;
            start_screen.setTexture(start_screen_texture);
            window.draw(start_screen);

            sf::Event game_event;
            while (window.pollEvent(game_event))
            {
                if (game_event.type == sf::Event::Closed)
                {
                    ss = false;
                    window.close();
                }
            }

            window.display();
        }

        while (enemies.size() < num_enemies and not ss) {
            sf::Vector2f displacement;
            int wall = randomizer() % 4;
            if (wall == 0) {
                displacement.x = (randomizer() % (int)(VIEW_WIDTH)) - VIEW_WIDTH/2;
                displacement.y = (int)(VIEW_HEIGHT / 2);
            } else if (wall == 1) {
                displacement.x = (randomizer() % (int)(VIEW_WIDTH)) - VIEW_WIDTH/2;
                displacement.y = -(int)(VIEW_HEIGHT / 2);
            } else if (wall == 2) {
                displacement.x = (int)(VIEW_WIDTH / 2);
                displacement.y = (randomizer() % (int)(VIEW_HEIGHT)) - VIEW_HEIGHT/2;
            } else {
                displacement.x = -(int)(VIEW_WIDTH / 2);
                displacement.y = (randomizer() % (int)(VIEW_HEIGHT)) - VIEW_HEIGHT/2;
            }
            if (wall <= 1)
            {
                enemies.emplace_back(&enemy_texture, sf::Vector2u(8.0f, 3.0f),
                                     view.getCenter() + displacement,
                                     0.1f, 80.0f, player_1.getPosition(), 1);
            } else {
                enemies.emplace_back(&enemy_texture, sf::Vector2u(8.0f, 3.0f),
                                     view.getCenter() + displacement,
                                     0.1f, 80.0f, player_2.getPosition(), 2);
            }
        }

        if (enemies_killed - enemies_for_multiplier > enemies_killed_save)
        {
            multiplier += 0.25;
            enemies_killed_save = enemies_killed;
        }
        delta_time = clock.restart().asSeconds() * multiplier;

        if (score_clock.getElapsedTime().asSeconds() > 1)
        {
            score_clock.restart();
            score += 1;
        }

        // game events
        sf::Event game_event;
        while (window.pollEvent(game_event))
        {
            if (game_event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }

        // update screen
        player_1.update(delta_time, rope_length, view.getCenter());
        player_2.update(delta_time, rope_length, view.getCenter());
        for (Enemy& enemy : enemies)
        {
            if (enemy.who == 1) {
                enemy.update(delta_time, player_1.getPosition());
            } else {
                enemy.update(delta_time, player_2.getPosition());
            }
        }
        if (new_enemy_clock.getElapsedTime().asSeconds() > time_for_extra_enemy) {
            num_enemies += 1;
            new_enemy_clock.restart();
        }
        rope.update(player_1, player_2);
        view.setCenter((player_1.getPosition() + player_2.getPosition()) / 2.0f);
        window.setView(view);
        window.clear(sf::Color(200, 200, 200));

        // collisions
        player_1.getCollision().checkCollision(player_2.getCollision(), 0.5f);
        for (Platform& wall : walls)
        {
            wall.getCollision().checkCollision(player_1.getCollision(), 1.0f);
            wall.getCollision().checkCollision(player_2.getCollision(), 1.0f);
        }
        for (Platform& rock : rocks)
        {
            rock.getCollision().checkCollision(player_1.getCollision(), 0.5f);
            rock.getCollision().checkCollision(player_2.getCollision(), 0.5f);
        }
        for (Enemy& enemy : enemies) {
            if (enemy.state == "none" and enemy.checkHit(player_1.getPosition(), player_2.getPosition())) {
                enemy.state = "attack";
                lives -= 1;
            }
            if (enemy.state == "none" and enemy.checkSlice(rope)) {
                enemy.state = "dead";
                enemy.animation.current_image.x = 0;
                score += 25;
                enemies_killed += 1;
            }
        }
        for (int i = 0; i < enemies.size(); i++)
        {
            if (enemies[i].state == "gone")
            {
                enemies.erase(enemies.begin() + i);
                i -= 1;
            }
        }

        // draw sprites
        for (sf::Sprite bg : bgs){
            window.draw(bg);
        }
        for (Enemy& enemy : enemies)
        {
            if (enemy.state == "hit" || enemy.state == "dead")
                enemy.draw(window);
        }
        rope.draw(window);
        player_1.draw(window);
        player_2.draw(window);
        for (Platform& wall : walls)
        {
            wall.draw(window);
        }
        for (Platform& rock : rocks)
        {
            rock.draw(window);
        }
        for (Enemy& enemy : enemies)
        {
            if (enemy.state != "hit" and enemy.state != "dead")
                enemy.draw(window);
        }
        scoreboard.setPosition(view.getCenter() - sf::Vector2f(VIEW_WIDTH/2, VIEW_HEIGHT/2));
        window.draw(scoreboard);
        text.setString(std::to_string(score));
        text.setPosition(view.getCenter() - sf::Vector2f(340, 345));
        window.draw(text);
        text.setString(std::to_string(multiplier));
        text.setPosition(view.getCenter() - sf::Vector2f(-340, 315));
        window.draw(text);
        text.setString(std::to_string(num_enemies));
        text.setPosition(view.getCenter() - sf::Vector2f(-340, 345));
        window.draw(text);
        for (int i = 0; i < lives; i++)
        {
            hearts[i].setPosition(view.getCenter() + sf::Vector2f(-340 + (i*33), -305));
            window.draw(hearts[i]);
        }

        // FINAL SCREEN UPDATE
        window.display();

    }

    end_screen = true;
    clock.restart();
    while (end_screen)
    {
        sf::Event game_event;
        while (window.pollEvent(game_event))
        {
            if (game_event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }
        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(30);
        window.draw(scoreboard);
        window.draw(scoreboard);
        text.setString(std::to_string(score));
        text.setPosition(view.getCenter() - sf::Vector2f(340, 345));
        window.draw(text);
        text.setString(std::to_string(multiplier));
        text.setPosition(view.getCenter() - sf::Vector2f(-340, 315));
        window.draw(text);
        text.setString(std::to_string(num_enemies));
        text.setPosition(view.getCenter() - sf::Vector2f(-340, 345));
        window.draw(text);

        text.setFillColor(sf::Color::Red);
        text.setCharacterSize(50);
        text.setString("GAME OVER");
        text.setPosition(view.getCenter() - sf::Vector2f(150, 0));
        window.draw(text);
        window.display();
        if (clock.getElapsedTime().asSeconds() > 5){
            end_screen = false;
            ss = true;
            lives = 10;
            score = 0;
            num_enemies = 1;
            multiplier = 1;
        }
    }
    window.close();
}
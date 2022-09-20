#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H
#include <iostream>
#include <ncurses.h>
#include <vector>
#include <unistd.h>

struct SnakeType {
    int s_x, s_y;
    SnakeType(int, int);
    SnakeType();
};

class SnakeGame {
   
protected: 
    int m_max_width;
    int m_max_height;
    std::vector<SnakeType> snake;
    char m_snake_char;
    SnakeType v_food;
    char m_food_char;
    int m_delay; 
    char m_direction;
    bool m_tail_stop;
    int m_score;

public:
    SnakeGame();
    ~SnakeGame();

    void m_insert_food();

    void move_snake();
    bool _collide();
    void start();

};

#endif
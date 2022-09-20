#include <iostream>
#include <ncurses.h>
#include <vector>
#include <unistd.h>

//g++ main.cpp -lncurses -ltinfo

struct SnakeType {
    int s_x, s_y;

    SnakeType(int s_column, int s_row) {
        s_x = s_column; 
        s_y = s_row;
    }

    SnakeType() {
        s_x = 0; 
        s_y = 0;
    }
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
    SnakeGame(){
        initscr();
        nodelay(stdscr, true);
        keypad(stdscr, true);
        noecho();
        curs_set(0);

        m_max_width = getmaxx(stdscr);
        m_max_height = getmaxy(stdscr);

        m_snake_char = 'C';
        m_food_char = 'o';

        srand(time(NULL));
        m_insert_food();

        m_delay = 100000;
        m_direction = 'L';
        m_tail_stop = false;
        m_score = 0;

        for (int i = 0; i < m_max_width - 1; i++) {
            move(0, i);
            if (i == 0) {
                addch('+');
            } else if(i == (m_max_width - 2)) {
                addch('+');
            } else {
                addch('-');
            }
        }

         for (int i = 1; i < m_max_height - 1; i++) {
            move(i, 0);
            if (i == (m_max_height - 2)) {
                addch('+');
            } else {
                addch('|');
            }
        }

         for (int i = 0; i < m_max_width - 1; i++) {
            move(m_max_height - 2, i);
            if (i == 0) {
                addch('+');
            } else if(i == (m_max_width - 2)) {
                addch('+');
            } else {
                addch('-');
            }
        }


         for (int i = 1; i < m_max_height - 1; i++) {
            move(i, m_max_width - 2);
            if (i == (m_max_height - 2)) {
                addch('+');
            } else if (i == 0) {
                addch('+');
            } else {
                addch('|');
            }
        }

        for (int i = 0; i < 5; i++) {
            snake.push_back(SnakeType(40 + i, 10));
        }

        for (int i = 0; i < snake.size(); i++) {
            move(snake[i].s_y, snake[i].s_x);
            addch(m_snake_char);
        }

        move(m_max_height - 1, 0); 
        printw("Score: %d", m_score); 

        move(v_food.s_y, v_food.s_x);
        addch(m_food_char);
    }

    ~SnakeGame(){
        nodelay(stdscr, false);
        getch();
        endwin();
    }

    void m_insert_food(){
        while(true) {
            int tmpx = rand() % m_max_width + 1;
            int tmpy = rand() % m_max_height + 1;

            for (int i = 0; i < snake.size(); i++) {

                if (snake[i].s_x == tmpx && snake[i].s_y == tmpy) {
                    continue;
                }
            }

            if (tmpx >= m_max_width -2 || tmpy >= m_max_height - 3) {
                continue;
            }

            v_food.s_x = tmpx; 
            v_food.s_y = tmpy;
            break;
        }
            move(v_food.s_y, v_food.s_x);
            addch(m_food_char);
            refresh();
    }

    void move_snake(){
        int tmp = getch();
  
        switch (tmp){
        case KEY_LEFT: 
            if(m_direction != 'R') {
                m_direction = 'L';
            }
            break;

        case KEY_UP:
            if(m_direction != 'D') {
                m_direction = 'U';
            }
            break;

        case KEY_DOWN: 
            if(m_direction != 'U') {
                m_direction = 'D';
            }
            break;

        case KEY_RIGHT: 
            if(m_direction != 'L') {
                m_direction = 'R';
            }
            break;

        case 'q': 
            m_direction = 'Q';
            break;
        }

        if (!m_tail_stop) {
            move(snake[snake.size() - 1].s_y, snake[snake.size() - 1].s_x);
            printw(" ");
            refresh();
            snake.pop_back();
        }

        if (m_direction == 'L') {
            snake.insert(snake.begin(), SnakeType(snake[0].s_x - 1, snake[0].s_y));
        }

        if (m_direction == 'R') {
            snake.insert(snake.begin(), SnakeType(snake[0].s_x + 1, snake[0].s_y));
        }

        if (m_direction == 'U') {
            snake.insert(snake.begin(), SnakeType(snake[0].s_x, snake[0].s_y - 1));
        }

        if (m_direction == 'D') {
            snake.insert(snake.begin(), SnakeType(snake[0].s_x, snake[0].s_y + 1));
        }

        move(snake[0].s_y, snake[0].s_x);
        addch(m_snake_char);
        refresh();
    }

    bool _collide() {
        if(snake[0].s_x == 0 || snake[0].s_x == m_max_width - 1 || 
        snake[0].s_y == 0 || snake[0].s_y == m_max_height - 2) {
            return true;
        }

        for (int i = 2; i < snake.size(); i++) {
            if (snake[0].s_x == snake[i].s_x && snake[0].s_y == snake[i].s_y) {
                return true;
            }
        }
        
        if (snake[0].s_x == v_food.s_x && snake[0].s_y == v_food.s_y) {
            m_tail_stop = true; 
            m_insert_food(); 
            m_score +=10;
            move(m_max_height - 1, 0);
            printw("Score: %d", m_score);

            if ((m_score % 50) == 0) {
                m_delay -= 10000;
            }

        } else {
            m_tail_stop = false;
        }

        return false;
    }

    void start() {
        while(true) {

            if(_collide()) {
                move(m_max_height / 2, (m_max_width / 2) - 4);
                printw("GAME OVER!");
                break;
            }

            move_snake(); 

            if (m_direction == 'Q') {
                break; 
            }

            usleep(m_delay);
        }
    }

};

int main(int argc, char const *argv[])
{
    SnakeGame snake;
    snake.start();
    return 0;
}


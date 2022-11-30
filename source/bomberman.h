#define SDL_MAIN_HANDLED
#include <SDL.h>

typedef struct vec2
{

    int x;
    int y;

} vec2_t;

typedef struct player
{
    vec2_t pos;
    unsigned int number_of_lifes;
    unsigned int number_of_bombs;
    unsigned int score;
    unsigned int speed;

} player_t;

typedef struct bomb
{
    vec2_t pos;
    unsigned int range;
    unsigned int cooldown;

} bomb_t;

enum cell_type
{
    GROUND = 0,
    WALL,
    UNDESTROYABLE_WALL,
    TELEPORT

};

enum bonus_type
{
    NONE = 0,
    SPEED,
    BOMB2,
    BOMB3

};

enum enemy_type
{
    BAT = 0,
    BALOON
};

typedef struct cell
{
    enum cell_type type;
    enum bonus_type bonus;
} cell_t;

typedef struct enemy
{
    vec2_t position;
    enum enemy_type c;
    
} enemy_t;

typedef struct game_mode
{
    unsigned int timer;
    
}game_mode_t;
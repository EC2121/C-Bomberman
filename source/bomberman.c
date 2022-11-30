#include "bomberman.h"



static void bomberman_game_mode_init(game_mode_t * game_mode)
{
    game_mode->timer = 60;
}

static void bomberman_map_init()
{
}

static void bomberman_player_init(player_t *player)
{

    player->pos.x = 0;
    player->pos.y = 0;
    player->number_of_lifes = 1;
    player->number_of_bombs = 1;
    player->score = 0;
    player->speed = 1;
}

int main(int argc, char **argv)
{

    game_mode_t game_mode;
    cell_t map[64 * 64];
    player_t player;

    bomberman_game_mode_init(&game_mode);
    
    bomberman_map_init();

    bomberman_player_init(&player);

    for (;;)
    {
    }
}
#include "bomberman.h"
#define STB_IMAGE_IMPLEMENTATION
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "stb_image.h"




double delta_time;
static void bomberman_game_mode_init(game_mode_t *game_mode)
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
    player->speed = 100;
}
static int bomberman_graphics_init(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    *window = SDL_CreateWindow("SDL is active!", 100, 100, 512, 512, 0);

    if (!*window)
    {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!*renderer)
    {
        SDL_Log("Unable to create renderer : %s", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return -1;
    }

    int width;
    int height;
    int channels;
    unsigned char *pixels = stbi_load("source/Textures/spr_zombie_idle.png", &width, &height, &channels, 4);

    if (!pixels)
    {
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        SDL_Log("Unable to open image");
        SDL_Quit();
        return -1;
    }

    SDL_Log("Image width : %d height:%d channels: %d", width, height, channels);
    *texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);
    if (!*texture)
    {
        free(pixels);
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        SDL_Log("Unable to create texture : %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    free(pixels);
    SDL_UpdateTexture(*texture, NULL, pixels, width * 4);

    return 0;
}

int CheckForJoysticks()
{
    SDL_GameController *controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i)
    {
        if (SDL_IsGameController(i))
        {
            controller = SDL_GameControllerOpen(i);
            if (controller)
            {
                SDL_Log("Found Controller %s", SDL_GameControllerName(controller));
                break;
            }
            else
            {
                SDL_Log("Could not open gamecontroller %d: %s\n", i, SDL_GetError());
                return -1;
            }
        }
    }
    return 0;
}
int main(int argc, char **argv)
{

    game_mode_t game_mode;
    cell_t map[64 * 64];
    player_t player;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    uint64_t timeLast;
    uint64_t timeNow = SDL_GetPerformanceCounter();
    if (bomberman_graphics_init(&window, &renderer, &texture))
    {
        return -1;
    }

    bomberman_game_mode_init(&game_mode);

    bomberman_map_init();

    bomberman_player_init(&player);

    int running = 1;
    int x = 100;
    while (running)
    {
        delta_time = ((double)(SDL_GetTicks() - timeLast) /1000);
        timeLast = SDL_GetTicks();
        printf("FPS : %f\n",delta_time);
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }
        SDL_PumpEvents();
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        player.pos.x += (keys[SDL_SCANCODE_D] * player.speed * delta_time);
        player.pos.x += -(keys[SDL_SCANCODE_A] * player.speed * delta_time);
        player.pos.y += (keys[SDL_SCANCODE_S] * player.speed * delta_time);
        player.pos.y += -(keys[SDL_SCANCODE_W] * player.speed * delta_time);



        SDL_JoyAxisEvent joyEvent;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Rect target_rect = {player.pos.x, player.pos.y, 128, 128};
        SDL_RenderCopy(renderer, texture, NULL, &target_rect);
        SDL_RenderPresent(renderer);
    }
    SDL_Quit();

    return 0;
}

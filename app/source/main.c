#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION
#include <SDL.h>

#include "bomberman.h"
#include "server.h"
#include "math.h"

int main(int argc, char **argv) {
    int socket = init_socket();
    game_mode_t game_mode;
    vec2_t map_size = {32, 32};
    player_t player;
    double delta_time;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    uint64_t timeLast;
    uint64_t timeNow = SDL_GetPerformanceCounter();
    if (bomberman_graphics_init(&window, &renderer, &texture)) {
        return -1;
    }

    bomberman_game_mode_init(&game_mode);

    bomberman_map_init(&renderer);

    bomberman_player_init(&player);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    int running = 1;
    int x = 100;
    char buffer[4096];
    while (running) {
        delta_time = ((double)(SDL_GetTicks() - timeLast) / 1000);
        timeLast = SDL_GetTicks();
        struct sockaddr_in sender_in;
        int sender_in_size = sizeof(sender_in);
        int len = recvfrom(socket, buffer, 4096, 0, (struct sockaddr *)&sender_in, &sender_in_size);
        if (len > 0) {
            char addr_as_string[64];
            inet_ntop(AF_INET, &sender_in.sin_addr, addr_as_string, 64);
            int *input_x = (int *)&buffer[0];
            int *input_y = (int *)&buffer[sizeof(int)];

            player.pos.x += (*input_x) * (player.speed * delta_time);
            player.pos.y += (*input_y) * (player.speed * delta_time);
            printf("%f , %f\n", player.pos.x, player.pos.y);
        }
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        SDL_PumpEvents();
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        // player.pos.x += (keys[SDL_SCANCODE_D] * player.speed * delta_time);
        // player.pos.x += -(keys[SDL_SCANCODE_A] * player.speed * delta_time);
        // player.pos.y += (keys[SDL_SCANCODE_S] * player.speed * delta_time);
        // player.pos.y += -(keys[SDL_SCANCODE_W] * player.speed * delta_time);
        
        SDL_RenderClear(renderer);
        draw_map(renderer, map_size);
        SDL_Rect target_rect = {player.pos.x, player.pos.y, 64, 64};
        SDL_RenderCopy(renderer, texture, NULL, &target_rect);
        SDL_RenderPresent(renderer);
    }
    SDL_Quit();

    return 0;
}
#include "bomberman.h"
double delta_time;
SDL_Texture *slate_square_texture;
SDL_Texture *grey_square_texture;
SDL_Rect map_rect = {0, 0, 64, 64};
void bomberman_game_mode_init(game_mode_t *game_mode) {
    game_mode->timer = 60;
}

int bomberman_map_init(SDL_Renderer **renderer) {
    int slate_square_width;
    int slate_square_height;
    int slate_square_channels;
    unsigned char *slate_square_pixels = stbi_load("app/resources/slate_64x64.png", &slate_square_width, &slate_square_height, &slate_square_channels, 4);
    if (!slate_square_pixels) {
        SDL_Log("Unable to open red square image");
        SDL_Quit();
        return -1;
    }
    slate_square_texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, slate_square_width, slate_square_height);
    SDL_UpdateTexture(slate_square_texture, NULL, slate_square_pixels, slate_square_width * 4);

    int grey_square_width;
    int grey_square_height;
    int grey_square_channels;
    unsigned char *grey_square_pixels = stbi_load("app/resources/grey_64x64.png", &grey_square_width, &grey_square_height, &grey_square_channels, 4);
    if (!grey_square_pixels) {
        SDL_Log("Unable to open green square image");
        SDL_Quit();
        return -1;
    }
    grey_square_texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, grey_square_width, grey_square_height);
    SDL_UpdateTexture(grey_square_texture, NULL, grey_square_pixels, grey_square_width * 4);

    return 0;
}

void draw_map(SDL_Renderer *renderer,vec2_t map_dimension) {
    int x = 0;
    int y = 0;
    SDL_Rect rect = {0, 0, 64, 64};
    for (size_t y = 0; y < map_dimension.y ; ++y) {
        for (size_t x = 0; x < map_dimension.x; x++) {
                SDL_RenderCopy(renderer, slate_square_texture, NULL, &rect);
            rect.x += 64;
        }
        rect.y += 64;
        rect.x = 0;
    }
}
void bomberman_player_init(player_t *player) {
    player->pos.x = 0;
    player->pos.y = 0;
    player->number_of_lifes = 1;
    player->number_of_bombs = 1;
    player->score = 0;
    player->speed = 100;
}
int bomberman_graphics_init(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    *window = SDL_CreateWindow("SDL is active!", 100, 100, 1280, 720, 0);

    if (!*window) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!*renderer) {
        SDL_Log("Unable to create renderer : %s", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return -1;
    }

    int width;
    int height;
    int channels;
    unsigned char *pixels = stbi_load("app/resources/spr_zombie_idle.png", &width, &height, &channels, 4);

    if (!pixels) {
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        SDL_Log("Unable to open image");
        SDL_Quit();
        return -1;
    }

    SDL_Log("Image width : %d height:%d channels: %d", width, height, channels);
    *texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);
    if (!*texture) {
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

int CheckForJoysticks() {
    SDL_GameController *controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (controller) {
                SDL_Log("Found Controller %s", SDL_GameControllerName(controller));
                break;
            } else {
                SDL_Log("Could not open gamecontroller %d: %s\n", i, SDL_GetError());
                return -1;
            }
        }
    }
    return 0;
}

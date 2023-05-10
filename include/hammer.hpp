#ifndef HAMMER_H
#define HAMMER_H

#include <vector>
#include <SDL.h>
#include "player.hpp"

class Hammer {
public:
    Hammer(int x_pos, int y_pos, std::vector<int> section_dim, SDL_Renderer* renderer);
    ~Hammer();

    void draw(SDL_Renderer* renderer, Player* player);
    void setUsed(bool value);
    void destroy();

private:
    int x_pos, y_pos;
    int width, height;
    bool used;
    SDL_Texture* image;
};

#endif /* HAMMER_H */
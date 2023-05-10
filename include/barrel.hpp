#ifndef BARREL_HPP
#define BARREL_HPP

#include <SDL.h>
#include <vector>
#include "ladder.hpp"
#include "level.hpp"
#include "game_object.hpp"

class Barrel : public GameObject {
public:
    Barrel(int x_pos, int y_pos, std::vector<int> section_dim, SDL_Renderer* renderer);
    void update(std::vector<Bridge> plats, int screen_height, GameLevel1 level);
    void check_fall(std::vector<Ladder> lads);
    void draw(SDL_Renderer* renderer);
    SDL_Rect getRect();
    void destroy();

private:
    int x_pos;
    int y_pos;
    std::vector<int> section_dim;
    SDL_Texture* barrel_img;
    SDL_Rect rect;
    float y_change;
    float x_change;
    int pos;
    int count;
    bool oil_collision;
    bool falling;
    bool check_lad;
    SDL_Rect bottom;
public:

    Barrel() = default;
};

#endif // BARREL_HPP
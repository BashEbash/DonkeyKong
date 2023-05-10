#ifndef LADDER_HPP
#define LADDER_HPP

#include <SDL.h>
#include <vector>

class Ladder {
public:
    Ladder(int x_pos, int y_pos, int length, std::vector<int> section_dim);
    void draw(SDL_Renderer* renderer);
    SDL_Rect getCollider() const;

private:
    int x_pos;
    int y_pos;
    int length;
    std::vector<int> section_dim;
};

#endif // LADDER_HPP
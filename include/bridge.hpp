#ifndef BRIDGE_H
#define BRIDGE_H

#include <vector>
#include <SDL.h>

class Bridge {
public:
    Bridge(int x_pos, int y_pos, int length, std::vector<int> section_dim);
    void draw(SDL_Renderer* renderer);
    SDL_Rect getCollider() const;

private:
    int x_pos;
    int y_pos;
    int length;
    std::vector<int> section_dim;
};

#endif /* BRIDGE_H */
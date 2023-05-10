#include "hammer.hpp"
#include <image_loader.hpp>

Hammer::Hammer(int x_pos, int y_pos, std::vector<int> section_dim, SDL_Renderer* renderer)
    : x_pos(x_pos* section_dim[0]),
    y_pos(y_pos),
    width(2 * section_dim[0]),
    height(2 * section_dim[1]),
    used(false),
    image(nullptr)
{
    ImageLoading imageLoader(renderer);
    image = imageLoader.loadTexture("assets/images/hammer.png", 2 * section_dim[0], 2 * section_dim[1]);
    if (image == nullptr) {
        exit(-1);
    }
}

Hammer::~Hammer() {
    SDL_DestroyTexture(image);
}

void Hammer::draw(SDL_Renderer* renderer, Player* player) {
    if (!used) {
        SDL_Rect hammerRect = { x_pos, y_pos, width, height };

        // Render the hammer onto the screen
        SDL_RenderCopy(renderer, image, nullptr, &hammerRect);

        // Collision detection
        if (SDL_HasIntersection(&player->getRect(), &hammerRect)) {
            used = true;
            player->setHammer(true);
            player->setHammerLength(player->getMaxHammer());
            used = true;
        }
    }
}

void Hammer::setUsed(bool value) {
    used = value;
}

void Hammer::destroy()
{
    SDL_DestroyTexture(image);
}

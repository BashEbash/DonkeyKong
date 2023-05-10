#include "bridge.hpp"

/**
     * @brief Constructor for Bridge objects.
     * @param x_pos X-coordinate of the Bridge.
     * @param y_pos Y-coordinate of the Bridge.
     * @param length Length of the Bridge.
     * @param section_dim Dimensions of each section of the Bridge.
     */
Bridge::Bridge(int x_pos, int y_pos, int length, std::vector<int> section_dim)
    : x_pos(x_pos* section_dim[0]), y_pos(y_pos), length(length), section_dim(section_dim) {

}

void Bridge::draw(SDL_Renderer* renderer) {
    int line_width = 5;
    SDL_Color platform_color = { 225, 51, 129, 255 };

    for (int i = 0; i < length; ++i) {
        int bot_coord = y_pos + section_dim[1];
        int left_coord = x_pos + (section_dim[0] * i);
        int mid_coord = left_coord + (section_dim[0] * 0.5);
        int right_coord = left_coord + section_dim[0];

        SDL_SetRenderDrawColor(renderer, platform_color.r, platform_color.g, platform_color.b, platform_color.a);

        //// Draw 4 lines: top, bottom, left diagonal, right diagonal
        for (int j = 0; j < line_width; ++j) {
            SDL_RenderDrawLine(renderer, left_coord, y_pos + j, right_coord, y_pos + j); // Top line
            SDL_RenderDrawLine(renderer, left_coord, bot_coord - j, right_coord, bot_coord - j); // Bottom line
            SDL_RenderDrawLine(renderer, left_coord, bot_coord - j, mid_coord, y_pos + j); // Left diagonal line
            SDL_RenderDrawLine(renderer, mid_coord, y_pos + j, right_coord, bot_coord - j); // Right diagonal line
        }
    }
}

SDL_Rect Bridge::getCollider() const{
    SDL_Rect collider;
    collider.x = x_pos;
    collider.y = y_pos;
    collider.w = length * section_dim[0];
    collider.h = 10;

    return collider;
}
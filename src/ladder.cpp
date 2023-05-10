#include "ladder.hpp"

Ladder::Ladder(int x_pos, int y_pos, int length, std::vector<int> section_dim) {
    this->x_pos = x_pos;
    this->y_pos = y_pos;
    this->length = length;
    this->section_dim = section_dim;
    // Constructor initializes Ladder properties
}

void Ladder::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF); // Light blue color

    int line_width = 3;
    double lad_height = 0.6;
    for (int i = 0; i < length; ++i) {
        int top_coord = static_cast<int>(y_pos + lad_height * section_dim[1] * i);
        int bot_coord = static_cast<int>(top_coord + lad_height * section_dim[1]);
        int mid_coord = static_cast<int>((lad_height / 2) * section_dim[1] + top_coord);
        int left_coord = x_pos;
        int right_coord = left_coord + section_dim[0];

        // Draw ladder lines
        SDL_RenderDrawLine(renderer, left_coord, top_coord, left_coord, bot_coord);
        SDL_RenderDrawLine(renderer, right_coord, top_coord, right_coord, bot_coord);
        SDL_RenderDrawLine(renderer, left_coord, mid_coord, right_coord, mid_coord);
    }
}

SDL_Rect Ladder::getCollider() const {
    double lad_height = 0.6;
    return SDL_Rect{ x_pos, y_pos - section_dim[1],
                     section_dim[0], static_cast<int>(lad_height * length * section_dim[1] + section_dim[1]) };
}
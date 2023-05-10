#include "barrel.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <image_loader.hpp>

/**
     * @brief Constructor for Barrel objects.
     * @param x_pos X-coordinate of the Barrel.
     * @param y_pos Y-coordinate of the Barrel.
     * @param section_dim Dimensions of the Barrel.
     * @param renderer SDL_Renderer pointer.
     */
Barrel::Barrel(int x_pos, int y_pos, std::vector<int> section_dim, SDL_Renderer* renderer)
    : x_pos(x_pos), y_pos(y_pos),
    section_dim(section_dim),
    barrel_img(nullptr),
    rect({ x_pos, y_pos, static_cast<int>(section_dim[0] * 1.3), static_cast<int>(section_dim[1] * 1.6) }),
    y_change(0), x_change(1), pos(0), count(0), oil_collision(false),
    falling(false), check_lad(false),
    bottom(rect)
{
    ImageLoading imageLoader(renderer);
    barrel_img = imageLoader.loadTexture("assets/images/barrels/barrel.png",
        static_cast<int>(section_dim[0] * 1.5f),
        static_cast<int>(section_dim[1] * 2.0f));

}

/**
     * @brief Update method for Barrel objects.
     * @param plats Vector of Bridge objects for collision detection.
     * @param screen_height Height of the game screen.
     * @param level GameLevel1 object for level information.
     */
void Barrel::update(std::vector<Bridge> plats, int screen_height, GameLevel1 level) {
    // Update barrel movement and collision logic
    if (y_change < 8 && !falling) {
        y_change += 2;
    }

    for (const auto& plat : plats) {
        if (SDL_HasIntersection(&bottom, &plat.getCollider())) {
            y_change = 0;
            falling = false;
        }
    }

    if (!falling) {
        if (level.row_tops[4] - 30 >= (rect.y + rect.h) ||
            (level.row_tops[2] -30>= (rect.y + rect.h) && (rect.y + rect.h) >= level.row_tops[3] -30) ||
            (level.row_tops[0] -30>= (rect.y + rect.h) && (rect.y + rect.h) >= level.row_tops[1] -30)) {
            x_change = 5;
        }
        else {
            x_change = -5;
        }
    }
    else {
        x_change = 0;
    }

    rect.x = static_cast<int>(rect.x + x_change);
    rect.y = static_cast<int>(rect.y + y_change);

    if (count < 15) {
        count += 1;
    }
    else {
        count = 0;
        if (x_change > 0) {
            if (pos < 3) {
                pos += 1;
            }
            else {
                pos = 0;
            }
        }
        else {
            if (pos > 0) {
                pos -= 1;
            }
            else {
                pos = 3;
            }
        }
    }

    bottom = { rect.x, rect.y + rect.h, rect.w, 3 };
}

/**
     * @brief Check if Barrel should fall from Ladder collision.
     * @param lads Vector of Ladder objects for collision detection.
     */
void Barrel::check_fall(std::vector<Ladder> lads) {
    bool already_collided = false;
    SDL_Rect below = { rect.x, rect.y + rect.h, rect.w, section_dim[1] };

    for (const auto& lad : lads) {
        if (SDL_HasIntersection(&below, &lad.getCollider()) && !falling && !check_lad) {
            check_lad = true;
            already_collided = true;
            if (rand() % 61 == 60) {
                falling = true;
                y_change = 4;
            }
        }
    }

    if (!already_collided) {
        check_lad = false;
    }
}


void Barrel::draw(SDL_Renderer* renderer) {
    // draw collider
    SDL_RenderCopyEx(renderer, barrel_img, nullptr, &rect, 90 * pos, nullptr, SDL_FLIP_NONE);
}

SDL_Rect Barrel::getRect()
{
    return rect;
}

void Barrel::destroy() {
    // Free texture
    if (barrel_img != nullptr) {
        SDL_DestroyTexture(barrel_img);
        barrel_img = nullptr;
    }
}
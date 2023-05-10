#include "player.hpp"
#include <image_loader.hpp>

Player::Player(int x_pos, int y_pos, const std::vector<int>& section_dim, SDL_Renderer* renderer):
    x_pos(x_pos),
    y_pos(y_pos),
    section_dim(section_dim),
    y_change(0),
    x_speed(3),
    x_change(0),
    landed(false),
    pos(0),
    dir(1),
    count(0),
    climbing(false),
    hammer(false),
    max_hammer(450),
    hammer_len(max_hammer),
    hammer_pos(1),
    over_barrel(false),
    rect({ x_pos, y_pos, static_cast<int>(section_dim[0] * 2), static_cast<int>(section_dim[1] * 2.5) })
    //rect({ x_pos, y_pos, 0, 0 })
{
    ImageLoading imageLoader(renderer);

    standing = imageLoader.loadTexture("assets/images/mario/standing.png", static_cast<int>(2 * section_dim[0]), static_cast<int>(2.5 * section_dim[1]));
    image = standing;
    running = imageLoader.loadTexture("assets/images/mario/running.png", static_cast<int>(2 * section_dim[0]), static_cast<int>(2.5 * section_dim[1]));
    climbing1 = imageLoader.loadTexture("assets/images/mario/climbing1.png", static_cast<int>(2 * section_dim[0]), static_cast<int>(2.5 * section_dim[1]));
    climbing2 = imageLoader.loadTexture("assets/images/mario/climbing2.png", static_cast<int>(2 * section_dim[0]), static_cast<int>(2.5 * section_dim[1]));
    jumping = imageLoader.loadTexture("assets/images/mario/jumping.png", static_cast<int>(2 * section_dim[0]), static_cast<int>(2.5 * section_dim[1]));
    hammer_jump = imageLoader.loadTexture("assets/images/mario/hammer_jump.png", static_cast<int>(2.5 * section_dim[0]), static_cast<int>(2.5 * section_dim[1]));
    hammer_overhead = imageLoader.loadTexture("assets/images/mario/hammer_overhead.png", static_cast<int>(2.5 * section_dim[0]), static_cast<int>(3.5 * section_dim[1]));

    hitbox = rect;
    hammer_box = rect;
    bottom = { rect.x, rect.y + rect.h, rect.w, 0 };
}

void Player::update(std::vector<Bridge>& plats) {  
    landed = false;

    // Checking collisions with Bridge objects
    for (auto& plat : plats) {
        if (SDL_HasIntersection(&bottom, &plat.getCollider())) {
            landed = true;
            if (!climbing) {
                rect.y = plat.getCollider().y - rect.h + 5;
            }
        }
    }

    // Applying gravity if not landed and not climbing
    if (!landed && !climbing) {
        y_change += 0.25;
    }

    // Moving the player

    rect.x += static_cast<int>(x_change * x_speed);
    rect.y += static_cast<int>(y_change);

    // Updating the bottom rect
    bottom = SDL_Rect{ rect.x + 10, rect.y + rect.h - 20, rect.w - 20, 20 };

    // Update animations and state
    if (x_change != 0 || (climbing && y_change != 0)) {
        if (count < 3) {
            count += 1;
        }
        else {
            count = 0;
            if (pos == 0) {
                pos += 1;
            }
            else {
                pos = 0;
            }
        }
    }
    else {
        pos = 0;
    }

    // Update hammer state
    if (hammer) {
        hammer_pos = (hammer_len / 30) % 2;
        --hammer_len;
        if (hammer_len == 0) {
            hammer = false;
            hammer_len = max_hammer;
        }
    }
}

/**
* @brief Draws the player to the screen.
*
**/
void Player::draw(SDL_Renderer* renderer) {
    SDL_Texture* currentTexture = nullptr;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //SDL_RenderDrawRect(renderer, &bottom);

    // If the player is not holding a hammer draw the player normally based on the current state of the player.
    if (!hammer) {
        if (!climbing && landed) {
            currentTexture = pos == 0 ? standing : running;
        }
        else if (!landed && !climbing) {
            currentTexture = jumping;
        }
        else if (climbing) {
            currentTexture = pos == 0 ? climbing1 : climbing2;
        }
    }
    else {
        currentTexture = hammer_pos == 0 ? hammer_jump : hammer_overhead;
    }

    if (dir == -1) {
        SDL_RenderCopyEx(renderer, currentTexture, NULL, &rect, 0.0, NULL, SDL_FLIP_HORIZONTAL);
        return;
    }
    else {
        SDL_RenderCopy(renderer, currentTexture, NULL, &rect);
        return;
    }

    // 
    calc_hitbox();

    // If the player is holding a hammer draw the hammer.
    if (hammer_pos == 1 && hammer) {
        SDL_Rect hammerRect = { rect.x + 20, rect.y - section_dim[1] - 20, rect.w - 30, rect.h - 30};
        SDL_RenderCopy(renderer, currentTexture, NULL, &hammerRect);
    }
    else {
        SDL_RenderCopy(renderer, currentTexture, NULL, &rect);
    }
}

/**
* @brief Calculates the hitbox of the player based on the current state of the player.
* 
**/
void Player::calc_hitbox() {
    if (!hammer) {
        hitbox = { rect.x + 15, rect.y + 5, rect.w - 30, rect.h - 10 };
    }
    else if (hammer_pos == 0) {
        if (dir == 1) {
            hitbox = { rect.x, rect.y + 5, rect.w - 30, rect.h - 10 };
            hammer_box = { hitbox.x + hitbox.w, rect.y + 5, hitbox.w, rect.h - 10 };
        }
        else {
            hitbox = { rect.x + 40, rect.y + 5, rect.w - 30, rect.h - 10 };
            hammer_box = { hitbox.x - hitbox.w, rect.y + 5, hitbox.w, rect.h - 10 };
        }
    }
    else {
        hitbox = { rect.x + 15, rect.y + 5, rect.w - 30, rect.h - 10 };
        hammer_box = { hitbox.x, hitbox.y - section_dim[1], hitbox.w, section_dim[1] };
    }
}


void Player::setHammer(bool value)
{
    this->hammer = value;
}

bool Player::getHammer() const
{
    return hammer;
}

void Player::setHammerLength(int length)
{
    this->hammer_len = length;
}

int Player::getHammerLength() const
{
    return hammer_len;
}

int Player::getMaxHammer() const
{
    return max_hammer;
}

SDL_Rect Player::getRect() const {
    return rect;
}

void Player::setOverBarrel(bool overbarrel)
{
    this->over_barrel = overbarrel;
}

bool Player::isOverBarrel()
{
    return over_barrel;
}

bool Player::isLanded()
{
    return landed;
}

void Player::setClimbing(bool climbing)
{
    this->climbing = climbing;
}

void Player::setDirection(int dir)
{
    this->dir = dir;
}

bool Player::isClimbing()
{
    return climbing;
}

void Player::setYChange(float y_change)
{
    this->y_change = y_change;
}

float Player::getYChange()
{
    return y_change;
}

SDL_Rect Player::getHammerBox()
{
    return hammer_box;
}

void Player::setLanded(bool landed)
{
    this->landed = landed;
}

void Player::setXChange(float x_change)
{
    this->x_change = x_change;
}

int Player::getBottom() {
    return rect.y + rect.h;
}
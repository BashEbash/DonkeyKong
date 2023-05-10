#pragma once
#include <SDL.h>
#include <vector>
#include <memory>
#include "bridge.hpp"

class Player {
public:
    Player(int x_pos, int y_pos, const std::vector<int>& section_dim, SDL_Renderer* renderer);

    void update(std::vector<Bridge>& plats);
    void draw(SDL_Renderer* renderer);
    void calc_hitbox();

    // Getters and setters for various attributes
    void setHammer(bool value);
    bool getHammer() const;
    void setHammerLength(int length);
    int getHammerLength() const;
    int getMaxHammer() const;
    SDL_Rect getRect() const;
    int getBottom();
    void setOverBarrel(bool overbarrel);
    bool isOverBarrel();
    bool isLanded();
    void setClimbing(bool climbing);
    void setLanded(bool landed);
    void setXChange(float x_change);
    bool isClimbing();
    void setDirection(int dir);
    void setYChange(float y_change);
    float getYChange();
    SDL_Rect getHammerBox();

private:
    int x_pos;
    int y_pos;
    std::vector<int> section_dim;

    SDL_Texture* image;
    SDL_Texture* standing;
    SDL_Texture* running;
    SDL_Texture* climbing1;
    SDL_Texture* climbing2;
    SDL_Texture* jumping;
    SDL_Texture* hammer_jump;
    SDL_Texture* hammer_overhead;

    float y_change;
    float x_speed;
    float x_change;
    bool landed;
    int pos;
    int dir;
    int count;
    bool climbing;
    bool hammer;
    int max_hammer;
    int hammer_len;
    int hammer_pos;
    SDL_Rect rect;
    SDL_Rect hitbox;
    SDL_Rect hammer_box;
    bool over_barrel;
    SDL_Rect bottom;
};
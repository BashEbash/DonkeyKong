#ifndef GAME_H
#define GAME_H

#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "level.hpp" // Include your GameLevel1 header file here
#include <barrel.hpp>

class Game {
public:
    Game(SDL_Window* window, SDL_Renderer* renderer);
    ~Game();

    void reset(std::vector<Barrel>& barrels, std::vector<Hammer>& hammers, std::vector<Hammer>& hammers_list);
    bool checkVictory(const std::vector<GameLevel1>& levels, int active_level, Player* player);
    void drawBarrels(SDL_Texture* barrel, SDL_Renderer* renderer);
    void drawExtras(SDL_Renderer* renderer, SDL_Texture* peach1, SDL_Texture* peach2, int row6_y);
    void drawKong(SDL_Renderer* renderer, int row6_y);
    std::pair<bool, bool> checkClimb(std::vector<Ladder>& lads, Player* player);
    bool barrelCollide(bool reset, Player* player, std::vector<Barrel>& barrels);
    void runGame();
    int getScore();
    bool gameFinished();
    void RenderLostLifeMessage(SDL_Renderer* renderer, TTF_Font* font, int window_width, int window_height);
    bool WaitForGameContinue(SDL_Renderer* renderer, int fps);


private:
    SDL_Window* window;
    int screen_width, screen_height;
    int window_width, window_height;
    int section_width, section_height;
    int fps;
    SDL_Renderer* renderer;
    TTF_Font* font1;
    TTF_Font* font2;
    std::vector<int> section_dim;
    std::vector<GameLevel1> levels;
    bool run;
    int active_level;
    int counter;
    int score;
    int high_score;
    int lives;
    int bonus;
    bool victory;
    bool reset_game;
    int barrel_spawn_time;
    int barrel_count;
    int barrel_time;
    Player* player;
    bool game_finished;
  
    // kong textures
    SDL_Texture* dk1;
    SDL_Texture* dk2;
    SDL_Texture* dk3;
    SDL_Texture* barrel_texture;
    SDL_Texture* barrelImg;
};

#endif // GAME_H
#include <vector>
#include <map>
#include <string>
#include <tuple>
#include <memory>
#include <game_object.hpp>
#include <SDL.h>
#include <bridge.hpp>
#include <hammer.hpp>
#include <ladder.hpp>

#ifndef GAMELEVEL1_HPP
#define GAMELEVEL1_HPP

class GameLevel {
public:
    virtual ~GameLevel() = default;
    virtual void setupLevel() = 0;

    std::vector<int> row_tops;
    SDL_Renderer* renderer;
};

class GameLevel1 : public GameLevel {

    std::vector<std::shared_ptr<Bridge>> bridges;
    std::vector<std::shared_ptr<Ladder>> ladders;
    std::vector<std::shared_ptr<Hammer>> hammers;
    std::vector<std::tuple<int, int, int, int>> targets;

public:
    GameLevel1(int window_height, std::vector<int> section_dim, int slope, SDL_Renderer* renderer);
    void setupLevel() override;

    const std::vector<std::tuple<int, int, int, int>>& getTargets() const;
    std::vector<Ladder> getLadders();
    std::vector<Hammer> getHammers();

    std::vector<Bridge> getBridges();

    int slope;
    int row6_top, row5_top, row4_top, row3_top, row2_top, row1_top;
    int window_height;
    std::vector<int> section_dim;
    std::vector<int> row_tops;
    SDL_Renderer* renderer;

    GameLevel1() = default;
};

#endif // GAMELEVEL1_HPP
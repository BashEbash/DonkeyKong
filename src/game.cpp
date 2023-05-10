#include "game.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <image_loader.hpp>

// Implement other necessary class headers
Game::Game(SDL_Window* window, SDL_Renderer* renderer) : window(window), renderer(renderer){
    TTF_Init();

    SDL_GetWindowSize(window, &window_width, &window_height);
    // Font setup
    font1 = TTF_OpenFont("fonts/freesansbold.ttf", 50);
    font2 = TTF_OpenFont("fonts/freesansbold.ttf", 30);

    // Define section dimensions
    section_width = window_width / 32;
    section_height = window_height / 32;
    section_dim = { section_width, section_height };

    // Set up game state variables
    levels.push_back(GameLevel1(window_height, section_dim, 8, renderer));
    fps = 60;
    run = true;
    active_level = 0;
    counter = 0;
    score = 0;
    high_score = 0;
    lives = 1;
    bonus = 6000;
    victory = false;
    reset_game = false;

    // Level dependencies
    barrel_spawn_time = 360;
    barrel_count = barrel_spawn_time / 2;
    barrel_time = 360;
    player = nullptr;
    game_finished = false;
}

void Game::reset(std::vector<Barrel>& barrels, std::vector<Hammer>& hammers, std::vector<Hammer>& hammers_list) {
    SDL_Delay(1000);

    barrels.clear();

    lives -= 1;
    bonus = 6000;

    if (player != nullptr) {
        delete player;
    }

    player = new Player(250, window_height - 130, section_dim, renderer);

    barrel_spawn_time = 360;
    barrel_count = barrel_spawn_time / 2;
    victory = false;
}


bool Game::checkVictory(const std::vector<GameLevel1>& levels, int active_level,  Player* player) {
    const std::vector<std::tuple<int, int, int, int>>& targets = levels[active_level].getTargets();

    if (!targets.empty()) {
        const std::tuple<int, int, int, int>& target = targets[0];

        int targetX = std::get<0>(target);
        int targetY = std::get<1>(target);
        int targetWidth = std::get<2>(target);
        int targetHeight = std::get<3>(target);

        SDL_Rect targetRect = { targetX, targetY, targetWidth, targetHeight };
        SDL_Rect playerRect = player->getRect();

        // display rect
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

        return SDL_HasIntersection(&playerRect, &targetRect);
    }
    return false;
}

void Game::drawBarrels(SDL_Texture* barrel, SDL_Renderer* renderer) {
    std::vector<std::pair<float, float>> positions = {
        { 1.2, 5.4 },  // First position
        { 2.5, 5.4 },  // Second position
        { 2.5, 7.7 },  // Third position
        { 1.2, 7.7 }   // Fourth position
    };

    for (const auto& pos : positions) {
        SDL_Rect barrelRect = {
            static_cast<int>(section_width * pos.first),
            static_cast<int>(section_height * pos.second),
            0,
            0
        };
        SDL_QueryTexture(barrel, nullptr, nullptr, &barrelRect.w, &barrelRect.h);
        SDL_RenderCopyEx(renderer, barrel, nullptr, &barrelRect, 90, nullptr, SDL_FLIP_NONE);
    }
}

void Game::drawExtras(SDL_Renderer* renderer, SDL_Texture* peach1, SDL_Texture* peach2, int row6_y) {

    SDL_Color color = { 255, 255, 255, 255 };

    // Render text surfaces
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font1, ("POINTS " + std::to_string(score)).c_str(), color);
    SDL_Surface* highScoreSurface = TTF_RenderText_Solid(font1, ("TOP " + std::to_string(high_score)).c_str(), color);
    SDL_Surface* extrasTextSurface = TTF_RenderText_Solid(font2, "  LIVES    BONUS    LEVEL ", color);
    SDL_Surface* valuesSurface = TTF_RenderText_Solid(font2, ("      " + std::to_string(lives + 1) + "           " + std::to_string(bonus) + "          " + std::to_string(active_level + 1) + "  ").c_str(), color);

    // Create textures from surfaces
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_Texture* highScoreTexture = SDL_CreateTextureFromSurface(renderer, highScoreSurface);
    SDL_Texture* extrasTextTexture = SDL_CreateTextureFromSurface(renderer, extrasTextSurface);
    SDL_Texture* valuesTexture = SDL_CreateTextureFromSurface(renderer, valuesSurface);

    // Render textures
    SDL_Rect scoreRect = { 3 * section_width, 2 * section_height, scoreSurface->w, scoreSurface->h };
    SDL_Rect highScoreRect = { 14 * section_width, 2 * section_height, highScoreSurface->w, highScoreSurface->h };
    SDL_Rect extrasTextRect = { 20 * section_width + 5, 4 * section_height, extrasTextSurface->w, extrasTextSurface->h };
    SDL_Rect valuesRect = { 20 * section_width + 5, 5 * section_height, valuesSurface->w, valuesSurface->h };

    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
    SDL_RenderCopy(renderer, highScoreTexture, NULL, &highScoreRect);
    SDL_RenderCopy(renderer, extrasTextTexture, NULL, &extrasTextRect);
    SDL_RenderCopy(renderer, valuesTexture, NULL, &valuesRect);

    // Clean up surfaces and textures
    SDL_FreeSurface(scoreSurface);
    SDL_FreeSurface(highScoreSurface);
    SDL_FreeSurface(extrasTextSurface);
    SDL_FreeSurface(valuesSurface);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(highScoreTexture);
    SDL_DestroyTexture(extrasTextTexture);
    SDL_DestroyTexture(valuesTexture);

    // Draw peach
    SDL_Texture* peachImage = (barrel_count < barrel_spawn_time / 2) ? peach1 : peach2;
    SDL_Rect peachRect = { 10 * section_width, row6_y - 6 * section_height, 0, 0 };

    SDL_QueryTexture(peachImage, nullptr, nullptr, &peachRect.w, &peachRect.h);
    SDL_RenderCopy(renderer, peachImage, nullptr, &peachRect);

    // Drawing barrels and Kong
    drawBarrels(barrel_texture, renderer);
    drawKong(renderer, row6_y);
}

// This function is responsible for drawing the Kong character in the game.
void Game::drawKong(SDL_Renderer* renderer, int row6_y) {
    int phase_time = barrel_time / 4; // Calculating time for different phases
    SDL_Texture* dkImg = nullptr; // Initializing Kong's image

    // Determining Kong's image based on barrel spawning time
    if (barrel_spawn_time - barrel_count > 3 * phase_time) {
        dkImg = dk2; // Assigning Kong's second image
    }
    else if (barrel_spawn_time - barrel_count > 2 * phase_time) {
        dkImg = dk1; // Assigning Kong's first image
    }
    else if (barrel_spawn_time - barrel_count > phase_time) {
        dkImg = dk3; // Assigning Kong's third image
    }
    else {
        // Inverting Kong's first image horizontally
        SDL_Rect dkRect = {
            static_cast<int>(3.5 * section_width),
            static_cast<int>(row6_y - 5.5 * section_height),
            0,
            0
        };
        SDL_QueryTexture(dk1, nullptr, nullptr, &dkRect.w, &dkRect.h); // Getting barrel image dimensions

        // Rendering barrel image flipped horizontally
        SDL_RenderCopyEx(renderer, dk1, nullptr, &dkRect, 0, nullptr, SDL_FLIP_HORIZONTAL);
        return;
    }

    // Setting the rectangle to render Kong's image
    SDL_Rect dkRect = {
            static_cast<int>(3.5 * section_width),
            static_cast<int>(row6_y - 5.5 * section_height),
            0,
            0
    };
    SDL_QueryTexture(dkImg, nullptr, nullptr, &dkRect.w, &dkRect.h); // Getting Kong's image dimensions
    SDL_RenderCopy(renderer, dkImg, nullptr, &dkRect); // Rendering Kong's image
}

// This function is responsible for checking if the player is able to climb a ladder.
std::pair<bool, bool> Game::checkClimb(std::vector<Ladder>& lads, Player* player) {
    bool canClimb = false;
    bool climbDown = false;

    SDL_Rect under = { player->getRect().x, player->getRect().y + 2 * section_height, player->getRect().w, player->getRect().h };

    for (const auto& lad : lads) {
        if (SDL_HasIntersection(&player->getRect(), &lad.getCollider()) && !canClimb) {
            canClimb = true;
        }
        if (SDL_HasIntersection(&under, &lad.getCollider())) {
            climbDown = true;
        }
    }

    if ((!canClimb && (!climbDown || player->getYChange() < 0)) ||
        (player->isLanded() && canClimb && player->getYChange() > 0 && !climbDown)) {
        player->setClimbing(false);
    }

    return std::make_pair(canClimb, climbDown);
}

// This function is responsible for checking if the player collides with a barrel then resetting the game if so.
bool Game::barrelCollide(bool reset, Player* player, std::vector<Barrel>& barrels) {
    SDL_Rect under = { player->getRect().x, player->getRect().y + 2 * section_height, player->getRect().w, player->getRect().h };

    for (auto& brl : barrels) {
        if (SDL_HasIntersection(&brl.getRect(), &player->getRect())) {
            reset = true;
        }
        else if (!player->isLanded() && !player->isOverBarrel() && SDL_HasIntersection(&under, &brl.getRect())) {
            player->setOverBarrel(true);
            score += 100;
        }
    }

    if (player->isLanded()) {
        player->setOverBarrel(false);
    }

    return reset;
}

void Game::RenderLostLifeMessage(SDL_Renderer* renderer, TTF_Font* font, int window_width, int window_height) {
    std::string lostLifeText = "You lost 1 life. Press F to continue...";
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color

    // Render the surface for the lost life text
    SDL_Surface* lostLifeSurface = TTF_RenderText_Solid(font, lostLifeText.c_str(), textColor);
    SDL_Texture* lostLifeTexture = SDL_CreateTextureFromSurface(renderer, lostLifeSurface);

    // Get the dimensions of the text
    int textWidth = lostLifeSurface->w;
    int textHeight = lostLifeSurface->h;

    // Calculate the position to center the text
    int centerX = (window_width - textWidth) / 2;
    int centerY = (window_height - textHeight) / 2;

    // Create a rectangle for the text position
    SDL_Rect lostLifeRect = { centerX, centerY, textWidth, textHeight };

    // Render the text at the calculated position
    SDL_RenderCopy(renderer, lostLifeTexture, NULL, &lostLifeRect);

    // Clean up surface and texture
    SDL_FreeSurface(lostLifeSurface);
    SDL_DestroyTexture(lostLifeTexture);
}

// This function is responsible for displaying the "Press F to continue" message when the player loses a life or finishes a level.
bool Game::WaitForGameContinue(SDL_Renderer* renderer, int fps) {
    bool waitForF = true;

    while (waitForF) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        RenderLostLifeMessage(renderer, font1, window_width, window_height);

        SDL_RenderPresent(renderer);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return false;
                }
                else if (event.key.keysym.sym == SDLK_f) {
                    waitForF = false;
                    break;
                }
            }
        }

        SDL_Delay(1000 / fps);
    }

    return true;
}

void Game::runGame() {
    ImageLoading imageLoader(renderer);
    // Load images
    barrelImg = imageLoader.loadTexture("assets/images/barrels/barrel.png", static_cast<int>(section_width * 1.5), static_cast<int>(section_height * 2));
    barrel_texture = imageLoader.loadTexture("assets/images/barrels/barrel2.png", static_cast<int>(section_width * 2), static_cast<int>(section_height * 2.5));
    // Load donkey kong images
    dk1 = imageLoader.loadTexture("assets/images/dk/dk1.png", section_width * 5, section_height * 5);
    dk2 = imageLoader.loadTexture("assets/images/dk/dk2.png", section_width * 5, section_height * 5);
    dk3 = imageLoader.loadTexture("assets/images/dk/dk3.png", section_width * 5, section_height * 5);
    SDL_Texture* peach1 = imageLoader.loadTexture("assets/images/peach/peach1.png", section_width * 2, section_height * 3);
    SDL_Texture* peach2 = imageLoader.loadTexture("assets/images/peach/peach2.png", section_width * 2, section_height * 3);

    std::vector<Barrel> barrels;
    std::vector<Hammer> hammersList = levels[active_level].getHammers();
    std::vector<Hammer> hammers = levels[active_level].getHammers();

    player = new Player(250, window_height - 200, section_dim, renderer);

    while (run) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // bonus increasing with time
        if (counter < 60) {
            counter += 1;
        }
        else {
            counter = 0;
            if (bonus > 0) {
                bonus -= 100;
            }
        }

        // Draw platforms and ladders
        for (auto& ladder : levels[active_level].getLadders()) {
            ladder.draw(renderer);
        }
        for (auto& platform : levels[active_level].getBridges()) {
            platform.draw(renderer);
        }
        for (auto& ham : hammers) {
            ham.draw(renderer, player);
        }

        drawExtras(renderer, peach1, peach2, levels[active_level].row_tops[5]);

        std::pair<bool, bool> state = checkClimb(levels[active_level].getLadders(), player);
        bool climb = state.first;
        bool down = state.second;

        victory = checkVictory(levels, active_level, player);

        // update barrels
        if (barrel_count < barrel_spawn_time) {
            barrel_count += 1;
        }else {
            barrel_count = std::rand() % 121;
            int barrel_time = barrel_spawn_time - barrel_count;
            Barrel bar(270, 200, section_dim, renderer);
            barrels.push_back(bar);
        }

        for (auto& barrel : barrels) {
            barrel.check_fall(levels[active_level].getLadders());
            barrel.update(levels[active_level].getBridges(), screen_height, levels[active_level]);
            barrel.draw(renderer);
            
            if (SDL_HasIntersection(&barrel.getRect(), &player->getHammerBox()) && player->getHammer()) {
                barrel.destroy();
                score += 500;
            }

            // check if barrel is off screen
            if (barrel.getRect().x > window_width || barrel.getRect().x < 0 ||
                barrel.getRect().y > window_height || barrel.getRect().y < 0) {
                barrel.destroy();
            }
        }

        // update player
        player->update(levels[active_level].getBridges());
        player->draw(renderer);

        // check game reset
        reset_game = barrelCollide(reset_game, player, barrels);
        if (reset_game) {
            if (lives > 0) {
                reset(barrels, hammersList, hammers);
                reset_game = false;
                bool want_continue = WaitForGameContinue(renderer, fps);
                if (!want_continue) {
					run = false;
				}
            }
            else {
                run = false;
                game_finished = true;
            }
        }

        // Key events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run = false; // Set run to false to exit the loop
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    run = false; // Escape key to exit the loop
                    break;
                case SDLK_RIGHT:
                    // Handle right arrow key
                    if (!player->isClimbing()) {
                        player->setXChange(1);
                        player->setDirection(1);
                    }
                    break;
                case SDLK_LEFT:
                    // Handle left arrow key
                    if (!player->isClimbing()) {
                        player->setXChange(-1);
                        player->setDirection(-1);
                    }
                    break;
                case SDLK_SPACE:
                    // Handle space key
                    if (player->isLanded()) {
                        player->setLanded(false);
                        player->setYChange(-6);
                    }
                    break;
                case SDLK_UP:
                    // Handle up arrow key
                    if (climb) {
                        player->setYChange(-2);
                        player->setXChange(0);
                        player->setClimbing(true);
                    }
                    break;
                case SDLK_DOWN:
                    // Handle down arrow key
                    if (down) {
                        player->setYChange(2);
                        player->setXChange(0);
                        player->setClimbing(true);
                    }
                    break;
                default:
                    break;
                }
            }
            else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                case SDLK_RIGHT:
                    // Handle release of right arrow key
                    player->setXChange(0);
                    break;
                case SDLK_LEFT:
                    // Handle release of left arrow key
                    player->setXChange(0);
                    break;
                case SDLK_UP:
                    // Handle release of up arrow key
                    if (climb) {
                        player->setYChange(0);
                    }
                    if (player->isClimbing() && player->isLanded()) {
                        player->setClimbing(false);
                    }
                    break;
                case SDLK_DOWN:
                    // Handle release of down arrow key
                    if (climb) {
                        player->setYChange(0);
                    }
                    if (player->isClimbing() && player->isLanded()) {
                        player->setClimbing(false);
                    }
                    break;
                default:
                    break;
                }
            }
        }
        //check victory
        if (victory) {
            reset_game = true;
            lives += 1;
            score += bonus;
            if (score > high_score) {
                high_score = score;
            }

            score = 0;
            player->setClimbing(false);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / fps);
    }
    TTF_Quit();
}

bool Game::gameFinished()
{
    return game_finished;
}

int Game::getScore()
{
    return score;
}

// Destructor to clean up resources
Game::~Game() {
    TTF_Quit();
}
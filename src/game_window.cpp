#include "game_window.hpp"

GameWindow::GameWindow() :
    window(nullptr),
    renderer(nullptr),
    displayHighScores(false),
    displayMainMenu(true),
    displaySaveScore(false),
    done(false),
    displayedScores(std::vector<HighScore>()) {}

bool GameWindow::Initialize(const char* title, int width, int height) {
    // SDL initialization
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        return false;
    }

    // Create SDL window and renderer
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
    if (window == nullptr) {
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        return false;
    }

    // ImGui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    return true;
}

void GameWindow::Run() {
    while (!done) {
        // Handle SDL events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                done = true;
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(window)) {
                done = true;
            }
        }

        // ImGui rendering
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (displayHighScores) {
            DisplayHighScores();
        }

        if (displayMainMenu) {
            DisplayMainMenu();
        }

        if (displaySaveScore) {
			DisplaySaveScore();
		}

        // Rendering
        int display_w, display_h;
        SDL_GetWindowSize(window, &display_w, &display_h);
        ImGui::Render();
        SDL_RenderSetViewport(renderer, NULL);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }

    Shutdown();
}

void GameWindow::Shutdown() {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GameWindow::LoadHighScores(std::vector<HighScore>& scores) {
    std::ifstream inFile("high_scores.txt");

    if (inFile.is_open()) {
        scores.clear(); // Clear the existing scores

        std::string line;
        while (std::getline(inFile, line)) {
            // Assuming each line has the format "Player: Name, Score: N"
            if (line.find("Player: ") != std::string::npos && line.find(", Score: ") != std::string::npos) {
                // Extract player name and score
                size_t nameStart = line.find("Player: ") + strlen("Player: ");
                size_t nameEnd = line.find(", Score: ");
                size_t scoreStart = line.find(", Score: ") + strlen(", Score: ");

                std::string playerName = line.substr(nameStart, nameEnd - nameStart);
                int score = std::stoi(line.substr(scoreStart));

                scores.push_back({ playerName, score });
            }
        }

        inFile.close();
        std::cout << "High scores loaded from 'high_scores.txt'.\n";
    }
    else {
        std::cout << "Unable to open file for reading!\n";
    }
}

void GameWindow::DisplayMainMenu() {
    ImGui::Begin("Main Menu");

    // Display menu options
    if (ImGui::Button("Start Game")) {
        displayMainMenu = false; // Hide the main menu
        Game game{window, renderer};
        game.runGame();

        score = game.getScore();

        if (game.gameFinished()) {
			displaySaveScore = true;
		}
        else {
			displayMainMenu = true;
		}
    }

    if (ImGui::Button("High Scores")) {
        displayHighScores = true; // Show high scores screen
        displayMainMenu = false; // Hide the main menu
    }

    if (ImGui::Button("Exit")) {
        done = true; // Exit the game
    }

    ImGui::End();
}

void GameWindow::DisplayHighScores() {
    // Sort the scores in descending order
    std::vector<HighScore> sortedScores;
    LoadHighScores(sortedScores);

    std::sort(sortedScores.begin(), sortedScores.end(), [](const HighScore& a, const HighScore& b) {
        return a.score > b.score;
        });

    if (ImGui::Begin("High Scores", &displayHighScores)) {
        // Display the high scores
        for (const auto& score : sortedScores) {
            ImGui::TextWrapped("%s: %d", score.playerName.c_str(), score.score);
        }

        if (!displayHighScores) {
            displayMainMenu = true;
        }

        ImGui::End();
    }
}

void GameWindow::DisplaySaveScore()
{
    static char playerName[64] = ""; // Buffer to hold the player's name

    ImGui::Begin("Game over. Enter your name to save your score.", &displayMainMenu);

    ImGui::InputText("Name", playerName, sizeof(playerName)); // Input field for player's name

    if (ImGui::Button("Save")) {
        SavePlayerScore(playerName, score); // Save the player's score
        displaySaveScore = false; // Hide the save score screen
        displayHighScores = true; // Show the high scores screen
    }

    ImGui::End(); // End ImGui window
}

void GameWindow::SavePlayerScore(const char* playerName, int score) {
    std::ofstream file("high_scores.txt", std::ios_base::app); // Open file in append mode
    if (file.is_open()) {
        file << "Player: " << playerName << ", Score: " << score << "\n";
        file.close();
        std::cout << "Player score saved: Player: " << playerName << ", Score: " << score << std::endl; // Optional: Print to console
    }
    else {
        std::cerr << "Unable to open file!" << std::endl;
    }
}

GameWindow::~GameWindow() {
    Shutdown();
}

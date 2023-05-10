#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <game.hpp>

struct HighScore {
	std::string playerName;
	int score;

	HighScore(const std::string& name, int scr) : playerName(name), score(scr) {}
};

class GameWindow {
public:
	GameWindow();
	~GameWindow();
	bool Initialize(const char* title, int width, int height);
	void Run();
	void Shutdown();
	void LoadHighScores(std::vector<HighScore>& scores);
	void SavePlayerScore(const char* playerName, int score);
	void DisplayMainMenu();
	void DisplayHighScores();
	void DisplaySaveScore();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool displayHighScores;
	bool displayMainMenu;
	bool displaySaveScore;
	bool done = false;
	int score = 0;
	std::vector<HighScore> displayedScores;
};
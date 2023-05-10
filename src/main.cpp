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
#include <../game_window.hpp>

int main(int, char**) {

    GameWindow gameWindow;
    if (!gameWindow.Initialize("My Game", 1120, 930)) {
		return -1;
	}

    gameWindow.Run();
    gameWindow.Shutdown();
	return 0;
}
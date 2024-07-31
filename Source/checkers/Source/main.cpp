
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include <iostream>
#include "Game.h"

int main()
{
	checkers::Game* game = new checkers::Game();
	game->StartGame();
}


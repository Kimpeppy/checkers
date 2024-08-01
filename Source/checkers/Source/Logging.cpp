//-----------------------------------------------------------------------------------------------------------
//
// Logging.cpp
// 
//
// Entire point of this file is to keep logging dependencies in one place
// This file is temperory since there should be better logging libraries in the future

#include "Game.h"
#include "Error.h"

#include <iostream>
#include <sstream> 
#include <regex>
//===========================================================================================================
namespace checkers {

std::string TakeInputs()
{
	bool valid = false;
	std::string input;

	while (!valid)
	{
		
		std::getline(std::cin, input);

		// Display help
		if (input == "h" || input == "H")
		{
			std::cout << "Movement: ## M ##" << std::endl;
			std::cout << "Capture:: ## C ##" << std::endl;
			continue;
		}

		// TODO: Check if the format is valid
		std::regex pattern(R"(\d{2} \w \d{2})");
		if (!(std::regex_match(input, pattern)))
		{
			std::cout << "Wrong format" << std::endl;
			continue;
		}

		valid = true;
	}
	

	return input;
}

void PrintLog(Error::ErrorCode code)
{
	switch (code) {
	case Error::HELP:
		std::cout << "Movement: ## M ##" << std::endl;
		std::cout << "Capture:: ## C ##" << std::endl;
		break;
	case Error::INPUTOUTOFRANGE:
		std::cout << "Input is out of range" << std::endl;
		break;
	case Error::MOVEMENTFAR:
		std::cout << "You can only move one space diagonally" << std::endl;
		break;
	case Error::PIECENOTTHERE:
		std::cout << "There is no piece there" << std::endl;
		break;
	case Error::WRONGDIRECTION:
		std::cout << "Wrong direction" << std::endl;
		break;
	case Error::COMMANDNOTPOSSIBLE:
		std::cout << "Command not possible" << std::endl;
		break;
	case Error::WRONGCOLOR:
		std::cout << "Wrong color" << std::endl;
		break;
	case Error::WRONGFORMAT:
		std::cout << "Wrong format" << std::endl;
		break;
	case Error:: MOVEMENTFAILED:
		std::cout << "You cannot move to that space" << std::endl;
		break;
	case Error::CAPTUREFAILED:
		std::cout << "You cannot capture that piece" << std::endl;
		break;
	case Error::ENDGAME:
		std::cout << "Game over!" << std::endl;
		break;
	}
	
}

void PrintBoard(Pieces* gameBoard, PlayerColor currentTurn, int redPieces, int blackPieces)
{
	int m = 8;
	int n = 8;
	// Initialize Board with Pieces
	std::cout << "------------------------------" << std::endl;
	std::cout << "Black: " << blackPieces << " pieces left" << std::endl;
	std::cout << "Red: " << redPieces << " pieces left" << std::endl;
	std::cout << "__________" << std::endl;
	for (int i = 0; i < m; i++)
	{
		std::cout << i << ":";
		for (int j = 0; j < n; j++)
		{
			if ((gameBoard + i * n + j)->m_playerColor == RED)
			{
				std::cout << "R";
			}

			else if ((gameBoard + i * n + j)->m_playerColor == BLACK)
			{
				std::cout << "B";
			}

			else
			{
				std::cout << ".";
			}

		}
		std::cout << "|";
		std::cout << std::endl;

	}
	std::cout << "__________" << std::endl;
	std::cout << "  01234567" << std::endl;

	std::string curr;
	if (currentTurn == RED)
	{
		curr = "Red";
	}
	else
	{
		curr = "Black";
	}

	std::cout << "Its " << curr << " turn" << std::endl;
}

//===========================================================================================================
}
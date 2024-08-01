//-----------------------------------------------------------------------------------------------------------
//
// Logging.h
//

#pragma once

#include "Game.h"

namespace checkers {

	std::string TakeInputs();
	void PrintBoard(Pieces* gameBoard, PlayerColor currentTurn, int redPieces, int blackPieces);
	void PrintLog(Error::ErrorCode code);

}
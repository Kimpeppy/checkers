//-----------------------------------------------------------------------------------------------------------
//
// Game.cpp
// 
//

#include "Game.h"
#include "Error.h"
#include "Logging.h"


namespace checkers {
//===========================================================================================================
// Construct the gameBoard
Game::Game()
{
	// Initialize Board with Pieces
	for (int i = 0; i < m_rowSize; i++) 
	{
		for (int j = 0; j < m_colSize; j++) 
		{
			*(m_gameBoard + i * m_colSize + j) = { EMPTY, false };
		}
	}

	// Initialize Black side
	for (int i = 0; i < 3; i++) 
	{
		if (i % 2 != 0)
		{
			int j = 1;
			while (j < 7)
			{
				(m_gameBoard + i * m_colSize + j)->m_playerColor = BLACK;
				j += 2;
			}
		}
		else
		{
			int j = 0;
			while (j < 7)
			{
				(m_gameBoard + i * m_colSize + j)->m_playerColor = BLACK;
				j += 2;
			}
		}
	}

	// Initialize Red side
	for (int i = 7; i > 4; i--)
	{
		if (i % 2 == 0)
		{
			int j = 0;
			while (j < 7)
			{
				(m_gameBoard + i * m_colSize + j)->m_playerColor = RED;
				j += 2;
			}
		}
		else
		{
			int j = 1;
			while (j < 7)
			{
				(m_gameBoard + i * m_colSize + j)->m_playerColor = RED;
				j += 2;
			}
		}
	}
}

// Clear up the gameboard
Game::~Game()
{
	delete[] m_gameBoard;
}

// Main Loop of the entire game
void Game::StartGame()
{
	while (m_stateOfGame == gamestate::RUNNING){
		UpdateUser();

		// Get valid inputs
		std::string input = TakeInputs();

		// Try to execute command based on user's input
		m_currentLog = ExecuteCommand(input, m_currentTurn);

		// calculate gamestate
		// Game state should only update when a command is successfully executed
		if (m_currentLog == Error::SUCCESS)
		{
			CheckGameState();
			SwitchTurns();
		}
		
	}
}

void Game::UpdateUser()
{
	// Show user current state of the game
		// Only print board at the start of the game
	if (m_currentLog == Error::SUCCESS)
	{
		PrintBoard(m_gameBoard, m_currentTurn, m_redPlayer.numPieces, m_blackPlayer.numPieces);
	}
	// We must print current error if the input is not successful
	else
	{
		PrintLog(m_currentLog);
	}
}

Error::ErrorCode Game::ExecuteCommand(std::string input, PlayerColor color)
{
	// Convert the string into coordinates and check if the coordinates are correct
	int oldX = input[0] - '0';
	int oldY = input[1] - '0';
	int newX = input[5] - '0';
	int newY = input[6] - '0';
	char command = input[3];

	return ExecuteCommand(oldX, oldY, newX, newY, color, command);
}

Error::ErrorCode Game::ExecuteCommand(int oldX, int oldY, int newX, int newY, PlayerColor color, char command)
{
	bool captureCommand = (command == 'c' || command == 'C');
	bool movementCommand = (command == 'm' || command == 'M');

	// Check if the movement is even valid
	Error::ErrorCode log = ValidMovements(oldX, oldY, newX, newY, m_currentTurn);

	// Check if valid moves are possible
	if (log != Error::SUCCESS)
	{
		return log;
	}

	// We need to make sure the user cannot take move when capture is avaliable
	bool captureAvaliable = (GetPlayer(color).state == moveState::CAPTUREAVALIABLE);

	// Show error if capture is avaliable when the user tries to move
	if (movementCommand && captureAvaliable)
	{
		log = Error::CAPTUREAVALIABLE;
	}

	else if (movementCommand && !captureAvaliable)
	{
		// Try to execute movement command
		log = ExecuteMovement(oldX, oldY, newX, newY, m_currentTurn);
	}

	// Check if capture is possible
	else if (captureCommand)
	{
		// Try to execute capture command
		log = ExecuteCapture(oldX, oldY, newX, newY, m_currentTurn);
	}

	else
	{
		log = Error::COMMANDNOTPOSSIBLE;
	}

	return log;
}

Error::ErrorCode Game::ExecuteMovement(int oldX, int oldY, int newX, int newY, PlayerColor color)
{
	Error::ErrorCode log = CheckMovePiece(oldX, oldY, newX, newY, color);
	if (log == Error::SUCCESS)
	{
		MovePiece(oldX, oldY, newX, newY, color);
	}
	return log;
}

Error::ErrorCode Game::ExecuteCapture(int oldX, int oldY, int newX, int newY, PlayerColor color)
{
	Error::ErrorCode log = CheckCapturePiece(oldX, oldY, newX, newY, color);
	if (log == Error::SUCCESS)
	{
		CapturePiece(oldX, oldY, newX, newY, color);
	}
	return log;
}

// Check if movement is possible
Error::ErrorCode Game::CheckMovePiece(int oldX, int oldY, int newX, int newY, PlayerColor color)
{
	bool movementPossible = (m_gameBoard + newX * m_colSize + newY)->m_playerColor == EMPTY;
	if (movementPossible)
	{
		return Error::SUCCESS;
	}
	return Error::MOVEMENTFAILED;
}

// Move the piece
void Game::MovePiece(int oldX, int oldY, int newX, int newY, PlayerColor color)
{
	(m_gameBoard + oldX * m_colSize + oldY)->m_playerColor = EMPTY;
	(m_gameBoard + newX * m_colSize + newY)->m_playerColor = m_currentTurn;
}



// Check if capture is possible
Error::ErrorCode Game::CheckCapturePiece(int oldX, int oldY, int newX, int newY, PlayerColor color)
{
	// Need to check many conditions to see if capture is possible
	int diffX = newX - oldX;
	int diffY = newY - oldY;
	int captureX = newX + diffX;
	int captureY = newY + diffY;

	// Check to see if there is a piece you can capture
	bool pieceExist = !IsEmpty(newX, newY);
	bool isOppositePiece = GetPiece(newX, newY)->m_playerColor != color;
	bool inRange = (captureX > 7 || captureY > 7) && (captureX < 0 || captureY < 0);

	if (!pieceExist || !isOppositePiece || inRange)
	{
		return Error::CAPTUREFAILED;
	}
	// Check to see if you can move the piece into an empty valid space
	Error::ErrorCode checkMovement = CheckMovePiece(oldX, oldY, captureX, captureY, color);
	
	if (checkMovement == Error::SUCCESS)
	{
		return Error::SUCCESS;

	}

	return Error::CAPTUREFAILED;
}

// Capture the piece
void Game::CapturePiece(int oldX, int oldY, int newX, int newY, PlayerColor color)
{
	// Need to check many conditions to see if capture is possible
	int diffX = newX - oldX;
	int diffY = newY - oldY;
	int captureX = newX + diffX;
	int captureY = newY + diffY;

	GetPiece(newX, newY)->m_playerColor = EMPTY;
	MovePiece(oldX, oldY, captureX, captureY, color);
}

// Check gamestate for both black and red pieces
gamestate::GameState Game::CheckGameState()
{
	// King pieces based on where they are located
	UpdateKing();

	// Update both player's possible moveset
	m_blackPlayer.state = UpdatePlayerCommands(BLACK);
	m_redPlayer.state = UpdatePlayerCommands(RED);

	// If the user cannot make any possible moves, they are in no condition to win
	// If both user cannot make any possible moves, a stalemate occurs
	if (m_blackPlayer.state == moveState::NONEAVALIABLE && m_redPlayer.state == moveState::NONEAVALIABLE)
	{
		return gamestate::STALEMATE;
	}

	else if (m_blackPlayer.state == moveState::NONEAVALIABLE)
	{
		return gamestate::REDWINS;
	}

	else if (m_redPlayer.state == moveState::NONEAVALIABLE)
	{
		return gamestate::BLACKWINS;
	}

	else
	{
		return gamestate::RUNNING;
	}
}

moveState::MoveState Game::UpdatePlayerCommands(PlayerColor color)
{
	if (CapturePossible(color))
	{
		return moveState::CAPTUREAVALIABLE;
	}
	else if (MovementPossible(color))
	{
		return moveState::MOVEMENTAVALIABLE;
	}
	else
	{
		return moveState::NONEAVALIABLE;
	}
}

bool Game::MovementPossible(PlayerColor color)
{
	for (int i = 0; i < m_rowSize; i++)
	{
		for (int j = 0; j < m_colSize; j++)
		{
			int xDirection[4] = { i + 1,i - 1, i - 1, i + 1 };
			int yDirection[4] = { j + 1, j - 1, j + 1, j - 1 };
			for (int k = 0; k < 4; k++)
			{
				int oldX = i;
				int oldY = i;
				int newX = xDirection[k];
				int newY = yDirection[k];

				// Check if movement is valid
				Error::ErrorCode validMovements = ValidMovements(oldX, oldY, newX, newY, color);
				Error::ErrorCode checkMovePiece = CheckMovePiece(oldX, oldY, newX, newY, color);

				if (validMovements == Error::SUCCESS && checkMovePiece == Error::SUCCESS)
				{
					return true;
				}

			}

		}
	}
	return false;
}

bool Game::CapturePossible(PlayerColor color)
{
	for (int i = 0; i < m_rowSize; i++)
	{
		for (int j = 0; j < m_colSize; j++)
		{
			int xDirection[4] = { i + 1,i - 1, i - 1, i + 1 };
			int yDirection[4] = { j + 1, j - 1, j + 1, j - 1 };
			for (int k = 0; k < 4; k++)
			{
				int oldX = i;
				int oldY = i;
				int newX = xDirection[k];
				int newY = yDirection[k];

				// Check if movement is valid
				Error::ErrorCode validMovements = ValidMovements(oldX, oldY, newX, newY, color);
				Error::ErrorCode checkCapturePiece = CheckCapturePiece(oldX, oldY, newX, newY, color);

				if (validMovements == Error::SUCCESS && checkCapturePiece == Error::SUCCESS)
				{
					return true;
				}

			}

		}
	}
	return false;
}

// Check if the coordinates are possible for movements or captures
// Parameters are old coordinates, new coordinates, and the color the piece is suppose to be
Error::ErrorCode Game::ValidMovements(int oldX, int oldY, int newX, int newY, PlayerColor color)
{
	// Check if inputs are valid
	// Number must be in between 0 - 7 inclusive
	if ((oldX > 7 || oldY > 7 || newX > 7 || newY > 7) && (oldX < 0 || oldY < 0 || newX < 0 || newY < 0))
	{
		return Error::INPUTOUTOFRANGE;
	}

	// The difference between old and new x and y coordinates must have a difference of one
	if (abs(newX - oldX) != 1 || abs(newY - oldY) != 1)
	{
		// Error in inputting movement
		return Error::MOVEMENTFAR;
	}

	// You need to check if the piece is even at the old position
	bool emptySpace = IsEmpty(oldX, oldY);
	if (emptySpace)
	{
		// Error in trying to move an empty piece
		return Error::PIECENOTTHERE;
	}

	if ((GetPiece(oldX, oldY))->m_playerColor != color)
	{
		return Error::WRONGCOLOR;
	}

	// Black must go down
	// Red must go up

	bool blackDirection = (newY - oldY) == 1;
	bool redDirection = (newY - oldY) == -1;
	bool isKing = (GetPiece(oldX, oldY))->m_isKing;
	if (color == BLACK && !blackDirection && !isKing)
	{
		return Error::WRONGDIRECTION;
	}

	else if (color == RED && !redDirection && !isKing)
	{
		return Error::WRONGDIRECTION;
	}

	return Error::SUCCESS;
}

bool Game::IsEmpty(int x, int y)
{
	return (m_gameBoard + x * m_colSize + y)->m_playerColor == EMPTY;
}

Pieces* Game::GetPiece(int x, int y)
{
	return (m_gameBoard + x * m_colSize + y);
}

void Game::UpdateKing()
{
	for (int i = 0; i < m_rowSize; i++)
	{
		if (GetPiece(0, i)->m_playerColor == RED)
		{
			GetPiece(0, i)->m_isKing = true;
		}
		else if (GetPiece(m_colSize - 1, i)->m_playerColor == BLACK)
		{
			GetPiece(m_colSize - 1, i)->m_isKing = false;
		}
	}
}

Player Game::GetPlayer(PlayerColor color)
{
	// TODO: build a map later that correspond to the color
	if (color == RED)
	{
		return m_redPlayer;
	}

	return m_blackPlayer;
}
void Game::SwitchTurns()
{
	if (m_currentTurn == RED)
	{
		m_currentTurn = BLACK;
	}
	else
	{
		m_currentTurn = RED;
	}

}

} // namespace checkers
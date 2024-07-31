//-----------------------------------------------------------------------------------------------------------
//
// Game.h
//

#pragma once

#include "Pieces.h"
#include "Player.h"

#include <string>

namespace moveState {

//===========================================================================================================

enum MoveState
{
	CAPTUREAVALIABLE = 0,
	MOVEMENTAVALIABLE = 1,
	NONEAVALIABLE = 2
};

//===========================================================================================================

} // namespace moveState


namespace gamestate{
//===========================================================================================================

// This gamestate will determine if the game can continue or not	
enum GameState
{
	RUNNING = 0,
	BLACKWINS = 1,
	REDWINS = 2,
	STALEMATE = 3,
	
};

//===========================================================================================================

} // namespace gameState


namespace Error {

//===========================================================================================================

// This errorcode must be related to the user's input
enum ErrorCode
{
	SUCCESS = 0,
	HELP = 1,
	INPUTOUTOFRANGE = 2,
	MOVEMENTFAR = 3,
	PIECENOTTHERE = 4,
	WRONGDIRECTION = 5,
	COMMANDNOTPOSSIBLE = 6,
	WRONGCOLOR = 7,
	WRONGFORMAT = 8,
	MOVEMENTFAILED = 9,
	CAPTUREFAILED = 10,
	ENDGAME = 11,
	CAPTUREAVALIABLE = 12
};

//===========================================================================================================

} // namespace Error


namespace checkers {

//===========================================================================================================
// Colors to indicate board
enum PlayerColor
{
	BLACK = 1,
	RED = -1,
	EMPTY = 0
};

//-----------------------------------------------------------------------------------------------------------
// Pieces that can be moved around in the board
struct Pieces
{
	PlayerColor m_playerColor;
	bool m_isKing;
};

//-----------------------------------------------------------------------------------------------------------
// Player stats
struct Player
{
	int numPieces;
	moveState::MoveState state = moveState::NONEAVALIABLE;
	PlayerColor color;
};

//-----------------------------------------------------------------------------------------------------------

// The state of the game
class Game
{
public:
	// Constructor
	Game();

	// Deconstructor
	~Game();

	// Simulate the game
	void StartGame();
	void UpdateUser();

	// Execute commands
	Error::ErrorCode ExecuteCommand(std::string input, PlayerColor color);
	Error::ErrorCode ExecuteCommand(int oldX, int oldY, int newX, int newY, PlayerColor color, char command);
	Error::ErrorCode ExecuteMovement(int oldX, int oldY, int newX, int newY, PlayerColor color);
	Error::ErrorCode ExecuteCapture(int oldX, int oldY, int newX, int newY, PlayerColor color);

	// Function used for checking actions
	Error::ErrorCode CheckMovePiece(int oldX, int oldY, int newX, int newY, PlayerColor color);
	Error::ErrorCode CheckCapturePiece(int oldX, int oldY, int newX, int newY, PlayerColor color);

	// Function used for actions
	void MovePiece(int oldX, int oldY, int newX, int newY, PlayerColor color);
	void CapturePiece(int oldX, int oldY, int newX, int newY, PlayerColor color);

	// Function used to check if commands are possible
	moveState::MoveState UpdatePlayerCommands(PlayerColor color);
	bool MovementPossible(PlayerColor color);
	bool CapturePossible(PlayerColor color);

	// Function used to check if kinging is possible
	void UpdateKing();

	// Validate Inputs
	Error::ErrorCode ValidMovements(int oldX, int oldY, int newX, int newY, PlayerColor color);

	// Check current state of the game
	gamestate::GameState CheckGameState();

	// Utils
	Pieces* GetPiece(int x, int y);
	Player GetPlayer(PlayerColor color);
	bool IsEmpty(int x, int y);

private:
	// Construct Board
	int m_rowSize = 8;
	int m_colSize = 8;
	Pieces* m_gameBoard = new Pieces[m_rowSize * m_colSize];
	PlayerColor m_currentTurn = BLACK;

	// Construct player's stats
	Player m_redPlayer = {12, moveState::NONEAVALIABLE, RED};
	Player m_blackPlayer = { 12, moveState::NONEAVALIABLE, BLACK };
	
	// Private function to switch turns
	void SwitchTurns();

	// Check if the user's input is valid
	Error::ErrorCode m_currentLog = Error::SUCCESS;

	// Check if the game should still be running
	gamestate::GameState m_stateOfGame = gamestate::RUNNING;
};

//===========================================================================================================

} // namespace checkers



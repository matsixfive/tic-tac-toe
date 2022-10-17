#include <iostream>
#include <string.h>
// using namespace std;

class Lines
{
public:
	std::string vertical = "|";
	std::string horizontal = "-";
	std::string cross = "+";
	std::string bar;

	Lines()
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				bar += horizontal;
			};
			if (i < 2)
			{
				bar += cross;
			}
		};
	};
};

class Colors
{
public:
	std::string start = "\033[";
	std::string red = ";31m";
	std::string blue = ";34m";
	std::string grey = ";30m";
	std::string end = "\033[0m";
};

std::string formatColor(std::string str, std::string color, bool bold = false)
{
	const std::string red = "red";
	const std::string blue = "blue";
	const std::string grey = "grey";

	Colors Colors;

	if (color == red)
	{
		return Colors.start + (bold ? "1" : "") + Colors.red + str + Colors.end;
	}
	else if (color == blue)
	{
		return Colors.start + (bold ? "1" : "") + Colors.blue + str + Colors.end;
	}
	else if (color == grey)
	{
		return Colors.start + (bold ? "1" : "") + Colors.grey + str + Colors.end;
	}

	return str;
};

std::string getBoardCharacter(int num, int i, int j)
{
	std::string character;
	switch (num)
	{
	case 1:
		character = formatColor("X", "red", true);
		break;
	case 2:
		character = formatColor("O", "blue", true);
		break;
	default:
		character = formatColor(std::to_string((j + 1) + 3 * i), "grey", false);
		break;
	}
	return character;
};

void printBoard(int board[3][3])
{
	Lines Lines;
	for (int i = 0; i < 3; ++i)
	{
		std::string start = " ";
		std::cout << start;
		for (int j = 0; j < 3; ++j)
		{
			std::string end = " " + Lines.vertical + " ";
			if (j >= 2)
			{
				end = " ";
			};

			std::cout << getBoardCharacter(board[i][j], i, j) << end;
		};
		if (i < 2)
		{
			std::cout << "\n"
								<< Lines.bar
								<< "\n";
		}
		else
		{
			std::cout << "\n";
		};
	};
};

bool isNumber(const std::string &s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it))
		++it;
	return !s.empty() && it == s.end();
};

void gameLoop(int (*boardPointer)[3][3], bool *playerTurn)
{
	std::string positionString;
	int position, x, y;

	while (true)
	{
		std::cout << "Where would you like to go? ";
		std::getline(std::cin, positionString);

		if (!isNumber(positionString))
		{
			std::cout << "\n"
								<< "Must be a number between 1 and 9"
								<< "\n";
			continue;
		}
		else
		{
			position = std::stoi(positionString);
		};

		if (position < 1 || position > 9)
		{
			std::cout << "\n"
								<< "Must be a number between 1 and 9"
								<< "\n";
			continue;
		}
		else
		{
			x = (position - 1) % 3;
			y = (position - 1) / 3;

			if ((*boardPointer)[y][x] == 0)
			{
				break;
			}
			else
			{
				std::cout << "\n"
									<< "That space is already taken"
									<< "\n";
			};
		};
	};

	(*boardPointer)[y][x] = *playerTurn + 1;
	*playerTurn = *playerTurn == false ? true : false;
};

int isWinner(int board[3][3])
{
	/*
	0: no winner yet
	1: player 1 (X) wins
	2: player 1 (O) wins
	3: draw
	*/

	for (int i = 0; i < 3; ++i)
	{
		// vertical
		if (board[0][i] == board[1][i] &&
				board[1][i] == board[2][i] &&
				board[2][i] != 0)
		{
			return board[0][i];
		};

		// horizontal
		if (board[i][0] == board[i][1] &&
				board[i][1] == board[i][2] &&
				board[i][2] != 0)
		{
			return board[i][0];
		};
	}

	// top left to bottom right diagonal
	if (board[0][0] == board[1][1] &&
			board[1][1] == board[2][2] &&
			board[2][2] != 0)
	{
		return board[1][1];
	};

	// bottom left to top right diagonal
	if (board[0][2] == board[1][1] &&
			board[1][1] == board[2][0] &&
			board[2][0] != 0)
	{
		return board[1][1];
	};

	// draw
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (board[i][j] == 0)
			{
				return 0; // not won yet
			}
		};
	};

	return 3; // if all others fail, draw
};

int main()
{
	int board[3][3] = {
			{0, 0, 0},
			{0, 0, 0},
			{0, 0, 0}};

	bool playerTurn = false; // false = 0 = 'X', true = 1 = 'O'

	do
	{
		printBoard(board);

		gameLoop(&board, &playerTurn);

	} while (isWinner(board) == 0);

	int winner = isWinner(board);

	printBoard(board);

	std::cout << "\n";

	if (winner == 1)
	{
		std::cout << formatColor("X", "red", true) + " wins";
	}
	else if (winner == 2)
	{
		std::cout << formatColor("O", "blue", true) + " wins";
	}
	else
	{
		std::cout << "It's a " + formatColor("draw", "grey", true);
	};

	return 0;
};
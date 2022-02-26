#include <iostream>
#include <cstring>
#include <Windows.h> //for color + music/sound
#include <conio.h> //for getch and system

using namespace std;

struct Board {
	int L, W; //w column, L: row
	int grid[20][20] = { 0 }; //0 represents empty position
	char playerName[2][6];
	int cursor;
	int menuCursor;
};
//function prototypes
//board design functions
int maxV(int, int);
int minV(int, int);
void printHorizontalLine(int, int, int, int);
void printVerticalLine(int, int, int, int);
void printMostLeftVerticalLine(int, int, int);
void printPIcon(Board, int, int, int, int);
void printBoard(Board, int, int);
void printSpaces(int);
void colorText(int);
//gameplay functions
void playGame(Board&, int&, int&, int&, bool&, int, int, int&, bool, bool&, char&);
int findTheWinner(Board, int);
bool Istie(Board);
//game setting functions
void printMenuSelection(Board, int[]);
void boardSelection(bool&, Board&, int[]);
void changeTextColor(Board, int&, int&);
void printmusicSettings(bool&);
//Account management
void printOutcome(bool, Board, int, int, int);

///

int main()
{
	Board B;
	char name[2][6], c;
	int player = 1, winner = 0, p1Moves = 0, p2Moves;
	int boardSize[3] = { 3,5,7 };
	bool boardSelectionisMade = false, tie = false, gameReplay = false, replaySelection = false;

	std::cout << "Welcome to TICTACTOE!" << endl;
	//players' icons
	for (int i = 0; i < 2; i++)
	{
		std::cout << "Player " << i + 1 << ": ";
		std::cin >> name[i];
		memcpy(B.playerName[i], name[i], 6);
	}

	B.menuCursor = 0;
	boardSelection(boardSelectionisMade, B, boardSize);

	//change text color
	int color1 = 0, color2 = 0;
	changeTextColor(B, color1, color2);

	//sound settings
	bool hasSound = false;
	printmusicSettings(hasSound);

	playGame(B, winner, p1Moves, p2Moves, tie, color1, color2, player, hasSound, gameReplay, c);

	return 0;
}

//board design functions
int maxV(int a, int b)
{
	return (a >= b ? a : b);
}
int minV(int a, int b)
{
	return (a <= b ? a : b);
}
void printHorizontalLine(int n, int r, int l, int w) //r for row, l: len, w: wid of the cursor
{
	std::cout << "    ";
	for (int i = 0; i < n; i++)
	{
		if ((i == w) && ((r == l) || (r == l + 1))) //r == l: set that row
		{
			colorText(FOREGROUND_GREEN);
			std::cout << " -------";
			colorText(7); //back to white
		}
		else std::cout << " -------";
	}
	std::cout << std::endl;

}
void printVerticalLine(int n, int r, int l, int w)
{
	for (int i = 0; i < n; i++)
	{
		if ((r == l) && (i == w || (i == w - 1)))
		{
			colorText(FOREGROUND_GREEN);
			std::cout << "       |";
			colorText(7);
		}
		else std::cout << "       |";

	}
	std::cout << std::endl;
}
void printMostLeftVerticalLine(int i, int len, int wid)
{
	if (i == len && wid == 0)
	{
		colorText(FOREGROUND_GREEN);
		std::cout << "|";
		colorText(7);
	}
	else
		std::cout << "|";
}
void printPIcon(Board B, int i, int j, int color1, int color2)
{
	if ((B.grid[i][j] - 1) == 0)
	{
		colorText(color1);
		std::cout << B.playerName[B.grid[i][j] - 1]; //B.grid[i][j] - 1 = 0 or 1
		colorText(7);
	}
	else
	{
		colorText(color2);
		std::cout << B.playerName[B.grid[i][j] - 1]; //B.grid[i][j] - 1 = 0 or 1
		colorText(7);
	}
}
void printBoard(Board B, int color1, int color2)
{

	int i, space;
	int len = B.cursor / B.W; //y coordinates of the cursor
	int wid = B.cursor % B.W; //x 

	std::cout << "     ";
	//first line of letters
	for (int i = 0; i < B.W; i++)
	{
		std::cout << "   " << static_cast<char>(65 + i) << "    ";
	}
	std::cout << endl;
	for (i = 0; i < B.L; i++)
	{
		printHorizontalLine(B.W, i, len, wid);
		//first row
		std::cout << "    ";
		printMostLeftVerticalLine(i, len, wid);
		printVerticalLine(B.W, i, len, wid);
		//second row
		std::cout << "  " << i << " ";

		printMostLeftVerticalLine(i, len, wid);

		for (int j = 0; j < B.W; j++)
		{
			if (B.grid[i][j] != 0)
			{
				if (strlen(B.playerName[B.grid[i][j] - 1]) % 2 == 0) //print 1 more space if player's name is even
				{
					space = (7 - strlen(B.playerName[B.grid[i][j] - 1])) / 2; //B.player[0] or B.player[1]
					printSpaces(space); //print left spaces
					printPIcon(B, i, j, color1, color2);
					printSpaces(space); //print right spaces
					std::cout << " ";
				}
				else {
					space = (7 - strlen(B.playerName[B.grid[i][j] - 1])) / 2; //B.player[0] or B.player[1]
					printSpaces(space); //print left spaces
					printPIcon(B, i, j, color1, color2);
					printSpaces(space); //print right spaces
				}

				if ((i == len) && (j == wid || (j == wid - 1)))
				{
					colorText(FOREGROUND_GREEN);
					std::cout << "|";
					colorText(7);
				}
				else std::cout << "|";
			}
			else {
				if ((i == len) && (j == wid || (j == wid - 1)))
				{
					colorText(FOREGROUND_GREEN);
					std::cout << "       |";
					colorText(7);
				}
				else std::cout << "       |";
			}
		}
		std::cout << endl;
		//third row
		std::cout << "    ";
		printMostLeftVerticalLine(i, len, wid);
		printVerticalLine(B.W, i, len, wid);

	}
	printHorizontalLine(B.W, B.W - 1, len, wid);
}
void printSpaces(int space)
{
	for (int k = 0; k < space; k++)
		std::cout << " ";
}
void colorText(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//gameplay function
void playGame(Board& B, int& winner, int& p1Moves, int& p2Moves, bool& tie, int color1, int color2, int& player, bool hasSound, bool& gameReplay, char& c)
{
	do
	{
		//reset values
		B.cursor = 0; //default position for cursor
		winner = 0;
		p1Moves = 0;
		p2Moves = 0;
		for (int i = 0; i < B.W; i++)
			for (int j = 0; j < B.W; j++)
			{
				B.grid[i][j] = 0;
			}
		while (winner == 0 && tie == false) {
			system("cls");
			printBoard(B, color1, color2);
			c = _getch();
			switch (c)
			{
			case 'w':
			{
				if (B.cursor >= B.W) {
					B.cursor -= B.W;
				}
				break;
			}
			case 's':
			{
				if (B.cursor <= B.W * (B.L - 1) - 1) { //check if B.cursor is at the second row, if it is at the last row, B.cursor wont move up
					B.cursor += B.W;
				}
				break;
			}
			case 'a':
			{
				if (B.cursor > 0) {
					B.cursor -= 1;
				}
				break;
			}
			case 'd':
			{
				if (B.cursor < B.W * B.L - 1) {
					B.cursor += 1;
				}
				break;
			}
			case 13: //enter 
			{
				if (player == 1)
				{
					if (B.grid[B.cursor / B.W][B.cursor % B.W] == 0)
					{
						B.grid[B.cursor / B.W][B.cursor % B.W] = player; //player's num 
						p1Moves++;
						tie = Istie(B);
						winner = findTheWinner(B, player);
						player = 2; //switch turn
					}
					else
					{
						std::cout << "You cant move there.";
						c = _getch();
					}
				}
				else if (player == 2)
				{
					if (B.grid[B.cursor / B.W][B.cursor % B.W] == 0)
					{
						B.grid[B.cursor / B.W][B.cursor % B.W] = player; //player's num
						p2Moves++;
						tie = Istie(B);
						winner = findTheWinner(B, player);
						player = 1; //switch turn
					}
					else
					{
						std::cout << "You cant move there.";
						c = _getch();
					}
				}

			}
			}
		}
		system("cls");
		printBoard(B, color1, color2);
		if (hasSound == true)
		{
			mciSendString(TEXT("play correct.wav"), NULL, 0, 0); //play sound 
		}
		printOutcome(tie, B, p1Moves, p2Moves, winner);
		std::cout << "Game replay? Y or N" << endl;
		std::cin >> c;
		switch (c)
		{
		case 'Y':
		case 'y': gameReplay = true;
			break;
		case 'N':
		case 'n': gameReplay = false;
		}
	} while (gameReplay == true);
}

int findTheWinner(Board B, int player)
{
	//condition to win 3x3: 3 in a row
	//condition to win 5x5: 4 in a row
	//condition to win 7x7: 5 in a row
	// condition to win 8+ x 8+: 6 in a row
	int winScore; 
	switch (B.W)
	{
	case 3: winScore = 3; 
		break; 
	case 5: winScore = 4; 
		break; 
	case 7: winScore = 5; 
		break; 
	default: winScore = 6; 
	}
	//check horizontal
	int count = 0;
	for (int i = 0; i < B.W; i++)
	{
		for (int j = 0; j < B.W; j++)
		{
			if (B.grid[i][j] == 1 && count >= 0) count++;
			if (B.grid[i][j] == 2 && count <= 0) count--;
			if (B.grid[i][j] == 0) count = 0;
			//winning conditions
			if (count == winScore) return 1;
			if (count == -winScore) return 2;
		}
	}
	//check vertical 
	for (int j = 0; j < B.W; j++)
	{
		for (int i = 0; i < B.W; i++)
		{
			if (B.grid[i][j] == 1 && count >= 0) count++;
			if (B.grid[i][j] == 2 && count <= 0) count--;
			if (B.grid[i][j] == 0) count = 0;
			//winning conditions
			if (count == winScore) return 1;
			if (count == -winScore) return 2;
		}
	}
	//check diagonal
	//bottom left to top right, starting from the top left corner
	//checking only lines with enough squares to win
	for (int line = winScore; line <= (B.W * 2) - winScore; ++line)
	{
		int startCol = maxV(line - B.W, 0);
		int startRow = minV(line - 1, B.W - 1);
		//get number of elements every diagonal 
		int cntMem = minV(line, B.W - startCol);
		//iterate every diagonal and update count
		int count = 0;
		while (cntMem--)
		{
			if (B.grid[startRow][startCol] == 1 && count >= 0) count++;
			if (B.grid[startRow][startCol] == 2 && count <= 0) count--;
			if (B.grid[startRow][startCol] == 0) count = 0;
			//winning conditions
			if (count == winScore) return 1;
			if (count == -winScore) return 2;
			//update grid pos
			startRow--; startCol++;
		}
	}
	//bottom right to top left, starting from the top right corner
	int cntCol = B.W - 1;
	for (int line = winScore; line <= (B.W * 2) - winScore; ++line)
	{
		int startRow = minV(line - 1, B.W - 1);
		int startCol = (line < B.W ? B.W - 1 : cntCol--);
		//get number of elements every diagonal 
		int cntMem = minV(line, B.W - (startRow - startCol));
		//iterate every diagonal and update count
		int count = 0;
		while (cntMem--)
		{
			if (B.grid[startRow][startCol] == 1 && count >= 0) count++;
			if (B.grid[startRow][startCol] == 2 && count <= 0) count--;
			if (B.grid[startRow][startCol] == 0) count = 0;
			//winning conditions
			if (count == winScore) return 1;
			if (count == -winScore) return 2;
			//update grid pos
			startRow--; startCol--;
		}
	}

	//tie
	return 0;
}
bool Istie(Board B)
{
	for (int i = 0; i < B.W; i++)
		for (int j = 0; j < B.W; j++)
		{
			if (B.grid[i][j] == 0)
				return false;
		}
	return true;
}

//game setting functions
void changeTextColor(Board B, int& color1, int& color2)
{
	char c;
	std::cout << "Choose text color for player 1: A for aqua, B for red, P for purple and Y for yellow: \n";
	std::cin >> c;
	switch (c)
	{
	case 'A':
	case 'a': color1 = 3;
		break;
	case 'b':
	case 'B': color1 = 4;
		break;
	case 'P':
	case 'p': color1 = 5;
		break;
	case 'Y':
	case 'y': color1 = 6;
	}

	std::cout << "Choose text color for player 2: A for aqua, B for red, P for purple and Y for yellow: \n";
	std::cin >> c;
	switch (c)
	{
	case 'A':
	case 'a': color2 = 3;
		break;
	case 'b':
	case 'B': color2 = 4;
		break;
	case 'P':
	case 'p': color2 = 5;
		break;
	case 'Y':
	case 'y': color2 = 6;
	}
}
void printmusicSettings(bool& hasSound)
{
	int c;
	std::cout << "Background music: Type 1 or 2 \n1. ON \n2. OFF\n";
	std::cin >> c;
	if (c == 1)
		PlaySound(TEXT("cozy_music.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	std::cout << "Sound effect: Type 1 or 2 \n1. ON \n2. OFF\n";
	std::cin >> c;
	if (c == 1)
		hasSound = true;
	else hasSound = false;

}
void printMenuSelection(Board B, int boardSize[])
{
	std::cout << "Board selection (W A S D to move cursor)" << endl;
	for (int i = 0; i < 3; i++)
	{
		if (B.menuCursor == i)
		{
			colorText(10 * 13 + 2);
			std::cout << boardSize[i] << "x" << boardSize[i] << endl;
			colorText(7);
		}
		else
			std::cout << boardSize[i] << "x" << boardSize[i] << endl;
	}
}
void boardSelection(bool& boardSelectionisMade, Board& B, int boardSize[])
{
	char c;
	while (boardSelectionisMade == false)
	{
		system("cls");
		printMenuSelection(B, boardSize);

		c = _getch();
		switch (c)
		{
		case 'w':
		{
			if (B.menuCursor >= 1)
				B.menuCursor -= 1;
			break;
		}
		case 's': {
			if (B.menuCursor <= 1)
				B.menuCursor += 1;
			break;
		}
		case 13:
		{
			switch (B.menuCursor)
			{
			case 0:
			{
				B.L = 3;
				B.W = 3;
				break;
			}
			case 1:
			{
				B.L = 5;
				B.W = 5;
				break;
			}
			case 2:
			{
				B.L = 7;
				B.W = 7;
			}
			}
			boardSelectionisMade = true;
			B.menuCursor = 0;
		}
		}
	}
}

//Account management
void printOutcome(bool  tie, Board B, int p1, int p2, int winner)
{
	std::cout << "-----------------\n";
	std::cout << "Statiscal outcome\n";
	std::cout << "-----------------\n";

	if (tie == false)
		std::cout << "The winner is " << B.playerName[winner - 1] << endl;
	else std::cout << "It is a tie!" << endl;
	std::cout << B.playerName[0] << " used " << p1 << " moves.\n";
	std::cout << B.playerName[1] << " used " << p2 << " moves.\n";
}

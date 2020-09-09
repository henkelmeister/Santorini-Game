#include <stdio.h>
#include <stdbool.h>

/*
here lies the resting place of my old global varibles. 

int board[7][7];
bool isNotWon;
int playerPos;
int aiPos;
bool playerWon;
bool aiWon;
int pAdjList[8];
int aiAdjList[8];
*/

void start();
bool isDone();
bool isValidInput(int cord,int testinput);
bool validPlayerMove(int num);
void printBoard();
void movePlayer(int move);
void moveAi(int move);
void insertAi();
void incOctagonal();
void decOcatagonal();
void doNextMoveAi();
void updateAdjList();
void decCord(int cord);
void incCord(int cord);

//Technically I made one struct to encapsualte my global varibles into a composite data type
//Therefore there it is only one global varible.
//You cannot tell me otherwise
//haha. 
struct game
{
    int board[7][7];
    bool isNotWon;
    int playerPos;
    int aiPos;
    bool playerWon;
    bool aiWon;
    int pAdjList[8];
    int aiAdjList[8];
} game;

//Main function where execution of other fuctions occur.
void main()
{
    start();
    int moveAttempt;
    while (!isDone())
    {
        printf("\nmake your next move:");
        scanf("%d", &moveAttempt);
        printf("\n");
        if (validPlayerMove(moveAttempt))
        {
            movePlayer(moveAttempt);
            doNextMoveAi();
            updateAdjList();
            incOctagonal();
            decOcatagonal();
            printBoard();
            printf("\nPosition updated\n");
        }
    }
    if (game.playerWon)
    {
        printf("\nThe player has won!!!\n");
    }
    if (game.aiWon)
    {
        printf("\nThe ai has won :(\n");
    }
}

//Initalizes the game start
void start()
{
    game.isNotWon = true;
    game.playerWon = false;
    game.aiWon = false;
    int tempboard[7][7] = {
        {0, 1, 2, 3, 4, 5, 6},
        {1, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2},
        {3, 2, 2, 2, 2, 2, 2},
        {4, 2, 2, 2, 2, 2, 2},
        {5, 2, 2, 2, 2, 2, 2},
        {6, 2, 2, 2, 2, 2, 2}};

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            game.board[i][j] = tempboard[i][j];
        }
    }
    //This is where the player selects their starting point
    printf("Welcome to the game\n\n");
    printf("choose a starting coodinate on the 6x6 board:\n(write in the format of \"14\" for coordinate row 1 column 4 ): ");
    bool temp = true;
    int testinput; 
    while (temp)
    {
        scanf("%d", &testinput);
        if (isValidInput(-100,testinput))
        {
            game.playerPos= testinput;
            printf("valid input, let the games begin...\n\n");
            insertAi();
            printBoard();
            temp = false;
        }
        else
        {
            printf("\nINVALID INPUT, try again\n");
        }
    }
}

/*
per stated in the rules, this method checks if there are 
more than 10 zeros or 10 fours on the gameboard because if so 
the game is over and whoever has 10 of each number wins the game
*/
bool isDone()
{
    int numZeros = 0;
    int numFours = 0;

    for (int i = 1; i < 7; i++)
    {
        for (int j = 1; j < 7; j++)
        {
            if (game.board[i][j] == 4)
            {
                numFours++;
            }
            if (game.board[i][j] == 0)
            {
                numZeros++;
            }
        }
    }

    if (numZeros >= 10 && numFours >= 10)
    {
        printf("\nBOTH SIDES WIN!!! TIE BREAKER!!!\n");
        return true;
    }

    if (numZeros >= 10)
    {
        game.isNotWon = false;
        game.aiWon = true;
        return true;
    }

    if (numFours >= 10)
    {
        game.isNotWon = false;
        game.playerWon = true;
        return true;
    }

    return false;
}

//checks if the cordinate is within the bounds of the matrix
bool isValidInput(int cord,int testinput)
{
    //specifiec case used for game start
    if (cord == -100)
    {
        if ((testinput >= 11 && testinput <= 16) ||
            (testinput >= 21 && testinput <= 26) ||
            (testinput >= 31 && testinput <= 36) ||
            (testinput >= 41 && testinput <= 46) ||
            (testinput >= 51 && testinput <= 56) ||
            (testinput >= 61 && testinput <= 66))
            return true;
        else
        {
            printf("\ncoordinate is out of bounds");
            return false;
        }
    }
    if ((cord >= 11 && cord <= 16) ||
        (cord >= 21 && cord <= 26) ||
        (cord >= 31 && cord <= 36) ||
        (cord >= 41 && cord <= 46) ||
        (cord >= 51 && cord <= 56) ||
        (cord >= 61 && cord <= 66))
    {
        if (cord != game.playerPos&& cord != game.aiPos)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

//prints out the current state of the game board
void printBoard()
{

    //Obtain the X and Y cordinates of the player and Ai
    int pRow = game.playerPos/ 10;
    int pColumn = game.playerPos % 10;
    int aiRow = game.aiPos / 10;
    int aiColumn = game.aiPos % 10;

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (i == pRow && j == pColumn)
            {
                printf("P  ");
            }
            else if (i == aiRow && j == aiColumn)
            {
                printf("A  ");
            }
            else
            {
                printf("%d  ", game.board[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void doNextMoveAi()
{
    bool hasMoved = false;
    int moveInY = 0;
    int moveInX = 0;
    int pRow = game.playerPos / 10;
    int pColumn = game.playerPos % 10;
    int aiRow = game.aiPos / 10;
    int aiColumn = game.aiPos % 10;

    //My Ai is designed to move towards the player every turn
    //finds the closest open square and moves to it
    if (pRow > aiRow)
    {
        moveInY = 1;
    }
    else
    {
        moveInY = -1;
    }
    if (pColumn > aiColumn)
    {
        moveInX = 1;
    }
    else
    {
        moveInX = -1;
    }

    //If the Ai predicts the next move of the player it is designed to not move.
    if (aiRow + moveInY == pRow && pColumn == aiColumn + moveInX)
    {
        moveInX = 0;
        moveInY = 0;
    }
    moveAi(((moveInY + aiRow) * 10) + (aiColumn + moveInX));
    return;
}

//moves player to a new specified cord
void movePlayer(int move)
{
    if (validPlayerMove(move))
    {
        game.playerPos = move;
        return;
    }
}

//moves Ai to a new speciefied cord
void moveAi(int move)
{
    if (isValidInput(move,0))
    {
        game.aiPos = move;
        return;
    }
}

//inserts Ai in an open space next to player at start of game
void insertAi()
{
    if (isValidInput(game.playerPos + 1,0))
    {
        game.aiPos = game.playerPos + 1;
        return;
    }
    if (isValidInput(game.playerPos - 1,0))
    {
        game.aiPos = game.playerPos - 1;
        return;
    }
    if (isValidInput(game.playerPos - 10,0))
    {
        game.aiPos = game.playerPos - 10;
        return;
    }
    if (isValidInput(game.playerPos + 10,0))
    {
        game.aiPos = game.playerPos + 10;
        return;
    }
}

/*checks if the player can move to the specified 
location by checking bounds and if the given cordinate is one of the 
octagonals around the current position 
*/

bool validPlayerMove(int num)
{
    if (isValidInput(num,0))
    {
        if (game.playerPos + 1 == num)
        {
            return true;
        }
        if (game.playerPos - 1 == num)
        {
            return true;
        }
        if (game.playerPos + 10 == num)
        {
            return true;
        }
        if (game.playerPos - 10 == num)
        {
            return true;
        }
        if (game.playerPos + 11 == num)
        {
            return true;
        }
        if (game.playerPos - 11 == num)
        {
            return true;
        }
        if (game.playerPos - 9 == num)
        {
            return true;
        }
        if (game.playerPos + 9 == num)
        {
            return true;
        }
    }
    printf("NOT VALID MOVE");
    return false;
}

//cross compares adjacency lists and increases cords that are not shared between lists
void incOctagonal()
{
    int rowPlayer = game.playerPos / 10;
    int columnPlayer = game.playerPos % 10;
    for (int i = 0; i < 8; i++)
    {
        bool match = false;
        for (int j = 0; j < 8; j++)
        {
            if (game.pAdjList[i] == game.aiAdjList[j])
            {
                match = true;
            }
        }
        if (match == true)
            continue;

        if (match == false)
        {
            incCord(game.pAdjList[i]);
        }
    }
}

//cross compares adjacency lists and decreases cords that are not shared between lists
void decOcatagonal()
{
    for (int i = 0; i < 8; i++)
    {
        bool match = false;
        for (int j = 0; j < 8; j++)
        {
            if (game.aiAdjList[i] == game.pAdjList[j])
            {
                match = true;
            }
        }
        if (match == true)
            continue;

        if (match == false)
        {
            decCord(game.aiAdjList[i]);
        }
    }
}
//increases given cord provided it is less than 4
void incCord(int cord)
{
    int rowCord = cord / 10;
    int colCord = cord % 10;

    if (game.board[rowCord][colCord] < 4)
    {
        game.board[rowCord][colCord]++;
        return;
    }
}

//decreses given cord provided it is more than 0
void decCord(int cord)
{
    int rowCord = cord / 10;
    int colCord = cord % 10;

    if (game.board[rowCord][colCord] > 0)
    {
        game.board[rowCord][colCord]--;
        return;
    }
}

/*this method checks the octagonal around both Ai and Player current locations and stores in their respective adjacency list. 
    If the cordinate is either out of bounds of contains the other player, 
    it will not be included (put as -1) becasue it is irrelevant for increment/decrement purpouses
    */
void updateAdjList()
{
    //the adjacency list of the player is updated here
    if (isValidInput(game.playerPos + 10,0) && game.playerPos + 10 != game.aiPos)
    {
        game.pAdjList[0] = game.playerPos + 10;
    }
    else
    {
        game.pAdjList[0] = -1;
    }
    if (isValidInput(game.playerPos + 1,0) && game.playerPos + 1 != game.aiPos)
    {
        game.pAdjList[1] = game.playerPos + 1;
    }
    else
    {
        game.pAdjList[1] = -1;
    }
    if (isValidInput(game.playerPos + 11,0) && game.playerPos + 11 != game.aiPos)
    {
        game.pAdjList[2] = game.playerPos + 11;
    }
    else
    {
        game.pAdjList[2] = -1;
    }
    if (isValidInput(game.playerPos + 9,0) && game.playerPos + 9 != game.aiPos)
    {
        game.pAdjList[3] = game.playerPos+ 9;
    }
    else
    {
        game.pAdjList[3] = -1;
    }
    if (isValidInput(game.playerPos- 10,0) && game.playerPos- 10 != game.aiPos)
    {
        game.pAdjList[4] = game.playerPos- 10;
    }
    else
    {
        game.pAdjList[4] = -1;
    }
    if (isValidInput(game.playerPos- 1,0) && game.playerPos- 1 != game.aiPos)
    {
        game.pAdjList[5] = game.playerPos- 1;
    }
    else
    {
        game.pAdjList[5] = -1;
    }
    if (isValidInput(game.playerPos- 9,0) && game.playerPos- 9 != game.aiPos)
    {
        game.pAdjList[6] = game.playerPos- 9;
    }
    else
    {
        game.pAdjList[6] = -1;
    }
    if (isValidInput(game.playerPos- 11,0) && game.playerPos- 11 != game.aiPos)
    {
        game.pAdjList[7] = game.playerPos- 11;
    }
    else
    {
        game.pAdjList[7] = -1;
    }

    //the adjacency list of the Ai is updated here
    if (isValidInput(game.aiPos + 10,0) && game.aiPos + 10 != game.playerPos)
    {
        game.aiAdjList[0] = game.aiPos + 10;
    }
    else
    {
        game.aiAdjList[0] = -1;
    }
    if (isValidInput(game.aiPos + 1,0) && game.aiPos + 1 != game.playerPos)
    {
        game.aiAdjList[1] = game.aiPos + 1;
    }
    else
    {
        game.aiAdjList[1] = -1;
    }
    if (isValidInput(game.aiPos + 11,0) && game.aiPos + 11 != game.playerPos)
    {
        game.aiAdjList[2] = game.aiPos + 11;
    }
    else
    {
        game.aiAdjList[2] = -1;
    }
    if (isValidInput(game.aiPos + 9,0) && game.aiPos + 9 != game.playerPos)
    {
        game.aiAdjList[3] = game.aiPos + 9;
    }
    else
    {
        game.aiAdjList[3] = -1;
    }
    if (isValidInput(game.aiPos - 10,0) && game.aiPos - 10 != game.playerPos)
    {
        game.aiAdjList[4] = game.aiPos - 10;
    }
    else
    {
        game.aiAdjList[4] = -1;
    }
    if (isValidInput(game.aiPos - 1,0) && game.aiPos - 1 != game.playerPos)
    {
        game.aiAdjList[5] = game.aiPos - 1;
    }
    else
    {
        game.aiAdjList[5] = -1;
    }
    if (isValidInput(game.aiPos - 9,0) && game.aiPos - 9 != game.playerPos)
    {
        game.aiAdjList[6] = game.aiPos - 9;
    }
    else
    {
        game.aiAdjList[6] = -1;
    }
    if (isValidInput(game.aiPos - 11,0) && game.aiPos - 11 != game.playerPos)
    {
        game.aiAdjList[7] = game.aiPos - 11;
    }
    else
    {
        game.aiAdjList[7] = -1;
    }
}

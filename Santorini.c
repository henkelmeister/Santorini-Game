#include <stdio.h>
#include <stdbool.h>

int playerPos;
int aiPos;
bool playerWon;
bool aiWon;

void start(int board[7][7]);
bool isDone(int board[7][7]);
bool isValidInput(int cord,int testinput);
bool validPlayerMove(int num);
void printBoard(int board[7][7]);
void movePlayer(int move);
void moveAi(int move);
void insertAi();
void incOctagonal(int pAdjList[], int aiAdjList[], int board[7][7]);
void decOcatagonal(int pAdjList[], int aiAdjList[], int board[7][7]);
void doNextMoveAi();
void updateAdjList(int pAdjList[],int aiAdjList[]);
void decCord(int cord,int board[7][7]);
void incCord(int cord, int board[7][7]);

//Main function where execution of other fuctions occur.
void main()
{
    int board[7][7];
    int pAdjList[8];
    int aiAdjList[8]; 
    start(board);
    int moveAttempt;
    while (!isDone(board))
    {

        printf("\nmake your next move:");
        scanf("%d", &moveAttempt);
        printf("\n");
        if (validPlayerMove(moveAttempt))
        {
            movePlayer(moveAttempt);
            doNextMoveAi();
            updateAdjList(pAdjList, aiAdjList);
            incOctagonal(pAdjList, aiAdjList, board);
            decOcatagonal(pAdjList, aiAdjList,board);
            printBoard(board);
            printf("\nPosition updated\n");
        }
    }
    if (playerWon)
    {
        printf("\nThe player has won!!!\n");
    }
    if (aiWon)
    {
        printf("\nThe ai has won :(\n");
    }
}

//Initalizes the game start
void start(int board[7][7])
{
    playerWon = false;
    aiWon = false;
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
            board[i][j] = tempboard[i][j];
        }
    }
    //This is where the player selects their starting point
    printf("Welcome to the game\n\n");
    printf("choose a starting coodinate: (write in the format of \"14\" for coordinate row 1 column 4 ): ");
    bool temp = true;
    int testinput; 
    while (temp)
    {
        scanf("%d", &testinput);
        if (isValidInput(-100,testinput))
        {
            playerPos = testinput;
            printf("valid input, let the games begin...\n\n");
            insertAi();
            printBoard(board);
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
bool isDone(int board[7][7])
{
    int numZeros = 0;
    int numFours = 0;

    for (int i = 1; i < 7; i++)
    {
        for (int j = 1; j < 7; j++)
        {
            if (board[i][j] == 4)
            {
                numFours++;
            }
            if (board[i][j] == 0)
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
        aiWon = true;
        return true;
    }

    if (numFours >= 10)
    {
        playerWon = true;
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
        if (cord != playerPos && cord != aiPos)
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
void printBoard(int board[7][7])
{

    //Obtain the X and Y cordinates of the player and Ai
    int pRow = playerPos / 10;
    int pColumn = playerPos % 10;
    int aiRow = aiPos / 10;
    int aiColumn = aiPos % 10;

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
                printf("%d  ", board[i][j]);
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
    int pRow = playerPos / 10;
    int pColumn = playerPos % 10;
    int aiRow = aiPos / 10;
    int aiColumn = aiPos % 10;

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
        playerPos = move;
        return;
    }
}

//moves Ai to a new speciefied cord
void moveAi(int move)
{
    if (isValidInput(move,0))
    {
        aiPos = move;
        return;
    }
}

//inserts Ai in an open space next to player at start of game
void insertAi()
{
    if (isValidInput(playerPos + 1,0))
    {
        aiPos = playerPos + 1;
        return;
    }
    if (isValidInput(playerPos - 1,0))
    {
        aiPos = playerPos - 1;
        return;
    }
    if (isValidInput(playerPos - 10,0))
    {
        aiPos = playerPos - 10;
        return;
    }
    if (isValidInput(playerPos + 10,0))
    {
        aiPos = playerPos + 10;
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
        if (playerPos + 1 == num)
        {
            return true;
        }
        if (playerPos - 1 == num)
        {
            return true;
        }
        if (playerPos + 10 == num)
        {
            return true;
        }
        if (playerPos - 10 == num)
        {
            return true;
        }
        if (playerPos + 11 == num)
        {
            return true;
        }
        if (playerPos - 11 == num)
        {
            return true;
        }
        if (playerPos - 9 == num)
        {
            return true;
        }
        if (playerPos + 9 == num)
        {
            return true;
        }
    }
    printf("NOT VALID MOVE");
    return false;
}

//cross compares adjacency lists and increases cords that are not shared between lists
void incOctagonal(int pAdjList[], int aiAdjList[],int board[7][7])
{
    int rowPlayer = playerPos / 10;
    int columnPlayer = playerPos % 10;
    for (int i = 0; i < 8; i++)
    {
        bool match = false;
        for (int j = 0; j < 8; j++)
        {
            if (pAdjList[i] == aiAdjList[j])
            {
                match = true;
            }
        }
        if (match == true)
            continue;

        if (match == false)
        {
            incCord(pAdjList[i],board);
        }
    }
}

//cross compares adjacency lists and decreases cords that are not shared between lists
void decOcatagonal(int pAdjList[], int aiAdjList[], int board[7][7])
{
    for (int i = 0; i < 8; i++)
    {
        bool match = false;
        for (int j = 0; j < 8; j++)
        {
            if (aiAdjList[i] == pAdjList[j])
            {
                match = true;
            }
        }
        if (match == true)
            continue;

        if (match == false)
        {
            decCord(aiAdjList[i],board);
        }
    }
}
//increases given cord provided it is less than 4
void incCord(int cord,int board[7][7])
{
    int rowCord = cord / 10;
    int colCord = cord % 10;

    if (board[rowCord][colCord] < 4)
    {
        board[rowCord][colCord]++;
        return;
    }
}

//decreses given cord provided it is more than 0
void decCord(int cord,int board[7][7])
{
    int rowCord = cord / 10;
    int colCord = cord % 10;

    if (board[rowCord][colCord] > 0)
    {
        board[rowCord][colCord]--;
        return;
    }
}

/*this method checks the octagonal around both Ai and Player current locations and stores in their respective adjacency list. 
    If the cordinate is either out of bounds of contains the other player, 
    it will not be included (put as -1) becasue it is irrelevant for increment/decrement purpouses
    */
void updateAdjList(int pAdjList[], int aiAdjList[])
{
    //the adjacency list of the player is updated here
    if (isValidInput(playerPos + 10,0) && playerPos + 10 != aiPos)
    {
        pAdjList[0] = playerPos + 10;
    }
    else
    {
        pAdjList[0] = -1;
    }
    if (isValidInput(playerPos + 1,0) && playerPos + 1 != aiPos)
    {
        pAdjList[1] = playerPos + 1;
    }
    else
    {
        pAdjList[1] = -1;
    }
    if (isValidInput(playerPos + 11,0) && playerPos + 11 != aiPos)
    {
        pAdjList[2] = playerPos + 11;
    }
    else
    {
        pAdjList[2] = -1;
    }
    if (isValidInput(playerPos + 9,0) && playerPos + 9 != aiPos)
    {
        pAdjList[3] = playerPos + 9;
    }
    else
    {
        pAdjList[3] = -1;
    }
    if (isValidInput(playerPos - 10,0) && playerPos - 10 != aiPos)
    {
        pAdjList[4] = playerPos - 10;
    }
    else
    {
        pAdjList[4] = -1;
    }
    if (isValidInput(playerPos - 1,0) && playerPos - 1 != aiPos)
    {
        pAdjList[5] = playerPos - 1;
    }
    else
    {
        pAdjList[5] = -1;
    }
    if (isValidInput(playerPos - 9,0) && playerPos - 9 != aiPos)
    {
        pAdjList[6] = playerPos - 9;
    }
    else
    {
        pAdjList[6] = -1;
    }
    if (isValidInput(playerPos - 11,0) && playerPos - 11 != aiPos)
    {
        pAdjList[7] = playerPos - 11;
    }
    else
    {
        pAdjList[7] = -1;
    }

    //the adjacency list of the Ai is updated here
    if (isValidInput(aiPos + 10,0) && aiPos + 10 != playerPos)
    {
        aiAdjList[0] = aiPos + 10;
    }
    else
    {
        aiAdjList[0] = -1;
    }
    if (isValidInput(aiPos + 1,0) && aiPos + 1 != playerPos)
    {
        aiAdjList[1] = aiPos + 1;
    }
    else
    {
        aiAdjList[1] = -1;
    }
    if (isValidInput(aiPos + 11,0) && aiPos + 11 != playerPos)
    {
        aiAdjList[2] = aiPos + 11;
    }
    else
    {
        aiAdjList[2] = -1;
    }
    if (isValidInput(aiPos + 9,0) && aiPos + 9 != playerPos)
    {
        aiAdjList[3] = aiPos + 9;
    }
    else
    {
        aiAdjList[3] = -1;
    }
    if (isValidInput(aiPos - 10,0) && aiPos - 10 != playerPos)
    {
        aiAdjList[4] = aiPos - 10;
    }
    else
    {
        aiAdjList[4] = -1;
    }
    if (isValidInput(aiPos - 1,0) && aiPos - 1 != playerPos)
    {
        aiAdjList[5] = aiPos - 1;
    }
    else
    {
        aiAdjList[5] = -1;
    }
    if (isValidInput(aiPos - 9,0) && aiPos - 9 != playerPos)
    {
        aiAdjList[6] = aiPos - 9;
    }
    else
    {
        aiAdjList[6] = -1;
    }
    if (isValidInput(aiPos - 11,0) && aiPos - 11 != playerPos)
    {
        aiAdjList[7] = aiPos - 11;
    }
    else
    {
        aiAdjList[7] = -1;
    }
}

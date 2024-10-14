/**************************
Name: Hamd-Ul-Haq            Roll#: 23i-0081                  Assignment#: 2     
**************************/

/*Use classes for your solution. No marks will be awarded for code that does not use
object-oriented programming principles.*/

//Do not use STL, strings, arrays, or any built-in functions or libraries.
//For Windows, use PDCurses to display
//Create a 15 by 15 grid using 2d doubly linked list
//P for player, C for coin, K for key, D for door, B for bomb
//d=[x2-x1]+[y2-y1] manhattan distance

#include<iostream>
#include<cstdlib>
#include<ctime>
#include<ncurses.h>

using namespace std;

struct Node {
    int x;
    int y;
    char data;
    Node* next;
    Node* previous;
    Node* top;
    Node* bottom;

    Node(char data1) {
        data = data1;
        next = nullptr;
        previous = nullptr;
        top = nullptr;
        bottom = nullptr;
    }
};

class Game {
    private:
    int Rows;
    int Columns;
    int score;
    int moves;
    bool level1;
    bool level2;
    bool level3;
    int undoCount;

    public:
    Game() {
        level1 = false;
        level2 = true;
        level3 = false;
        score = 0;
    }

    int getRows() {
        if (level1 == true) {
            Rows = 10;
        } else if (level2 == true) {
            Rows = 15;
        } else if (level3 == true) {
            Rows = 20;
        }
        return Rows;
    }

    int getColumns() {
        if (level1 == true) {
            Columns = 10;
        } else if (level2 == true) {
            Columns = 15;
        } else if (level3 == true) {
            Columns = 20;
        }
        return Columns;
    }

    void setScore(int Score) {
        score = Score;
    }

    int getScore() {
        return score;
    }

    void setMoves(int Moves) {
        if(level1==true){
            undoCount=6;
            moves = Moves+6;
        }

        if(level2==true){
            undoCount=4;
            moves = Moves+2;
        }

        if(level3==true){
            undoCount=1;
            moves = Moves;
        }
    }

    void decreaseMoves(){
        if (moves > 0) {
            moves--;
        }
    }

    void decreaseUndoCount(){
        undoCount--;
        moves++;
    }

    void increaseUndoCount(){
        undoCount++;
    }

    int getMoves() {
        return moves;
    }

    int getUndoCount(){
        return undoCount;
    }
};

class Stack:public Game{

    private:

    Node* top;

    public:

    Stack(){
        top=nullptr;
    }

    void push(int x1,int y1){
        Node* newNode=new Node(' ');
        newNode->x=x1;
        newNode->y=y1;
        newNode->next=top;
        top=newNode;
    }

    Node* pop(){
        
        if(top==nullptr){
            printw("Can not move back further than this");
            return nullptr;
        }

        Node* temp=top;
        top=top->next;
        
        return temp;
    }
};

// Doubly Linked List Class
class Grid : public Game{
    private:
    Node* head;
    Node* tail;
    int rows;
    int cols;
    char coin;
    char player;
    char key;
    char bomb;
    char door;
    int playerX;
    int playerY;
    int keyX;
    int keyY;
    int horizontalDistance;
    int verticalDistance;
    int newHorizontalDistance;
    int newVerticalDistance;
    int distance;
    int sense;
    bool undo;
    Stack stack1;

    public:
    Grid() {
        head = nullptr;
        tail = nullptr;
        coin = 'C';
        player = 'P';
        key = 'K';
        door = 'D';
        bomb = 'B';
        playerX = -1;
        playerY = -1;
        keyX = -1;
        keyY = -1;
        rows = getRows();
        cols = getColumns();
        horizontalDistance = -1;
        verticalDistance = -1;
        newHorizontalDistance=-1;
        newVerticalDistance=-1;
        distance = -1;
        sense=-1;
        undo=false;
        stack1=Stack();
    }

    int calculateInitialDistance() {
        horizontalDistance = keyX - playerX;
        verticalDistance = keyY - playerY;

        if(horizontalDistance<0){
            horizontalDistance*=-1;
        }

        if(verticalDistance<0){
            verticalDistance*=-1;
        }

        int Distance = horizontalDistance + verticalDistance;
        setMoves(distance);

        return Distance;
    }

    void createGrid() {
        if (rows <= 0 || cols <= 0) {
            printw("Invalid number of rows and columns entered \n");
            return;
        }

        bool occupied[rows][cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                occupied[i][j] = false;
            }
        }

        srand(time(0));
        Node* grid[rows][cols];

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (i == 0 || j == 0 || i == rows - 1 || j == cols - 1) {
                    grid[i][j] = new Node('#');
                } else {
                    grid[i][j] = new Node('.');
                }
            }
        }

        int count = 0;
        while (count < 5) {
            int temp1, temp2;

            do {
                temp1 = rand() % (rows - 2) + 1;
                temp2 = rand() % (cols - 2) + 1;
            } while (occupied[temp1][temp2]);

            occupied[temp1][temp2] = true;

            if (count == 0) {
                grid[temp1][temp2]->data = player;
                playerX = temp2;
                playerY = temp1;
            } else if (count == 1) {
                grid[temp1][temp2]->data = key;
                keyX = temp2;
                keyY = temp1;
            } else if (count == 2) {
                grid[temp1][temp2]->data = bomb;
            } else if (count == 3) {
                grid[temp1][temp2]->data = coin;
            } else if (count == 4) {
                grid[temp1][temp2]->data = door;
            }

            count++;
        }

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (j < cols - 1) {
                    grid[i][j]->next = grid[i][j + 1];
                    grid[i][j + 1]->previous = grid[i][j];
                }
                if (i < rows - 1) {
                    grid[i][j]->bottom = grid[i + 1][j];
                    grid[i + 1][j]->top = grid[i][j];
                }
            }
        }

        head = grid[0][0];
        tail = grid[rows - 1][cols - 1];

        distance=calculateInitialDistance();
    }

    void movePlayer(char input) {
        int newX = playerX;
        int newY = playerY;

        
        if (input == 'w') {
            newY--;
        } 
        else if (input == 'a') {
            newX--;
        } 
        else if (input == 's') {
            newY++;
        } 
        else if (input == 'd') {
            newX++;
        }
        else if (input == 'u' && getUndoCount() > 0) {
            undo = true; 
        }

        if (newY >= 0 && newY < rows && newX >= 0 && newX < cols) {
            Node* current = head;
            
            
            for (int i = 0; i < playerY; i++) {
                current = current->bottom;
            }
            for (int j = 0; j < playerX; j++) {
                current = current->next;
            }

            if (!undo) {
                
                stack1.push(playerX, playerY);
                current->data = '.'; 
            }

            Node* nextNode = head;

            for (int i = 0; i < newY; i++) {
                nextNode = nextNode->bottom;
            }
            for (int j = 0; j < newX; j++) {
                nextNode = nextNode->next;
            }

            if(undo){
                Node* previousPosition = stack1.pop();
                if (previousPosition != nullptr) {
                    playerX = previousPosition->x;
                    playerY = previousPosition->y;

                    Node* restoredNode = head;
                    for (int i = 0; i < playerY; i++) {
                        restoredNode = restoredNode->bottom;
                    }
                    for (int j = 0; j < playerX; j++) {
                        restoredNode = restoredNode->next;
                    }

                    restoredNode->data = player;
                    nextNode->data='.';
                    decreaseUndoCount();
                }
                undo = false;
                return;
            }

            
            if (nextNode->data != '#') {
                playerX = newX; 
                playerY = newY;
                nextNode->data = player;
                decreaseMoves();
            } else {
                current->data = player; 
            }
        }

    
        newHorizontalDistance = keyX - playerX;
        newVerticalDistance = keyY - playerY;

        if (newVerticalDistance < 0) {
            newVerticalDistance *= -1;
        }

        if (newHorizontalDistance < 0) {
            newHorizontalDistance *= -1;
        }

        sense = newHorizontalDistance + newVerticalDistance;
        displayGrid();
    }

    void displayGrid() {
        clear();
        Node* row = head;

        printw("Player coordinates: (%d, %d)\n", playerX, playerY);
        printw("Key coordinates: (%d, %d)\n", keyX, keyY);

        printw("Moves: %d\n", getMoves());
        printw("Score: %d\n", getScore());


        printw("Hint: ");

        if(sense<distance){
            printw("Getting Closer\n");
        }
        
        if(sense>distance){
            printw("Moving Further Away\n");
        }

        if(sense==distance){
            printw("Keep Moving\n");
        }

        printw("\nGrid:\n");
        while (row != nullptr) {
            Node* column = row;
            while (column != nullptr) {
                printw("%c ", column->data);
                column = column->next;
            }
            printw("\n");
            row = row->bottom;
        }
        printw("\n");

        refresh();
    }

    ~Grid() {
        Node* row = head;
        while (row != nullptr) {
            Node* column = row;
            while (column != nullptr) {
                Node* tempColumn = column;
                column = column->next;
                delete tempColumn;
            }
            row = row->bottom;
        }
    }
};

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    Grid grid;
    Game game;
    grid.createGrid();
    grid.displayGrid();

    printw("Press 'q' to quit or any other key to continue...\n");

    int ch;
    while (true) {
        ch = getch();
        printw("You pressed: %c\n", ch);
        grid.movePlayer(ch);
        refresh();

        if (ch == 'q') {
            break;
        }
    }

    endwin();
    return 0;
}

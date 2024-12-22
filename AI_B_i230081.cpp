#include<iostream>
#include<cstdlib>
#include<ctime>
#include<ncurses.h>

using namespace std;

/**************************************
Global Variable for handling game state
**************************************/

bool gameStatus=true;


/*******************************************************
Node Structure: used for developing stack and linked list
*******************************************************/

struct Node {

    /********************************************
    Attributes: x and y represent coordinates
    for nodes, we use next,previous,top,bottom 
    for navigation and data is the representation
    of node in grid
    ********************************************/
    
    int x;
    int y;
    char data;
    Node* next;
    Node* previous;
    Node* top;
    Node* bottom;

    /*********************
    Initialization of node 
    *********************/

    Node(char data1){
        
        data=data1;
        next=nullptr;
        previous=nullptr;
        top=nullptr;
        bottom=nullptr;
    
    }
};



/************************************************************
Game Class: a key part of the game, it handles many variables
such as the level selected, score, moves, undo count etc
************************************************************/


class Game{
    
    private:

    /**************************************************
    Attributes: Rows and Columns are used to determine
    size of grid on selection of different levels,score
    and moves used to add information in the game, undo
    count used to indicate number of undos available,
    bool of levels used to indicate the current level
    **************************************************/
    
    int Rows;
    int Columns;
    int score;
    int moves;
    bool level1;
    bool level2;
    bool level3;
    int undoCount;

    public:

    /*********************
    Game Class Constructor
    *********************/

    Game(){
        level1=false;
        level2=true;
        level3=false;
        score=0;
    }

    /********************************************
    Getting Rows And Columns for different levels
    ********************************************/

    int getRows(){
        
        if(level1==true){
            Rows=10;
        } 
        
        else if(level2==true){
            Rows=15;
        } 
        
        else if(level3==true){
            Rows=20;
        }

        return Rows;
    }

    int getColumns(){
        
        if(level1==true){
            Columns=10;
        } 
        
        else if(level2==true){
            Columns=15;
        } 
        
        else if(level3==true){
            Columns=20;
        }

        return Columns;
    }

    /************************
    Setting the initial score
    ************************/

    void setScore(int Score){
        score=Score;
    }

    /************************
    Getting the current score
    ************************/

    int getScore(){
        return score;
    }

    /***************
    Increasing Score
    ***************/

    void increaseScore(){
        score++;
    }

    /*********************************
    Setting the initial moves and undo 
    count based on the level selected
    *********************************/


    void setMoves(int Moves){
        
        if(level1==true){
            undoCount=6;
            moves=Moves+6;
        }

        if(level2==true){
            undoCount=4;
            moves=Moves+2;
        }

        if(level3==true){
            undoCount=1;
            moves=Moves;
        }
    
    }

    /***************
    Decreasing Moves
    ***************/

    void decreaseMoves(){
        
        if(moves>0){
            moves--;
        }
    
    }

    /******************************
    Setting moves and undos to zero
    ******************************/

    void setMovesToZero(){
        moves=0;
    }

    void setUndosToZero(){
        undoCount=0;
    }

    /*******************************
    Increasing and decreasing moves
    and undo count, and also getting
    them with their current value
    *******************************/


    void increaseMoves(){
        moves++;
    }

    void decreaseUndoCount(){
        undoCount--;
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

/********************************************
Stack Class: Used for handling the undo logic
and storing the coins collected so far
********************************************/


class Stack:public Game{

    private:

    Node* top;

    public:

    Stack(){
        top=nullptr;
    }

    /******************************
    Push coordinates into the stack
    ******************************/

    void push(int x1,int y1){
        
        Node* newNode=new Node(' ');
        newNode->x=x1;
        newNode->y=y1;
        newNode->next=top;
        top=newNode;
    
    }


    /************************************
    Returns the last node pushed in stack
    ************************************/

    Node* pop(){
        
        if(top==nullptr){
            printw("Can not move back further than this");
            return nullptr;
        }

        Node* temp=top;
        top=top->next;
        
        return temp;
    }

    /******************************************
    Used for getting the last value in stack
    can be used to store stack in reverse order
    in a temporary stack
    ******************************************/

    Node* Peek(){
        if(top==nullptr){
            return nullptr;
        }

        return top;
    }

    /**********************
    Check if stack is empty
    **********************/

    bool isEmpty(){
        if(top==nullptr){
            return true;
        }

        return false;
    }

};


/***************************************************************
Grid Class: one of the most essential parts of the game, it is
responsible for handling the game grid, placement of items, how
items interact with each other and how the game responds to 
different game states
***************************************************************/


class Grid : public Game{
    private:
    
    /**************************************************************
    Attributes: head and tail used to indicate start and end of
    grid respectively, rows and cols for grid size, coin,player
    bomb,key and door chars to display these items on the grid,
    playerX,playerY,keyX,keyY,doorX,doorY,bombX,bombY,coinX,coinY 
    used to store X and Y coordinates of these items for different 
    interactions, distance and sense variables for calculation of
    manhattan distance,bool variables for key door to indicate if
    they are collected,undo variable to check if undo was pressed
    win,lost1,lost2 variables for handling game state for the
    different conditions, 2 stacks one for storing players previous
    coordinates for undo logic, and the other for storing the coins
    collected so far, an original grid for displaying the original 
    grid after game ends
    **************************************************************/


    
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
    int doorX;
    int doorY;
    int horizontalDistance;
    int verticalDistance;
    int newHorizontalDistance;
    int newVerticalDistance;
    int distance;
    int sense;
    bool undo;
    bool keyStatus;
    bool coinCollected;
    int numOfBombs;
    int numOfCoins;
    int *coinX;
    int *coinY;
    int *bombX;
    int *bombY;
    int distanceFromKey;
    int distanceFromDoor;
    Stack stack1;
    Stack stack2;
    bool win;
    bool lost1;
    bool lost2;
    int totalScore;
    char **originalGrid;

    public:

    /**********************************************
    Grid Constructor: used for intialization of all
    variables involved
    **********************************************/

    Grid(){
        
        head=nullptr;
        tail=nullptr;
        coin='C';
        player='P';
        key='.';
        door='.';
        bomb='.';
        playerX=-1;
        playerY=-1;
        keyX=-1;
        keyY=-1;
        doorX=-1;
        doorY=-1;
        rows=getRows();
        cols=getColumns();
        horizontalDistance=-1;
        verticalDistance=-1;
        newHorizontalDistance=-1;
        newVerticalDistance=-1;
        distance=-1;
        sense=-1;
        undo=false;
        keyStatus=false;
        win=false;
        lost1=false;
        lost2=false;
        stack1=Stack();
        stack2=Stack();
        numOfBombs=getRows()/5;
        numOfCoins=getRows()/5;
        coinCollected=false;
        coinX=new int[numOfCoins];
        coinY=new int[numOfCoins];
        bombX=new int[numOfBombs];
        bombY=new int[numOfBombs];
        totalScore=0;

        originalGrid=new char*[getRows()];
        
        for(int i=0;i<getRows();i++){
            originalGrid[i]=new char[getColumns()];
        }
    }

    /******************************************************
    Check if player has won and lost, and access allowed to 
    main to handle the game status smoothly
    ******************************************************/

    bool getWin(){
        return win;
    }

    bool getLost1(){
        return lost1;
    } 

    bool getLost2(){
        return lost2;
    }

    
    /******************************************
    Function for calculating manhattan distance
    ******************************************/

    int calculateInitialDistance(){

        /*****************************************
        Distance between player and key calculated
        *****************************************/

        if(keyX>=playerX){
            horizontalDistance=keyX-playerX;
        }
        
        if(keyY>=playerY){
            verticalDistance=keyY-playerY;
        }

        if(playerX>keyX){
            horizontalDistance=playerX-keyX;
        }
        
        if(playerY>keyY){
            verticalDistance=playerY-keyY;
        }

        distanceFromKey=horizontalDistance+verticalDistance;

        int horizontalDistance2;
        int verticalDistance2;

        int horizontalDistance3;
        int verticalDistance3;

        /*****************************************
        Distance between key and door calculated
        Distance between player and key calculated
        *****************************************/

        if(doorX>=keyX){
            horizontalDistance2=doorX-keyX;
        }
        
        if(doorY>=keyY){
            verticalDistance2=doorY-keyY;
        }

        if(keyX>doorX){
            horizontalDistance2=keyX-doorX;
        }

        if(keyY>doorY){
            verticalDistance2=keyY-doorY;
        }

        if(playerX>=doorX){
            horizontalDistance3=playerX-doorX;
        }

        if(doorX>playerX){
            horizontalDistance3=doorX-playerX;
        }

        if(playerY>=doorY){
            verticalDistance3=playerY-doorY;
        }

        if(doorY>playerY){
            verticalDistance3=doorY-playerY;
        }

        distanceFromDoor=horizontalDistance3+verticalDistance3;

        /**************************************
        Final distance calculated and moves set
        based on that distance
        **************************************/

        int Distance=(horizontalDistance+horizontalDistance2)+(verticalDistance+verticalDistance2);
        setMoves(Distance);

        return Distance;
    
    }


    /*************************
    Function for creating grid
    **************************/

    void createGrid(){

        /****************************************************
        Check for avoiding invalid number of rows and columns
        ****************************************************/

        if(rows<=0 || cols<=0){
            printw("Invalid number of rows and columns entered \n");
            return;
        }


        /****************************************************
        Calculating number of bombs and coins to be placed on
        grid, and intializing the occupied array, occupied 
        array has been used to avoid placing items on top of 
        each other
        ****************************************************/

        numOfBombs=rows/5;
        numOfCoins=rows/5;

        Node* bombs[numOfBombs];
        Node* coins[numOfCoins];

        bool occupied[rows][cols];
        
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                occupied[i][j]=false;
            }
        }


        /********************************
        Setting up random and intializing
        the grid data 
        ********************************/

        srand(time(0));
        Node* grid[rows][cols];

        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                
                if(i==0 || j==0 || i==rows-1 || j==cols-1){
                    grid[i][j]=new Node('#');
                }

                else{
                    grid[i][j]=new Node('.');
                }
            }
        }


        /****************************
        Placing items randomly on the 
        created grid
        ****************************/

        int count=0;
        
        while(count<3){
            int temp1,temp2;

            do{
                temp1=rand()%(rows-2)+1;
                temp2=rand()%(cols-2)+1;
            }while (occupied[temp1][temp2]);

            occupied[temp1][temp2]=true;

            if(count==0){
                
                grid[temp1][temp2]->data=player;
                playerX=temp2;
                playerY=temp1;
            
            } 
            
            else if(count==1){
                
                grid[temp1][temp2]->data=key;
                keyX=temp2;
                keyY=temp1;
            } 
            
            else if(count==2){
                
                grid[temp1][temp2]->data=door;
                doorX=temp2;
                doorY=temp1;
            
            }

            count++;
        }


        /************************************
        Placements of multiple bombs and keys
        ************************************/

        for(int i=0;i<numOfBombs;i++){
            int temp3,temp4;
            
            do{
                temp3=rand()%(rows-2)+1;
                temp4=rand()%(cols-2)+1;
            } while (occupied[temp3][temp4]);

            occupied[temp3][temp4]=true;
            grid[temp3][temp4]->data=bomb;
            bombs[i]=grid[temp3][temp4];
            bombX[i]=temp4;
            bombY[i]=temp3;
        
        }

        for(int i=0;i<numOfCoins;i++){
            int temp5,temp6;
            
            do{
                temp5=rand()%(rows-2)+1;
                temp6=rand()%(cols-2)+1;
            } while(occupied[temp5][temp6]);

            occupied[temp5][temp6]=true;
            grid[temp5][temp6]->data=coin;
            coins[i]=grid[temp5][temp6];
            coinX[i]=temp6;
            coinY[i]=temp5;
        }


        /***************************
        Linking all the nodes within
        the grid
        ***************************/

        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                
                if(j<cols-1){
                    
                    grid[i][j]->next=grid[i][j+1];
                    grid[i][j+1]->previous=grid[i][j];
                
                }
                
                if(i<rows-1){

                    grid[i][j]->bottom=grid[i+1][j];
                    grid[i+1][j]->top=grid[i][j];
                }
            }
        }



        /************************
        Storing the original grid
        ************************/

        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                originalGrid[i][j]=grid[i][j]->data;
            }
        }


        /*************************************
        Setting head and tail to start and end
        of the grid respectively
        *************************************/

        head=grid[0][0];
        tail=grid[rows-1][cols-1];



        /**********************
        Manhattan Distance Call
        **********************/

        distance=calculateInitialDistance();

    }


    /**********************************
    Function for displaying the game
    status when player escapes the maze
    **********************************/

    void gameWon(){


        /*************************
        Clear previous display for
        the new display
        **************************/
        
        clear();
        noecho();
        attron(COLOR_PAIR(2));
        attron(A_BOLD);

        printw("\n\n\n\t\t\t\t\t\tCongratulations You Have Escaped The Maze!");
        
        
        /****************************
        Temporary stack and coin node
        for reversing the stack that
        stored coin coordinates and 
        displaying coins in the order
        they were collected
        ****************************/
        
        Stack tempStack;
        Node* coinNode;

        /**************************
        Calculation of final score
        **************************/

        totalScore=getMoves()+getScore();



        /******************
        Reversing the stack
        ******************/
                
        while(stack2.isEmpty()==false){
            Node* coinNode=stack2.pop();
            tempStack.push(coinNode->x,coinNode->y);
        }


        /**************************
        Displaying coins collected
        **************************/

        int i=0;
        while(tempStack.isEmpty()==false){
            Node* coinNode2=tempStack.pop();
            printw("\n\nCoin %d's coordinates: (%d,%d,%c)",i+1,coinNode2->x,coinNode2->y,coin);
            i++;
        }

        printw("\n\nFinal Score: %d",totalScore);


        Node* row=head;
        key='K';
        door='D';
        bomb='B';


        /*******************************
        Displaying the initial grid
        with key,door and bombs revealed
        *******************************/

        printw("\n\t\t\t\t\t\tGrid:\n\n");
        for(int i=0;i<rows;i++){
            printw("\t\t\t\t\t\t");
            for(int j=0;j<cols;j++){
                if(i==keyY && j==keyX){
                    originalGrid[i][j]=key;
                }

                if(i==doorY && j==doorX){
                    originalGrid[i][j]=door;
                }
                for(int k=0;k<numOfBombs;k++){
                    if(i==bombY[k] && j==bombX[k]){
                        originalGrid[i][j]=bomb;
                    }
                }
                printw("%c ",originalGrid[i][j]);
            }
            printw("\n");

        }

        printw("\n");
        attroff(A_BOLD);
        attroff(COLOR_PAIR(1));

    }


    /**************************************
    Function for displaying the game
    when player activates a bomb and loses
    **************************************/

    
    void endGame(){

        /*************************
        Clear previous display for
        the new display
        **************************/
        
        clear();
        noecho();
        attron(COLOR_PAIR(1));
        attron(A_BOLD);
        printw("\n\n\n\t\t\t\t\t\tGame Over You Activated A Bomb!");


        /****************************
        Temporary stack and coin node
        for reversing the stack that
        stored coin coordinates and 
        displaying coins in the order
        they were collected
        ****************************/
        
        Stack tempStack;
        Node* coinNode;

        /**************************
        Calculation of final score
        **************************/

        totalScore=getScore();

        /******************
        Reversing the stack
        ******************/

        while(stack2.isEmpty()==false){
            Node* coinNode=stack2.pop();
            tempStack.push(coinNode->x,coinNode->y);
        }


        /**************************
        Displaying coins collected
        **************************/

        int i=0;
        while(tempStack.isEmpty()==false){
            Node* coinNode2=tempStack.pop();
            printw("\n\nCoin %d's coordinates: (%d,%d,%c)",i+1,coinNode2->x,coinNode2->y,coin);
            i++;
        }

        printw("\n\nFinal Score: %d",totalScore);

        
        Node* row=head;
        key='K';
        door='D';
        bomb='B';


        /*******************************
        Displaying the initial grid
        with key,door and bombs revealed
        *******************************/

        printw("\n\t\t\t\t\t\tGrid:\n\n");
        for(int i=0;i<rows;i++){
            printw("\t\t\t\t\t\t");
            for(int j=0;j<cols;j++){
                if(i==keyY && j==keyX){
                    originalGrid[i][j]=key;
                }

                if(i==doorY && j==doorX){
                    originalGrid[i][j]=door;
                }
                for(int k=0;k<numOfBombs;k++){
                    if(i==bombY[k] && j==bombX[k]){
                        originalGrid[i][j]=bomb;
                    }
                }
                printw("%c ",originalGrid[i][j]);
            }
            printw("\n");
            
        }
        printw("\n");
        attroff(A_BOLD);
        attroff(COLOR_PAIR(1));

    }

    /**************************************
    Function for displaying the game
    when player runs out of moves and loses
    **************************************/

    void endGame2(){

        /*************************
        Clear previous display for
        the new display
        **************************/

        clear();
        noecho();
        attron(COLOR_PAIR(1));
        attron(A_BOLD);
        printw("\n\n\n\t\t\t\t\t\tGame Over You Ran Out Of Moves!");


        /****************************
        Temporary stack and coin node
        for reversing the stack that
        stored coin coordinates and 
        displaying coins in the order
        they were collected
        ****************************/

        Stack tempStack;
        Node* coinNode;


        /**************************
        Calculation of final score
        **************************/

        totalScore=getScore();

        /******************
        Reversing the stack
        ******************/

                
        while(stack2.isEmpty()==false){
            Node* coinNode=stack2.pop();
            tempStack.push(coinNode->x,coinNode->y);
        }


        /**************************
        Displaying coins collected
        **************************/

        int i=0;
        while(tempStack.isEmpty()==false){
            Node* coinNode2=tempStack.pop();
            printw("\n\nCoin %d's coordinates: (%d,%d,%c)",i+1,coinNode2->x,coinNode2->y,coin);
            i++;
        }

        printw("\n\nFinal Score: %d",totalScore);

        Node* row=head;
        key='K';
        door='D';
        bomb='B';


        /*******************************
        Displaying the initial grid
        with key,door and bombs revealed
        *******************************/

        printw("\n\t\t\t\t\t\tGrid:\n\n");
        for(int i=0;i<rows;i++){
            printw("\t\t\t\t\t\t");
            for(int j=0;j<cols;j++){
                if(i==keyY && j==keyX){
                    originalGrid[i][j]=key;
                }

                if(i==doorY && j==doorX){
                    originalGrid[i][j]=door;
                }

                for(int k=0;k<numOfBombs;k++){
                    if(i==bombY[k] && j==bombX[k]){
                        originalGrid[i][j]=bomb;
                    }
                }
                printw("%c ",originalGrid[i][j]);
            }
            printw("\n");
            
        }
        printw("\n");
        attroff(A_BOLD);
        attroff(COLOR_PAIR(1));
    }


    /*******************************
    Function for checking if player
    is in the proximity of a bomb 
    and updating grid accordingly
    *******************************/

    void updateBombProximity(int playerX, int playerY){

        /***********************************
        Traverse the whole grid and reset it
        ***********************************/

        Node* current=head;
        
        for(int i=0;i<rows;i++){
            Node* rowStart=current;
            
            for(int j=0;j<cols;j++){
                
                if(current->data=='!'){
                    current->data='.';
                }

                current=current->next;
            }
            
            current=rowStart->bottom;  
        }

        int distanceFromBomb[numOfBombs];
        int horizontalBomb[numOfBombs];
        int verticalBomb[numOfBombs];

        for(int i=0;i<numOfBombs;i++){

            /*******************************
            Calculating distance between
            player and all the bombs
            *******************************/
            
            if(playerX>bombX[i]){
                horizontalBomb[i]=playerX-bombX[i];
            }

            if(playerY>bombY[i]){
                verticalBomb[i]=playerY-bombY[i];
            }

            if(bombX[i]>playerX){
                horizontalBomb[i]=bombX[i]-playerX;
            }

            if(bombY[i]>playerY){
                verticalBomb[i]=bombY[i]-playerY;
            }


            /*******************************
            Checking if player is in the
            bomb's activation proximity
            *******************************/

            if(bombX[i]!=-1 && bombY[i]!=-1){
                if(horizontalBomb[i]<=1 && verticalBomb[i]<=1){
                    for(int dx=0;dx<=1;dx++){
                        for(int dy=0;dy<=1;dy++){

                            int checkX=bombX[i]-dx;
                            int checkY=bombY[i]+dy;

                            if(checkX>=0 && checkX<cols && checkY>=0 && checkY<rows){
                                Node* warningNode=head;
                                
                                for(int y=0;y<checkY;y++){
                                    warningNode=warningNode->bottom;
                                }

                                for(int x=0;x<checkX;x++){
                                    warningNode=warningNode->next;
                                }
                            
                                if(warningNode->data=='.'){
                                    warningNode->data='!';
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    /***********************************
    Function for handling movement of
    player based on the different inputs
    given by user, one of the most 
    essential parts of the game
    ***********************************/

    void movePlayer(char input){
        
        int newX=playerX;
        int newY=playerY;


        /************************************
        Updating coordinates of player 
        according to key pressed and
        checking if undo key has been pressed
        ************************************/
    
        if(input=='w'){
            newY--;
        }

        else if(input=='a'){
            newX--;
        } 

        else if(input=='s' ){
            newY++;
        } 

        else if(input=='d'){
            newX++;
        }

        else if(input=='u' && getUndoCount()>0){
            undo=true; 
        }

        if(input=='u' && getUndoCount()==0){
            undo=false;
            increaseMoves();
        }

        if(getMoves()==0 && !undo){
            displayGrid();
            return;
        }


        if(newY>=0 && newY<rows && newX>=0 && newX<cols){
            
            
            /**************************
            Moving to previous position
            **************************/
            
            Node* current=head;

            for(int i=0;i<playerY;i++){
                current=current->bottom;
            }

            for(int j=0;j<playerX;j++){
                current=current->next;
            }

            if(!undo){


                /*******************************
                Storing previous position of 
                player in stack for future undos
                *******************************/
                
                stack1.push(playerX, playerY);
                current->data = '.'; 
            }


            /*******************************
            Moving to new position of player
            *******************************/

            Node* nextNode=head;

            for(int i=0;i<newY;i++){
                nextNode=nextNode->bottom;
            }

            for(int j=0;j<newX;j++){
                nextNode=nextNode->next;
            }


            /*****************
            Undo case handling
            *****************/

            if(undo){
                
                Node* previousPosition=stack1.pop();
                if (previousPosition!=nullptr){
                    playerX=previousPosition->x;
                    playerY=previousPosition->y;

                    Node* restoredNode=head;
                    
                    for(int i=0;i<playerY;i++){
                        restoredNode=restoredNode->bottom;
                    }

                    for (int j=0;j<playerX;j++){
                        restoredNode=restoredNode->next;
                    }

                    restoredNode->data=player;
                    nextNode->data='.';
                    decreaseUndoCount();
                    increaseMoves();
                }
                
                undo=false;
                displayGrid();
                return;
            }

                
            /****************
            Boundary Checking
            ****************/

            if (nextNode->data!='#'){
                playerX=newX; 
                playerY=newY;
                nextNode->data=player;
                decreaseMoves();
            } 
            
            else {
                current->data = player; 
            }


            /*******************************
            Checking if player has collected
            coins and storing the coordinates
            of coins in the stack
            *******************************/

            for(int i=0;i<numOfCoins;i++){
                
                if(playerX==coinX[i] && playerY==coinY[i]){
                    
                    increaseScore();
                    increaseScore();
                    increaseUndoCount();
                    displayGrid();
                    stack2.push(coinX[i],coinY[i]);
                    coinCollected=true;
                    coinX[i]=-1;
                    coinY[i]=-1;
                
                }
            
            }


            /*******************************
            Checking if player has activated
            bomb and updating game status
            *******************************/

            
            for(int i=0;i<numOfBombs;i++){
                if(playerX==bombX[i] && playerY==bombY[i]){
                    lost1=true;
                    gameStatus=false;
                    return;
                }
            }


            /*******************************
            Checking if player has collected
            the key
            *******************************/

            if(playerX==keyX && playerY==keyY){
                keyStatus=true;
            }


            /*******************************
            Checking if player has reached
            door and updating game status
            *******************************/

            if(playerX==doorX && playerY==doorY && keyStatus==true){
                win=true;
                gameStatus=false;
                return;
            }
        }


        /***********************************
        Checking if player has run out of
        moves,undos and updating game status
        ***********************************/

        if(getMoves()==0 && getUndoCount()==0){
            lost2=true;
            gameStatus=false;
            return;
        }


        /*********************************
        Calculating distance of player and
        key dynamically for hints
        *********************************/

        if(keyStatus==false){
            if(keyX>=playerX){
                newHorizontalDistance=keyX-playerX;
            }

            if(keyY>=playerY){
                newVerticalDistance=keyY-playerY;
            }

            if(playerX>keyX){
                newHorizontalDistance=playerX-keyX;
            }

            if(playerY>keyY){
                newVerticalDistance=playerY-keyY;
            }
        }


        /*********************************
        Calculating distance of player and
        door dynamically for hints
        *********************************/

        if(keyStatus==true){
           
            if(doorX>=playerX){
                newHorizontalDistance=doorX-playerX;
            }

            if(doorY>=playerY){
                newVerticalDistance=doorY-playerY;
            }

            if(playerX>doorX){
                newHorizontalDistance=playerX-doorX;
            }

            if(playerY>doorY){
                newVerticalDistance=playerY-doorY;
            }
        
        }


        /******************************************
        Calculating distance for hints,calling bomb
        proximity function to check if player is in
        proximity of bomb and calling display grid 
        function for displaying the updated grid
        ******************************************/

        sense=newHorizontalDistance+newVerticalDistance;
        updateBombProximity(playerX,playerY);
        displayGrid();
    
    }

    void displayGrid(){


        /************************************
        Clearing previous display for the new
        display
        ************************************/
        
        clear();
        Node* row=head;


        /*********************************
        Displaying Current Level Status
        *********************************/

        if(getRows()==10){
            attron(COLOR_PAIR(2));
            attron(A_BOLD);
            printw("\n\t\t\t\t\t\t\t      Level 1\n");
            attroff(COLOR_PAIR(2));
            attroff(A_BOLD);
        }

        if(getRows()==15){
            attron(COLOR_PAIR(2));
            attron(A_BOLD);
            printw("\n\t\t\t\t\t\t\t      Level 2\n");
            attroff(COLOR_PAIR(2));
            attroff(A_BOLD);
        }

        if(getRows()==20){
            attron(COLOR_PAIR(2));
            attron(A_BOLD);
            printw("\n\t\t\t\t\t\t\t      Level 3\n");
            attroff(COLOR_PAIR(2));
            attroff(A_BOLD);
        }


        /************************
        Displaying coin collected
        ************************/

        if(coinCollected==true){
            attron(COLOR_PAIR(2));
            attron(A_BOLD);
            printw("Coin Collected!\n");
            attroff(COLOR_PAIR(2));
            attroff(A_BOLD);
            coinCollected=false;
        }


        /***********************
        Displaying key collected
        ***********************/

        if(keyStatus==true){
            attron(COLOR_PAIR(2));
            attron(A_BOLD);
            printw("Key Collected!\n");
            attroff(COLOR_PAIR(2));
            attroff(A_BOLD);
        }


        /*********************************
        Displaying moves,undos remaining 
        and the key status
        *********************************/

        printw("\n\t\t\t\t\t\tMoves: %d", getMoves());
        printw("\t\tUndo Count: %d\n", getUndoCount());
        if(keyStatus==false){
            attron(COLOR_PAIR(1));
            attron(A_BOLD);
            printw("\n\t\t\t\t\t\tKey Status: False");
            attroff(COLOR_PAIR(1));
            attroff(A_BOLD);
        }

        if(keyStatus==true){
            attron(COLOR_PAIR(2));
            attron(A_BOLD);
            printw("\n\t\t\t\t\t\tKey Status: True");
            attroff(COLOR_PAIR(2));
            attroff(A_BOLD);
        }

        printw("\tScore: %d\n", getScore());


        /*******************
        Displaying Key Hints
        *******************/

        if(sense==distanceFromKey && keyStatus==false){
            attron(COLOR_PAIR(3));
            attron(A_BOLD);
            printw("\n\t\t\t\t\t\tHint: ");
            printw("No Progress\n");
            attron(COLOR_PAIR(3));
            attron(A_BOLD);
        }

        if(sense<distanceFromKey && keyStatus==false){
            attron(COLOR_PAIR(2));
            attron(A_BOLD);
            printw("\n\t\t\t\t\t\tHint: ");
            printw("Getting Closer To Key\n");
            attroff(COLOR_PAIR(2));
            attroff(A_BOLD);
        }
        
        if(sense>distanceFromKey && keyStatus==false){
            attron(COLOR_PAIR(1));
            attron(A_BOLD);
            printw("\n\t\t\t\t\t\tHint: ");
            printw("Moving Further Away From Key\n");
            attroff(COLOR_PAIR(1));
            attroff(A_BOLD);
        }


        /********************
        Displaying Door Hints
        ********************/

        if(sense==distanceFromDoor && keyStatus==true){
            attron(COLOR_PAIR(3));
            attron(A_BOLD);
            printw("\n\t\t\t\t\t\tHint: ");
            printw("No Progress\n");
            attroff(COLOR_PAIR(3));
            attroff(A_BOLD);
        }

        if(sense<distanceFromDoor && keyStatus==true){
            attron(COLOR_PAIR(2));
            attron(A_BOLD);
            printw("\n\t\t\t\t\t\tHint: ");
            printw("Getting Closer To Door\n");
            attroff(COLOR_PAIR(2));
            attroff(A_BOLD);
        }
        
        if(sense>distanceFromDoor && keyStatus==true){
            attron(COLOR_PAIR(1));
            attron(A_BOLD);
            printw("\n\t\t\t\t\t\tHint: ");
            printw("Moving Further Away From Door\n");
            attroff(COLOR_PAIR(1));
            attroff(A_BOLD);
        }



        /***********************
        Displaying the game grid
        ************************/
        
        attron(A_BOLD);
        printw("\n\t\t\t\t\t\tGrid:\n\n");
        attron(COLOR_PAIR(5));
        while (row != nullptr) {
            Node* column = row;
            printw("\t\t\t\t\t\t");
            while (column != nullptr) {
                printw("%c ",column->data);
                column=column->next;
            }
            printw("\n");
            row=row->bottom;
        }
        printw("\n");
        attroff(COLOR_PAIR(5));
        attroff(A_BOLD);

        refresh();
    }


    /***********************************
    Destructor for avoiding memory leaks
    ***********************************/

    ~Grid(){
        
        Node* row=head;
        while(row!=nullptr){
            Node* column=row;
            
            while (column!=nullptr){
                Node* tempColumn=column;
                column=column->next;
                delete tempColumn;
            }
            
            row=row->bottom;
        }

        delete[] coinX;
        delete[] coinY;
        delete[] bombX;
        delete[] bombY;
    }
};


/***********************************
Main Function: Used for initializing
ncurses and for styling purposes,used
to control flow of game and handle 
user input
***********************************/

int main(){
    
    
    /******************
    Intializing ncurses
    ******************/
    
    initscr();
    start_color();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    init_pair(1,COLOR_RED,COLOR_BLACK);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);
    init_pair(3,COLOR_YELLOW,COLOR_BLACK);
    init_pair(4,COLOR_BLUE,COLOR_BLACK);
    init_pair(5,COLOR_MAGENTA,COLOR_BLACK);

    Grid grid;
    Game game;
    grid.createGrid();
    grid.displayGrid();

    bool Win=grid.getWin();
    bool Lost1=grid.getLost1();
    bool Lost2=grid.getLost2();

    printw("Press 'q' to quit or any other key to continue...\n");

    int ch;


    /***********************************
    Loop and continue to take input from
    user until game ends
    ***********************************/
    
    while (gameStatus==true){
        ch=getch();
        printw("You pressed: %c\n",ch);
        grid.movePlayer(ch);
        refresh();

        if(ch=='q'){
            break;
        }
    }


    /***********************************
    Displaying game based on the updated
    game status
    ***********************************/

    if(grid.getWin()==true){
            grid.gameWon();
            refresh();
    }

    if(grid.getLost1()==true){
            grid.endGame();
            refresh();
    }

    if(grid.getLost2()==true){
            grid.endGame2();
            refresh();
    }
    
    return 0;
}

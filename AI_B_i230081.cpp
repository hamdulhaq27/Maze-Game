/****************************************************************************
Name: Hamd-Ul-Haq            Roll#: 23i-0081                  Assignment#: 2     
****************************************************************************/

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

struct Node{

    char data;
    Node* next;
    Node* previous;
    Node* top;
    Node* bottom;

    Node(char data1){
        data=data1;
        next=nullptr;
        previous=nullptr;
        top=nullptr;
        bottom=nullptr;
    }

};

class Game{

    private:

    int Rows;
    int Columns;
    bool level1;
    bool level2;
    bool level3;

    public:

    Game(){
        level1=true;
        level2=false;
        level3=false;
    }

    int getRows(){
        
        if(level1==true){
            Rows=10;
        }

        if(level2==true){
            Rows=15;
        }

        if(level3==true){
            Rows=20;
        }

        return Rows;

    }

    int getColumns(){
        
        if(level1==true){
            Columns=10;
        }

        if(level2==true){
            Columns=15;
        }

        if(level3==true){
            Columns=20;
        }

        return Columns;

    }

};

// Doubly Linked List Class
class Grid:public Game{

    private:
    
    Node* head;
    Node* tail;
    int randomRow[5];
    int randomCol[5];
    int rows;
    int cols;
    char coin;
    char player;
    char key;
    char bomb;
    char door;
    int playerX;
    int playerY;
    int coinX;
    int coinY;
    int doorX;
    int doorY;
    int bombX;
    int bombY;
    int keyX;
    int keyY;
    char keyPressed;


    public:

    Grid(){
        head=nullptr;
        tail=nullptr;
        coin='C';
        player='P';
        key='K';
        door='D';
        bomb='B';
        playerX=-1;
        playerY=-1;
        bombX=-1;
        bombY=-1;
        doorX=-1;
        doorY=-1;
        keyX=-1;
        keyY=-1;
        coinX=-1;
        coinY=-1;
        rows=getRows();
        cols=getColumns();
    }

   void createGrid(){

        if(rows<=0 || cols<=0){
            printw("Invalid number of rows and columns entered \n");
            return;
        }

        bool occupied[rows][cols];
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                occupied[i][j]=false;
            }
        } 

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

        int count=0;
        while(count<5){
            int temp1,temp2;

            do{
                temp1=rand()%(rows-2)+1;
                temp2=rand()%(cols-2)+1;
            } while(occupied[temp1][temp2]);

            occupied[temp1][temp2]=true;

            if(count==0){
                grid[temp1][temp2]->data=player;
                playerX=temp1;
                playerY=temp2;
            } 
            
            else if(count==1){
                grid[temp1][temp2]->data=key;
                keyX=temp1;
                keyY=temp2;
            } 
            
            else if(count==2){
                grid[temp1][temp2]->data=bomb;
                bombX=temp1;
                bombY=temp2;
            } 
            
            else if(count==3){
                grid[temp1][temp2]->data=coin;
                coinX=temp1;
                coinY=temp2;
            } 
            
            else if (count==4){
                grid[temp1][temp2]->data=door;
                doorX=temp1;
                doorY=temp2;
            }

            count++;
        }

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

        head=grid[0][0];
        tail=grid[rows-1][cols-1];

        displayCoordinates();
    }


    void displayCoordinates() {
        printw("Player coordinates: (%d, %d)\n",playerX,playerY);
        printw("Key coordinates: (%d, %d)\n", keyX, keyY);
        printw("Bomb coordinates: (%d, %d)\n", bombX,bombY);
        printw("Coin coordinates: (%d, %d)\n", coinX,coinY);
        printw("Door coordinates: (%d, %d)\n", doorX, doorY);
    }

    void movePlayer(char input){
 
        int newX=playerX;
        int newY=playerY;

        switch (input){
            case 'w':
                newX--;
                break;
            case 'a':
                newY--;
                break;
            case 's':
                newX++;
                break;
            case 'd':
                newY++;
                break;
            default:
                return; 
        }

        
        if(newX>=0 && newX<rows && newY>=0 && newY<cols){

            Node* current=head;

            for(int i=0;i<playerY;i++){
                current=current->bottom;
            }

            for(int j=0;j<playerY;j++){
                current = current->next;
            }

            current->data='.'; 
            Node* nextNode=head;

            for(int i=0;i<newX;i++){
                nextNode=nextNode->bottom;
            }

            for(int j=0;j<newY;j++){
                nextNode=nextNode->next;
            }

            if(nextNode->data!='#'){
                playerX=newX;
                playerY=newY;
                nextNode->data=player;

                //displayCoordinates();
            } 
            else{
                current->data=player;
            }
        }

        displayGrid();
    }


    void displayGrid() {
        clear();
        Node* row = head;
        
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
    }

};

// class Queue{

//     private:

//     Node* front;
//     Node* rear;
//     int count;

//     public:

//     Queue(){
//         front=nullptr;
//         rear=nullptr;
//         count=0;
//     }

//     void Enqueue(string data1){

//         if(isFull()){
//             cout<<"Queue is full!"<<endl;
//             return;
//         }
        
//         cout<<data1<<" added to queue successfully!"<<endl;
//         Node* newNode=new Node(data1);

//         if(front==nullptr && rear==nullptr){
//             front=newNode;
//             rear=newNode;
//             count++;
//             return;
//         }

//         rear->next=newNode;
//         rear=rear->next;
//         count++;

//     }

//     void Dequeue(){

//         if(isEmpty()){
//             cout<<"Queue is empty!"<<endl;
//             return;
//         }

//         cout<<front->data<<" removed from queue successfully!"<<endl;
//         Node* temp=front;
//         front=front->next;
//         delete temp;
//         count--;

//     }

//     bool isEmpty(){

//         if(front==nullptr && rear==nullptr || count==0){
//             return true;
//         }

//         else{
//             return false;
//         }

//     }

//     bool isFull(){
        
//         if(count==5){
//             return true;
//         }

//         else{
//             return false;
//         }
//     }

//     void makeNull(){

//         if(isEmpty()){
//             cout<<"Queue is already empty!"<<endl;
//             return;
//         }

        
//         while(front!=nullptr){
//             Node*temp=front;
//             front=front->next;
//             delete temp;
//         }

//         return;

//     }
// };

int main() {
    initscr();          
    noecho();           
    cbreak();          
    keypad(stdscr, TRUE);

    Grid grid;
    Game game;
    grid.createGrid();
    grid.displayGrid();

    printw("Hello");

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

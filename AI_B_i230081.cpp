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
//#include<curses.h>

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

// Doubly Linked List Class
class Grid{

    private:
    
    Node* head;
    Node* tail;
    int rows;
    int cols;
    int randomRow[5];
    int randomCol[5];
    char coin;
    char player;
    char key;
    char bomb;
    char door;
    int playerCoordinates[2];
    int keyCoordinates[2];
    int bombCoordinates[2];
    int coinCoordinates[2];
    int doorCoordinates[2];

    public:

    Grid(){
        head=nullptr;
        tail=nullptr;
        rows=15;
        cols=15;
        coin='C';
        player='P';
        key='K';
        door='D';
        bomb='B';
    }

    void createGrid(){
       
        if(rows<=0 || cols<=0){
            cout<<"Invalid number of rows and columns entered"<<endl;
            return;
        }

        srand(time(0));
        int temp1;
        int temp2;
        int count=0;
        while(count<5){
            while(randomRow[count]!=temp1 && randomCol[count]!=temp2){
                randomRow[count]=rand()%(rows-2)+1;
                randomCol[count]=rand()%(cols-2)+2;
                temp1=randomRow[count];
                temp2=randomCol[count];
            }
            count++;
        }
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
        
        grid[randomRow[0]][randomCol[0]]->data=player;
        
        playerCoordinates[0]=randomRow[0];
        playerCoordinates[1]=randomCol[0];
        
        grid[randomRow[1]][randomCol[1]]->data=key;
        
        keyCoordinates[0]=randomRow[1];
        keyCoordinates[1]=randomCol[1];
        
        grid[randomRow[2]][randomCol[2]]->data=bomb;
        
        bombCoordinates[0]=randomRow[2];
        bombCoordinates[1]=randomCol[2];
        
        grid[randomRow[3]][randomCol[3]]->data=coin;
        
        coinCoordinates[0]=randomRow[3];
        coinCoordinates[1]=randomCol[3];
        
        grid[randomRow[4]][randomCol[4]]->data=door;
        
        doorCoordinates[0]=randomRow[4];
        doorCoordinates[1]=randomCol[4];

        cout<<"Player coordinates: "<<"("<<playerCoordinates[0]<<","<<playerCoordinates[1]<<")"<<endl;
        cout<<"Key coordinates: "<<"("<<keyCoordinates[0]<<","<<keyCoordinates[1]<<")"<<endl;
        cout<<"Bomb coordinates: "<<"("<<bombCoordinates[0]<<","<<bombCoordinates[1]<<")"<<endl;
        cout<<"Coin coordinates: "<<"("<<coinCoordinates[0]<<","<<coinCoordinates[1]<<")"<<endl;
        cout<<"Door coordinates: "<<"("<<doorCoordinates[0]<<","<<doorCoordinates[1]<<")"<<endl;

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
    
    }

    void displayGrid(){
        Node* row=head;
        
        cout<<"\nGrid:\n"<<endl;
        while(row!=nullptr){
            Node* column=row;
            
            while(column!=nullptr){
                cout<<column->data<<" ";
                column=column->next;
            }
            
            cout<<endl;
            row=row->bottom;
        }

        cout<<endl;
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

int main()
{
    Grid grid;
    grid.createGrid();
    grid.displayGrid();
    
    return 0;
}
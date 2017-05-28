//File "snake.h"

#pragma once

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <string>
#include "console.h"

using namespace std;

const char STUDENT_ID[] = { '1', '6', '1', '2', '2', '2', '4' };
const int FOOD_NUM = 4;
const int MAX_LEVEL = 5;
const int MIN_SPEED = 150;
enum DIRECTION{UP, RIGHT, DOWN, LEFT};

struct Point { //2-Dimensional point
	int x; //Vertical
	int y; //Horizontial
};

struct Cell { //Cell on map
	char symbol;
	Point position;
};

struct LinkedList {
	Cell data;
	LinkedList* nextNode;
};

struct Snake {
	LinkedList* head; //Snake's head
	DIRECTION status; //Snake direction
	int length; //Snake's length
};

struct FoodList {
	Cell food[FOOD_NUM];
	int foodIndex;
};

struct Gate {
	Cell wall[5];
	DIRECTION gateDir;
};

//Global variables
extern Snake snake;
extern Gate gate;
extern FoodList food;
extern bool gateCreated;
extern bool moveLock;
extern bool isAlive;
extern bool dirChanged;
extern bool mapDrawn;
extern bool foodDrawn;
extern bool gateDrawn;
extern int gameLevel;
extern int gameSpeed;
extern int gameScore;
extern Cell oldTail;
extern LinkedList* tempHead;

void addHead(LinkedList*& head, Cell data); //Add node to snake head with cell's data

void initSnake(Snake& snake, int size, DIRECTION x, int startX = 5, int startY = 3); //Create snake

bool removeHead(LinkedList*& head); //Remove snake head

bool removeSnake(Snake& snake); //Remove whole snake

void drawOneCell(const Cell& data, int color); //Draw a cell based on color

void drawSnake(const Snake& snake, int color); //Draw whole snake

void swapPos(Point& a, Point& b); //Swap position

LinkedList* findTail(const Snake& snake); //Find snake tail's node

void moveSnake(Snake& snake); //Move snake based on current direction

void drawMap(int color); //Drawn map based on color

void removeGate(Gate& gate); //Remove current gate

bool hitWall(const Cell& c, const Gate& gate); //Check if a cell hit wall (map wall or gate wall) or not

bool hitSnakeHead(const Cell& c, const Snake& snake); //Check of a cell hit snake head

bool hitSnakeBody(const Cell& c, const Snake& snake); //Check if a cell hit snake body

void initFood(FoodList& food, const Snake& snake, const Gate& gate); //Create a food list

void drawFood(const FoodList& food, int color); //Draw current food based on color

bool foodEaten(FoodList& food, const Snake& snake); //Check if current food is eaten or not

void addSnake(Snake& snake, const Cell& oldTail); //Add node to snake tail

bool freeCell(const Cell& c, const Snake& snake, const Gate& gate); //Check if a cell is free

//Special effect when snake hits wall or hits itself
void hitEffect(const Snake& snake, const Gate& gate, int color); 

void initGate(Gate& gate, const Snake& snake); //Create a gate with random direction

void drawGate(const Gate& gate, int color); //Draw gate based on color

//Save game data to file
bool writeToFile(char* filename, const Snake& snake, const Gate& gate, const FoodList& food);

//Load game data from file
bool readFromFile(char* filename, Snake& snake, Gate& gate, FoodList& food);

//Reset game data
void resetData(Snake& snake, Gate& gate, FoodList& food);

//Update game level
void updateLevel(Snake& snake);

//Draw game components
void drawGame(const Snake& snake, const Gate& gate, const FoodList& food, int color);

//Main game thread
void gameThread();

//Print initial menu choice
void printMenu(int choice = 0);

//Fake processing bar
void processingBar(int x, int y);

//Save game process
void saveGame();

//Load game process
void loadGame();

//End game process
void endGame();


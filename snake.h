//File "snake.h"

#pragma once

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <thread>
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
extern Cell oldTail;
extern LinkedList* tempHead;

void addHead(LinkedList*& head, Cell data);

void initSnake(Snake& snake, int size, DIRECTION x, int startX = 5, int startY = 3);

bool removeHead(LinkedList*& head);

bool removeSnake(Snake& snake);

void drawOneCell(const Cell& data, int color);

void drawSnake(const Snake& snake, int color);

void swapPos(Point& a, Point& b);

LinkedList* findTail(const Snake& snake);

void moveSnake(Snake& snake);

void drawMap(int color);

void removeGate(Gate& gate);

bool hitWall(const Cell& c, const Gate& gate);

bool hitSnakeHead(const Cell& c, const Snake& snake);

bool hitSnakeBody(const Cell& c, const Snake& snake);

void initFood(FoodList& food, const Snake& snake, const Gate& gate);

void drawFood(const FoodList& food, int color);

bool foodEaten(FoodList& food, const Snake& snake);

void addSnake(Snake& snake, const Cell& oldTail);

bool freeCell(const Cell& c, const Snake& snake, const Gate& gate);

void hitEffect(const Snake& snake, const Gate& gate, int color);

void initGate(Gate& gate, const Snake& snake);

void drawGate(const Gate& gate, int color);

bool writeToFile(char* filename, const Snake& snake, const Gate& gate, const FoodList& food);

bool readFromFile(char* filename, Snake& snake, Gate& gate, FoodList& food);

void resetData(Snake& snake, Gate& gate, FoodList& food);

void updateLevel(Snake& snake);

void drawGame(const Snake& snake, const Gate& gate, const FoodList& food, int color);

void gameThread();

void printMenu(int choice = 0);

void saveGame();

void loadGame();


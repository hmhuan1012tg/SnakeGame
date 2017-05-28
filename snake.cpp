//File "snake.cpp"

#include "snake.h"

//Global variables
Snake snake;
Gate gate;
FoodList food;
bool gateCreated = false;
bool moveLock = false;
bool isAlive = false;
bool dirChanged = true;
bool mapDrawn = false;
bool foodDrawn = false;
bool gateDrawn = false;
int gameLevel = 0;
int gameSpeed = MIN_SPEED;
int gameScore = 0;
Cell oldTail;
LinkedList* tempHead;

void addHead(LinkedList*& head, Cell data) {
	LinkedList* temp = new LinkedList{ data, head };
	head = temp;
}

void initSnake(Snake& snake, int size, DIRECTION x, int startX, int startY) {
	snake.length = size;
	snake.status = x;
	snake.head = NULL;
	for (int i = 0; i < size; i++) {
		Cell temp;
		temp.symbol = STUDENT_ID[(size - i - 1) % 7];
		temp.position.x = i + startX; // snake tail starts at [5,3]
		temp.position.y = startY;
		addHead(snake.head, temp);
	}
	tempHead = snake.head;
	oldTail = findTail(snake)->data;
	isAlive = true;
	moveLock = false;
}

bool removeHead(LinkedList*& head) { //Remove snake head
	if (head == NULL)
		return false;
	LinkedList* temp = head;
	head = head->nextNode;
	delete temp;
	temp = NULL;
	return true;
}

bool removeSnake(Snake& snake) { //Remove whole snake
	snake.head = tempHead;
	while (snake.head != NULL) {
		removeHead(snake.head);
	}
	tempHead = snake.head;
	isAlive = false;
	return true;
}

void drawOneCell(const Cell& data, int color) { //Draw single cell to console screen
	goToXY(data.position.x, data.position.y); //Go to cell's position
	setColor(color); //Change console color
	cout << data.symbol; //Print out symbol
}

void drawSnake(const Snake& snake, int color) { //Draw snake
	LinkedList* iter = snake.head;
	for (; iter != NULL; iter = iter->nextNode)
		drawOneCell(iter->data, color);
}

void swapPos(Point& a, Point& b) { //Swap 2 cells' position
	Point temp = a;
	a = b;
	b = temp;
}

LinkedList* findTail(const Snake& snake) { //Find && return pointer to snake tail
	LinkedList* temp = snake.head;
	for (; temp != NULL; temp = temp->nextNode)
		if (temp->nextNode == NULL)
			break;
	return temp;
}

void moveSnake(Snake& snake) { 
	if (snake.head == NULL)
		return;
	Point temp = snake.head->data.position;
	switch (snake.status) { //Change snake head's position
	case UP:
		temp.y = (MAP_HEIGHT + (temp.y - 1)) % MAP_HEIGHT;
		break;
	case RIGHT:
		temp.x = (temp.x + 1) % MAP_WIDTH;
		break;
	case DOWN:
		temp.y = (temp.y + 1) % MAP_HEIGHT;
		break;
	case LEFT:
		temp.x = (MAP_WIDTH + (temp.x - 1)) % MAP_WIDTH;
		break;
	}
	for (LinkedList* iter = snake.head; iter != NULL; iter = iter->nextNode) {
		swapPos(iter->data.position, temp);
	}
}

void drawMap(int color) { //Draw map to screen
	setColor(color);
	for (int i = 0; i < MAP_WIDTH; i++) {
		goToXY(i, 0);
		cout << '*';
	}
	for (int i = 0; i < MAP_HEIGHT; i++) {
		goToXY(0, i);
		cout << '*';
		goToXY(MAP_WIDTH - 1, i);
		cout << '*';
	}
	for (int i = 0; i < MAP_WIDTH; i++) {
		goToXY(i, MAP_HEIGHT - 1);
		cout << '*';
	}

	mapDrawn = color != ColorCode_Black;
}

void removeGate(Gate& gate) { //Set initial gate
	gate.gateDir = UP;
	for (int i = 0; i < 5; i++) {
		gate.wall[i].position.x = 0;
		gate.wall[i].position.y = 0;
	}
	gateCreated = false;
}

bool hitWall(const Cell& c, const Gate& gate) { //Check if a cell hits the wall
	if (c.position.x == 0 || c.position.x == MAP_WIDTH - 1
		|| c.position.y == 0 || c.position.y == MAP_HEIGHT - 1)
		return true;
	for (int i = 0; i < 5; i++)
		if (gate.wall[i].position.x == c.position.x
			&& gate.wall[i].position.y == c.position.y)
			return true;
	return false;
}

bool hitSnakeHead(const Cell& c, const Snake& snake) { //Check if a cell hits snake's head
	if (snake.head->data.position.x == c.position.x
		&& snake.head->data.position.y == c.position.y)
		return true;
	return false;
}

bool hitSnakeBody(const Cell& c, const Snake& snake) { //Check if a cell hits snake's body
	LinkedList* iter = snake.head->nextNode;
	for (; iter != NULL; iter = iter->nextNode)
		if (iter->data.position.x == c.position.x
			&& iter->data.position.y == c.position.y)
			return true;
	return false;
}

void initFood(FoodList& food, const Snake& snake, const Gate& gate) { //Create a food list
	food.foodIndex = 0;
	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < FOOD_NUM; i++) {
		food.food[i].symbol = '#';
		food.food[i].position.x = rand() % (MAP_WIDTH - 2) + 1;
		food.food[i].position.y = rand() % (MAP_HEIGHT - 2) + 1;
		while (hitSnakeHead(food.food[i], snake) || hitSnakeBody(food.food[i], snake)) {
			food.food[i].position.x = rand() % (MAP_WIDTH - 2) + 1;
			food.food[i].position.y = rand() % (MAP_HEIGHT - 2) + 1;
		}
	}
	foodDrawn = false;
}

void drawFood(const FoodList& food, int color) { //Draw food to screen
	if (food.foodIndex >= 4)
		return;
	drawOneCell(food.food[food.foodIndex], color);
	if (hitSnakeHead(food.food[food.foodIndex], snake)
	 || hitSnakeBody(food.food[food.foodIndex], snake))
		foodDrawn = false;
	else
		foodDrawn = color != ColorCode_Black;
}

bool foodEaten(FoodList& food, const Snake& snake) { //check if snake eats food
	if (!hitSnakeHead(food.food[food.foodIndex], snake))
		return false;
	food.foodIndex++;
	gameScore++;
	foodDrawn = false;
	return true;
}

void addSnake(Snake& snake, const Cell& oldTail) { //Add new node to snake to its tail
	snake.length++; //Increase snake length

	LinkedList* newNode = new LinkedList; //Create new node

	newNode->data.symbol = STUDENT_ID[(snake.length - 1) % 7]; //Set new node symbol
	newNode->data.position = oldTail.position; //Set new node position the same as old tail
	newNode->nextNode = NULL;
	findTail(snake)->nextNode = newNode; //Connect new node to snake tail
}

bool freeCell(const Cell& c, const Snake& snake, const Gate& gate) {
	return !hitWall(c, gate) && !hitSnakeBody(c, snake) && !hitSnakeHead(c, snake)
		&& c.position.x > 0 && c.position.y > 0
		&& c.position.x < MAP_WIDTH
		&& c.position.y < MAP_HEIGHT;
}

void hitEffect(const Snake& snake, const Gate& gate, int color) { //Special effects when snake hits wall or hits itself
	Cell temp;
	temp.symbol = '#';
	int tempX = snake.head->data.position.x;
	int tempY = snake.head->data.position.y;
	for (int i = 1; i <= 4; i++) {
		for (int j = tempX - i; j <= tempX + i; j++) {
			temp.position.x = j;
			temp.position.y = tempY - i;
			if (freeCell(temp, snake, gate))
				drawOneCell(temp, color);
		}
		for (int j = tempY - i + 1; j < tempY + i; j++) {
			temp.position.x = tempX - i;
			temp.position.y = j;
			if (freeCell(temp, snake, gate))
				drawOneCell(temp, color);
			temp.position.x = tempX + i;
			temp.position.y = j;
			if (freeCell(temp, snake, gate))
				drawOneCell(temp, color);
		}
		for (int j = tempX - i; j <= tempX + i; j++) {
			temp.position.x = j;
			temp.position.y = tempY + i;
			if (freeCell(temp, snake, gate))
				drawOneCell(temp, color);
		}
		Sleep(100);
	}
}

void initGate(Gate& gate, const Snake& snake) {
	Cell randomCell[6];
	for (int i = 0; i < 6; i++) {
		randomCell[i].symbol = '*';
		randomCell[i].position.x = 0;
		randomCell[i].position.y = 0;
	}
	srand(static_cast<unsigned int>(time(0)));
	gate.gateDir = static_cast<DIRECTION>(rand() % 4);
	bool isOK = true;
	int tempX, tempY;
	switch (gate.gateDir) {
	case UP:
		do {
			tempX = rand() % (MAP_WIDTH - 2) + 1;
			tempY = rand() % (MAP_HEIGHT - 4) + 2;
			int k = 0;
			isOK = true;
			for(int j = 0; j < 2 && isOK; j++)
				for (int i = 0; i < 3; i++) {
					randomCell[k].position.x = tempX + i;
					randomCell[k].position.y = tempY + j;
					isOK = freeCell(randomCell[k], snake, gate);
					k++;
					if (!isOK)
						break;
				}
		} while (!isOK);
		break;
	case DOWN:
		do {
			tempX = rand() % (MAP_WIDTH - 2) + 1;
			tempY = rand() % (MAP_HEIGHT - 4) + 1;
			int k = 0;
			isOK = true;
			for (int j = 1; j >= 0 && isOK; j--)
				for (int i = 0; i < 3; i++) {
					randomCell[k].position.x = tempX + i;
					randomCell[k].position.y = tempY + j;
					isOK = freeCell(randomCell[k], snake, gate);
					k++;
					if (!isOK)
						break;
				}
		} while (!isOK);
		break;
	case LEFT:
		do {
			tempX = rand() % (MAP_WIDTH - 4) + 2;
			tempY = rand() % (MAP_HEIGHT - 4) + 1;
			int k = 0;
			isOK = true;
			for (int i = 0; i < 2 && isOK; i++)
				for (int j = 0; j < 3; j++) {
					randomCell[k].position.x = tempX + i;
					randomCell[k].position.y = tempY + j;
					isOK = freeCell(randomCell[k], snake, gate);
					k++;
					if (!isOK)
						break;
				}
		} while (!isOK);
		break;
	case RIGHT:
		do {
			tempX = rand() % (MAP_WIDTH - 4) + 1;
			tempY = rand() % (MAP_HEIGHT - 4) + 1;
			int k = 0;
			isOK = true;
			for (int i = 1; i >= 0 && isOK; i--)
				for (int j = 0; j < 3; j++) {
					randomCell[k].position.x = tempX + i;
					randomCell[k].position.y = tempY + j;
					isOK = freeCell(randomCell[k], snake, gate);
					k++;
					if (!isOK)
						break;
				}
		} while (!isOK);
		break;
	}
	for (int i = 0; i < 5; i++) {
		if (i == 0)
			gate.wall[i] = randomCell[i];
		else
			gate.wall[i] = randomCell[i + 1];
	}
	gateCreated = true;
}

void drawGate(const Gate& gate, int color) {
	for (int i = 0; i < 5; i++)
		drawOneCell(gate.wall[i], color);
	gateDrawn = color != ColorCode_Black;
}

bool writeToFile(char* filename, const Snake& snake, const Gate& gate, const FoodList& food) {
	FILE* f = fopen(filename, "w");
	if (!f)
		return false;
	fprintf(f, "%d %d %d\n", moveLock, gateCreated, dirChanged);
	fprintf(f, "%d %d\n", gameLevel, gameScore);
	fprintf(f, "%d %d\n", snake.length, snake.status);
	LinkedList* iter = snake.head;
	for (; iter != NULL; iter = iter->nextNode) {
		fprintf(f, "%d %d\n", iter->data.position.x, iter->data.position.y);
	}
	fprintf(f, "%d\n", gate.gateDir);
	for (int i = 0; i < 5; i++)
		fprintf(f, "%d %d\n", gate.wall[i].position.x, gate.wall[i].position.y);
	fprintf(f, "%d\n", food.foodIndex);
	for (int i = 0; i < FOOD_NUM; i++)
		fprintf(f, "%d %d\n", food.food[i].position.x, food.food[i].position.y);
	return true;
}

bool readFromFile(char* filename, Snake& snake, Gate& gate, FoodList& food) {
	FILE* f = fopen(filename, "r");
	if (!f)
		return false;
	removeSnake(snake);
	fscanf(f, "%d %d %d", &moveLock, &gateCreated, &dirChanged);
	fscanf(f, "%d %d", &gameLevel, &gameScore);
	fscanf(f, "%d %d", &snake.length, &snake.status);
	LinkedList* iter = snake.head;
	initSnake(snake, snake.length, snake.status);
	for (iter = snake.head; iter != NULL; iter = iter->nextNode) {
		fscanf(f, "%d %d", &iter->data.position.x, &iter->data.position.y);
	}
	oldTail = findTail(snake)->data;
	fscanf(f, "%d", &gate.gateDir);
	for (int i = 0; i < 5; i++) {
		fscanf(f, "%d %d", &gate.wall[i].position.x, &gate.wall[i].position.y);
		gate.wall[i].symbol = '*';
	}
	fscanf(f, "%d", &food.foodIndex);
	for (int i = 0; i < FOOD_NUM; i++) {
		fscanf(f, "%d %d", &food.food[i].position.x, &food.food[i].position.y);
		food.food[i].symbol = '#';
	}
	gameSpeed = MIN_SPEED / gameLevel;
	return true;
}

void resetData(Snake& snake, Gate& gate, FoodList& food) {
	removeSnake(snake);
	initSnake(snake, snake.length, RIGHT);
	removeGate(gate);
	initFood(food, snake, gate);
	mapDrawn = false;
}

void updateLevel(Snake& snake) {
	gameLevel = (gameLevel) % (MAX_LEVEL - 1) + 1;
	if (gameLevel == 1) {
		snake.length = 6;
	}
	gameSpeed = MIN_SPEED / gameLevel;
}

void drawGame(const Snake& snake, const Gate& gate, const FoodList& food, int color) {
	drawSnake(snake, color); //Draw current snake
	if (!mapDrawn)
		drawMap(color);
	if (!foodDrawn)
		drawFood(food, color);
	if (gateCreated && !gateDrawn)
		drawGate(gate, color);
}

void gameThread() {
	while (true) {
		if (isAlive) { //When snake is alive
			//Draw section
			drawGame(snake, gate, food, ColorCode_White);
			
			//Change snake's position
			moveSnake(snake);
			
			//Unlock direction changes
			dirChanged = true;

			//Check pass gate
			if (gateCreated) {
				int temp = abs(static_cast<int>(snake.status) - static_cast<int>(gate.gateDir));
				if (temp == 2 && snake.head != NULL
					&& hitSnakeHead(gate.wall[3], snake)) { //If snake goes into gate
					snake.head = snake.head->nextNode; //Keep snake head still, temporarily points to next node
					moveLock = true; //Lock snake movement
				}
				if (snake.head == NULL) { //If snake pass through the game
					drawGate(gate, ColorCode_Black); //Erase gate
					drawOneCell(oldTail, ColorCode_Black); //Erase snake tail
					updateLevel(snake); //Update game level
					resetData(snake, gate, food); //Reset data based on current level
				}
			}

			//Check dead & processing
			if (hitWall(snake.head->data, gate) 
			 || hitSnakeBody(snake.head->data, snake)) { //If snake hits wall or itself, show effect
				isAlive = false;
				while(!isAlive) { //Show message
					goToXY(INFO_X, INFO_Y);
					setColor(ColorCode_White);
					cout << "Game over.";
					goToXY(INFO_X, INFO_Y + 2);
					cout << "Your score is " << gameScore << ".";
					goToXY(INFO_X, INFO_Y + 4);
					cout << "Press Y to play again.";
					goToXY(INFO_X, INFO_Y + 6);
					cout << "Press any other key to exit.";
					hitEffect(snake, gate, ColorCode_Red); //Boom red
					hitEffect(snake, gate, ColorCode_Black);
					Sleep(100);
				}
				system("cls");
				continue;
			}

			//Eating food processing
			if (foodEaten(food, snake)) //If snake eats food
				addSnake(snake, oldTail); //Keep old snake tail && add new node to tail
			else
				drawOneCell(oldTail, ColorCode_Black); //Draw black old tail cell

			//Draw new snake after moving process
			drawSnake(snake, ColorCode_White);

			if (food.foodIndex == 4 && !gateCreated) { //If one level is over, create a gate
				initGate(gate, snake);
				drawGate(gate, ColorCode_White);
			}

			//Update snake tail cell
			oldTail = findTail(snake)->data;

			Sleep(gameSpeed);
		}
	}	
}

void printMenu(int choice) {
	goToXY(MENU_X, MENU_Y - 2);
	setColor(ColorCode_White);
	cout << "Welcome to Snake Game";
	char* message[3];
	message[0] = "Start New Game";
	message[1] = "Load Saved Game";
	message[2] = "Exit";
	for (int i = 0; i < 3; i++) {
		goToXY(MENU_X, MENU_Y + 2 * (i + 1));
		if (i == choice)
			setColor(ColorCode_MenuChoice);
		else
			setColor(ColorCode_White);
		cout << message[i];
	}
}

void processingBar(int x, int y) {
	srand(static_cast<unsigned int>(time(0)));
	int r = rand() % 500;
	for (int i = 0; i < 25; i++) {
		goToXY(x, y);
		setColor(ColorCode_White);
		cout << (i + 1) * 4 << "% completed: ";
		string temp = " ";
		for (int j = 0; j < i; j++) {
			goToXY(x + 16, y);
			setColor(ColorCode_MenuChoice);
			temp += ' ';
		}
		cout << temp;
		if (i < 6)
			Sleep(r / 12);
		else if (i < 18)
			Sleep(r / 4);
		else
			Sleep(r / 8);
	}
}

void saveGame() {
	//Clear screen
	setColor(ColorCode_White);
	system("cls");

	//Get save destination
	goToXY(MENU_X - 10, MENU_Y);
	setColor(ColorCode_White);
	cout << "Where do you want to save your game ?: "; //Ask where to save
	char filename[256];
	cin.getline(filename, 256);

	//Saving section
	LinkedList* temp = snake.head; //If snake is in gate, get back to real head for saving
	snake.head = tempHead; //Back to real head
	writeToFile(filename, snake, gate, food); //Write to file
	snake.head = temp; //Return to the current head to pass the gate

	//Inform game's been saving & finished
	goToXY(MENU_X - 10, MENU_Y + 2);
	cout << "Saving...";
	processingBar(MENU_X - 10, MENU_Y + 4);
	goToXY(MENU_X - 10, MENU_Y + 6);
	setColor(ColorCode_White);
	cout << "Saved. Resuming game...";

	//Reset drawing conditions
	mapDrawn = false;
	gateDrawn = false;
	foodDrawn = false;

	//Wait a bit for player to get ready
	Sleep(1000);
	system("cls");
}

void loadGame() {
	//Clear screen
	setColor(ColorCode_White);
	system("cls");

	goToXY(MENU_X - 10, MENU_Y);
	cout << "Enter file name: "; //Get input file name
	char filename[256];
	cin.getline(filename, 256);

	goToXY(MENU_X - 10, MENU_Y + 2);
	cout << "Loading...";
	processingBar(MENU_X - 10, MENU_Y + 4);
	setColor(ColorCode_White);

	//Read main data
	if (readFromFile(filename, snake, gate, food)) {
		
		//Process when snake is in gate (continuous node from head have the same position)
		LinkedList* iter = snake.head->nextNode;
		while (iter != NULL
			&& iter->data.position.x == snake.head->data.position.x
			&& iter->data.position.y == snake.head->data.position.y)
			iter = iter->nextNode;
		if (iter != snake.head->nextNode)
			snake.head = iter; 
		
		goToXY(MENU_X - 10, MENU_Y + 6);
		cout << "Loaded. Starting game...";
	}
	else {
		goToXY(MENU_X - 10, MENU_Y + 6);
		cout << "That file doesn't exist. Starting game...";
	}

	//Wait a bit for player to get ready
	Sleep(1000);

	system("cls");
}

void endGame() {
	//Clear screen
	setColor(ColorCode_White);
	system("cls");

	//Show end game message
	int color = 1;
	while (true) { //Goodbye screen
		goToXY(MENU_X + 6, MENU_Y);
		setColor(color);
		color = (++color) % 15 + 1;
		cout << "Goodbye.";
		goToXY(MENU_X, MENU_Y + 2);
		cout << "Press any key to exit.";
		if (_kbhit())
			break;
		Sleep(200);
	}
	
	setColor(ColorCode_Black);
}



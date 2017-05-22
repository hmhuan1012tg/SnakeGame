//Main file

#include "snake.h"
#include "console.h"

int main() {
	//Console setting
	fixConsoleWindow();
	hideCursor();

	//First initialization
	updateLevel(snake);
	resetData(snake, gate, food);

	//Menu
	int choice = 0;
	while (true) {
		if (_kbhit()) {
			char c = toupper(_getch());
			if (c == 'W')
				choice = (choice + 2) % 3;
			else if (c == 'S')
				choice = (choice + 1) % 3;
			else if (c == 13)
				break;
		}
		printMenu(choice);
		Sleep(200);
	}
	setColor(ColorCode_White);

	//If choose not to exit, start game
	if (choice != 2) {
		if (choice == 1) //If choose to load saves
			loadGame();
		system("cls"); //Clear screen for drawing main game

		//Main game loop
		thread t1(gameThread); //Game thread
		HANDLE handle_t1 = t1.native_handle();

		//Keyboard Input Thread
		while (true) {
			if (_kbhit()) {
				char c = toupper(_getch());
				if (isAlive) { //If snake is alive
					if (c == 'P') { //Pause game
						if (!gamePaused)
							pauseGame(handle_t1);
						else
							resumeGame(handle_t1);
					}
					else if (c == 27) { //Exit game
						exitGame(handle_t1);
						break;
					}
					else if (c == 'L') { //Save game
						pauseGame(handle_t1);
						saveGame();
						resumeGame(handle_t1);
					}
					else if(dirChanged) { //Change snake direction
						resumeGame(handle_t1);
						switch (c) {
						case 'W':
							if (snake.status != DOWN && !moveLock)
								snake.status = UP;
							break;
						case 'S':
							if (snake.status != UP && !moveLock)
								snake.status = DOWN;
							break;
						case 'A':
							if (snake.status != RIGHT && !moveLock)
								snake.status = LEFT;
							break;
						case 'D':
							if (snake.status != LEFT && !moveLock)
								snake.status = RIGHT;
							break;
						}
						dirChanged = false;
					}
				}
				else { //If snake is dead
					if (c == 'Y') { //Play again
						gameLevel = 0;
						updateLevel(snake);
						resetData(snake, gate, food);
					}
					else { //Exit game
						exitGame(handle_t1);
						break;
					}
				}
			}
		}
		t1.join(); //Wait for game thread to exit
	}
	
	removeSnake(snake); //Release memory

	//End game scene
	system("cls");
	int color = 1;
	while (true) { //Goodbye screen
		goToXY(MENU_X, MENU_Y);
		setColor(color);
		color = (++color) % 15 + 1;
		cout << "Goodbye.";
		goToXY(MENU_X - 5, MENU_Y + 1);
		cout << "Press any key to exit.";
		if (_kbhit())
			break;
		Sleep(200);
	}
	setColor(ColorCode_Black);

	return 0;
}
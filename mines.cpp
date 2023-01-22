#include <SDL.h>
#include <iostream>


const int ROWS = 8;
const int COLS = 8;
const int CELL_SIZE = 40;

int grid[ROWS][COLS];
int mines = 10;
bool gameOver = false;
bool win = false;

void drawBoard(int grid[][COLS], SDL_Renderer* renderer) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid[i][j] == -1) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            }
            else if (grid[i][j] == -2) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else if (grid[i][j] == -3) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            }
            SDL_Rect rect = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}

int countMines(int grid[][COLS], int x, int y) {
	int count = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (x + i >= 0 && x + i < COLS && y + j >= 0 && y + j < ROWS) {
				if (grid[y + j][x + i] == -1) {
					count++;
				}
			}
		}
	}
	return count;
}

void handleInput(int grid[][COLS], SDL_Event event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.x / CELL_SIZE;
        int y = event.button.y / CELL_SIZE;
        if (event.button.button == SDL_BUTTON_LEFT) {
            if (grid[y][x] == -1) {
                gameOver = true;
            } else {
                grid[y][x] = -2;
            }
        }
        else if (event.button.button == SDL_BUTTON_RIGHT) {
            if (grid[y][x] == -3) {
                grid[y][x] = 0;
            }
            else {
                grid[y][x] = -3;
            }
        }
    }
}

void checkForWin(int grid[][COLS]) {
    int revealedCells = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid[i][j] == -2) {
                revealedCells++;
            }
        }
    }
    if (revealedCells == (ROWS * COLS) - mines) {
        win = true;
    }
}


int main(int argc, char* argv[]) {
    srand(time(0));
    int minesPlaced = 0;
    while (minesPlaced < mines) {
        int x = rand() % COLS;
        int y = rand() % ROWS;
        if (grid[y][x] != -1) {
            grid[y][x] = -1;
            minesPlaced++;
        }
    }
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            handleInput(grid, event);
        }
        checkForWin(grid);
        if (gameOver || win) {
            running = false;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        drawBoard(grid, renderer);

        SDL_RenderPresent(renderer);
    }
    if (gameOver) {
        std::cout << "Game Over!" << std::endl;
    } 
    else if (win) {
        std::cout << "You Win!" << std::endl;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
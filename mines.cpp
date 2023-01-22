#include <SDL.h>
#include <iostream>
#include <SDL_ttf.h>
#include <string>


const int ROWS = 8;
const int COLS = 8;
const int CELL_SIZE = 40;

int grid[ROWS][COLS];
int mines = 10;
bool gameOver = false;
bool win = false;


void drawNeighboursCount(int grid[][COLS], SDL_Renderer* renderer, int x, int y, TTF_Font* font) {
    SDL_Color color = {0, 0, 0, 255};
    std::string number_of_mines = std::to_string(grid[x][y]);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, number_of_mines.c_str(), color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int text_x = y*CELL_SIZE + CELL_SIZE/2 - textSurface->w/2;
    int text_y = x*CELL_SIZE + CELL_SIZE/2 - textSurface->h/2;
    SDL_Rect textRect = {text_x, text_y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void drawRectangle(SDL_Renderer* renderer, int r, int g, int b, SDL_Rect rect) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
}


void drawBoard(int grid[][COLS], SDL_Renderer* renderer, TTF_Font* font) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            SDL_Rect rect = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            switch (grid[i][j]) {
                case -1:
                    drawRectangle(renderer, 0, 0, 255, rect);
                    break;
                case -2:
                    drawRectangle(renderer, 128, 128, 128, rect);
                    break;
                case -3:
                    drawRectangle(renderer, 255, 0, 0, rect);
                    break;
                case 0:
                    drawRectangle(renderer, 255, 255, 255, rect);
                    break;
                default:
                    drawRectangle(renderer, 128, 128, 128, rect);
                    drawNeighboursCount(grid, renderer, i, j, font);
                    break;
            }
        }
    }
}

int countNeighbours(int board[][COLS], int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (x + j >= 0 && x + j < COLS && y + i >= 0 && y + i < ROWS) {
                if (board[y + i][x + j] == -1) {
                    count++;
                }
            }
        }
    }
    if (count > 0) {
        return count;
    }
    else {
        return -2;
    }
}

void handleInput(int grid[][COLS], SDL_Event event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.x / CELL_SIZE;
        int y = event.button.y / CELL_SIZE;
        if (event.button.button == SDL_BUTTON_LEFT) {
            if (grid[y][x] == -1) {
                gameOver = true;
            } else {
                grid[y][x] = countNeighbours(grid, x, y);
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
    int count = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid[i][j] == -2 || grid[i][j] > 0) {
                count++;
            }
        }
    }
    if (count == ROWS * COLS - mines) {
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
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("font.ttf", 24);

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

        drawBoard(grid, renderer, font);

        SDL_RenderPresent(renderer);
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
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

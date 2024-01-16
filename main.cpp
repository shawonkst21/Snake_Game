#include<bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;


const int SCREEN_WIDTH = 620;
const int SCREEN_HEIGHT = 480;
const int GRID_SIZE = 10;
const int INITIAL_LENGTH = 5;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;
SDL_Color textColor = {255, 255, 255, 255};

struct Snake {
    vector<pair<int, int>> body;
    char direction;
};

struct Food {
    int x, y;
};

Snake snake;
Food fruit;
int score = 0;

bool gameOver = false;
//creating a food in random place............................................
void foodplace() 
{
    bool onSnakeBody = true;
    
    while (onSnakeBody)
     {
        onSnakeBody = false;
        fruit.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
        fruit.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;

        // Check fruit on body;
        for (const auto& segment : snake.body) 
        {
            if (fruit.x == segment.first && fruit.y == segment.second)
             {
                onSnakeBody = true;
                break;
            }
        }
    }
}
//initialize winddow,renderer and ttf...............................
void initializepart() 
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

    font = TTF_OpenFont("shawon.ttf", 25);

    if (!font)
     {
        cerr << "Failed to load font: " << TTF_GetError() << endl;
    
    }

    snake.body.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    snake.direction = 'R';
    foodplace();
}
//update .....................................................................
void update() {


    pair<int, int> newsnake = snake.body.front();
    
    switch (snake.direction) {
        case 'U':
            newsnake.second -= GRID_SIZE;
            break;
        case 'D':
            newsnake.second += GRID_SIZE;
            break;
        case 'L':
            newsnake.first -= GRID_SIZE;
            break;
        case 'R':
            newsnake.first += GRID_SIZE;
            break;
    }

    // collisions with the snake body
    for (size_t i = 1; i < snake.body.size(); ++i) {
        if (newsnake.first == snake.body[i].first && newsnake.second == snake.body[i].second) 
        {
            gameOver = true;
            break;
        }
    }
    //collusion check with boundaries.....................................................
    if (newsnake.first < 0) {
        newsnake.first = SCREEN_WIDTH - GRID_SIZE;
    } else if (newsnake.first >= SCREEN_WIDTH) {
        newsnake.first = 0;
    }

    if (newsnake.second < 0) {
        newsnake.second = SCREEN_HEIGHT - GRID_SIZE;
    } else if (newsnake.second >= SCREEN_HEIGHT) {
        newsnake.second = 0;
    }

    snake.body.insert(snake.body.begin(), newsnake);

    if (newsnake.first == fruit.x && newsnake.second == fruit.y) {
        foodplace();
        score +=5;
    } else {
        snake.body.pop_back();
    }

}
//
void renderGameOver() {
    TTF_Font* gameOverFont = TTF_OpenFont("shawon.ttf", 55);
    SDL_Color gameOverColor = {255, 0, 0, 255};
    string gameOverText = "Game Over!...";
    string finalScoreText = "Final Score: " + to_string(score);

    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(gameOverFont, gameOverText.c_str(), gameOverColor);
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_Rect gameOverRect = {(SCREEN_WIDTH - gameOverSurface->w) / 2, SCREEN_HEIGHT / 3, gameOverSurface->w, gameOverSurface->h};
    SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);

    SDL_Surface* finalScoreSurface = TTF_RenderText_Solid(font, finalScoreText.c_str(), textColor);
    SDL_Texture* finalScoreTexture = SDL_CreateTextureFromSurface(renderer, finalScoreSurface);
    SDL_Rect finalScoreRect = {(SCREEN_WIDTH - finalScoreSurface->w) / 2, SCREEN_HEIGHT / 2, finalScoreSurface->w, finalScoreSurface->h};
    SDL_RenderCopy(renderer, finalScoreTexture, nullptr, &finalScoreRect);

    SDL_FreeSurface(gameOverSurface);
    SDL_DestroyTexture(gameOverTexture);
    TTF_CloseFont(gameOverFont);

    SDL_FreeSurface(finalScoreSurface);
    SDL_DestroyTexture(finalScoreTexture);
}

void snakerend()
{
     for (const auto& segment : snake.body) {
        SDL_Rect rect = {segment.first, segment.second, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }
}
void foodrend()
{
     SDL_Rect foodRect = {fruit.x, fruit.y, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);
}
void render() {
    SDL_SetRenderDrawColor(renderer, 0, 102, 102, 255);
    SDL_RenderClear(renderer);

    if (!gameOver) {
    
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    snakerend();

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    foodrend();

    // Render the score
    string scoreText = "Score: " + to_string(score);
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect scoreRect = {10, 10, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &scoreRect);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
    } else {
       renderGameOver();
        
        SDL_RenderPresent(renderer);
        SDL_Delay(30000);
        SDL_Quit();
    }
}

bool isPointInsideRect(int x, int y, const SDL_Rect& rect) {
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
}
//button clicked or not........................................
bool isStartButtonClicked(int mouseX, int mouseY) {
    
    SDL_Rect startButtonRect = {200, 200, 200, 50};

    return isPointInsideRect(mouseX, mouseY, startButtonRect);
}

void renderStartButton() {
    SDL_Rect startButtonRect = {200, 200, 200, 50};
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &startButtonRect);

    TTF_Font* buttonFont = TTF_OpenFont("shawon.ttf", 60);
    SDL_Color buttonTextColor = {255, 255, 255, 255};
    string buttonText = "#start game#";

    SDL_Surface* surface = TTF_RenderText_Solid(buttonFont, buttonText.c_str(), buttonTextColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {startButtonRect.x + (startButtonRect.w - surface->w) / 2,
                         startButtonRect.y + (startButtonRect.h - surface->h) / 2,
                         surface->w,
                         surface->h};

    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(buttonFont);
}

void renderExitButton() {
    SDL_Rect exitButtonRect = {200, 300, 200, 50};
    SDL_RenderFillRect(renderer, &exitButtonRect);

    TTF_Font* buttonFont = TTF_OpenFont("shawon.ttf", 40);
    SDL_Color buttonTextColor = {255, 0, 0, 255};
    string buttonText = "Exit";

    SDL_Surface* surface = TTF_RenderText_Solid(buttonFont, buttonText.c_str(), buttonTextColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {exitButtonRect.x + (exitButtonRect.w - surface->w) / 2,
                         exitButtonRect.y + (exitButtonRect.h - surface->h) / 2,
                         surface->w,
                         surface->h};

    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(buttonFont);
}

bool isExitButtonClicked(int mouseX, int mouseY) {
    SDL_Rect exitButtonRect = {200, 300, 200, 50};
    return isPointInsideRect(mouseX, mouseY, exitButtonRect);
}

void gameloop() {
    SDL_Event e;
    bool quit = false;
    bool gameStarted = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                
                switch (e.key.keysym.sym)
                 {
                    case SDLK_UP:
                        if (snake.direction != 'D') snake.direction = 'U';
                        break;
                    case SDLK_DOWN:
                        if (snake.direction != 'U') snake.direction = 'D';
                        break;
                    case SDLK_LEFT:
                        if (snake.direction != 'R') snake.direction = 'L';
                        break;
                    case SDLK_RIGHT:
                        if (snake.direction != 'L') snake.direction = 'R';
                        break;
                }

            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (!gameStarted && isStartButtonClicked(mouseX, mouseY)) {
                    gameStarted = true;
                } else if (isExitButtonClicked(mouseX, mouseY)) {
                    quit = true;
                }
            }
        }

        if (gameStarted) {
            update();
            render();
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 102, 102, 255);
            SDL_RenderClear(renderer);

            renderStartButton();
            renderExitButton(); 

            SDL_RenderPresent(renderer);
        }

        SDL_Delay(100);
    }
}


int main(int argc, char* argv[]) {
    initializepart();
    gameloop();

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}


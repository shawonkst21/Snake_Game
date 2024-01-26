#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include<SDL2/SDL_image.h>
using namespace std;

const int SCREEN_WIDTH = 620;
const int SCREEN_HEIGHT = 480;
const int GRID_SIZE = 10;
const int INITIAL_LENGTH = 5;
int current_time;
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
TTF_Font *font = nullptr;
SDL_Color textColor = {255, 255, 255, 255};
SDL_Surface *p_surface = nullptr;
SDL_Texture *p_texture = nullptr;
Mix_Music *bgm = nullptr;
Mix_Music *test = nullptr;
Mix_Music *ne = nullptr;
Mix_Chunk *eat = nullptr;

SDL_Surface *finished = nullptr;
SDL_Texture *g_finished = nullptr;
SDL_Surface *phase_1 = nullptr;
SDL_Texture *Phase_t1 = nullptr;
SDL_Surface *msurface = nullptr;
SDL_Texture *mtexture = nullptr;
int highscore = 0;
int level = 1;
struct Snake
{
    vector<pair<int, int>> body;
    char direction;
};

struct Food
{
    int x, y;
};

Snake snake;
Food fruit;
Food sfruit;
int score = 0;
int cnt = 0;
bool gameOver = false;
// creating a food in random place............................................
bool isPointInsideRect(int x, int y, const SDL_Rect &rect)
{
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
}
void foodplace()
{
    bool onSnakeBody = true;

    while (onSnakeBody)
    {
        onSnakeBody = false;
        fruit.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
        fruit.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;

        // Check fruit on body;
        for (const auto &segment : snake.body)
        {
            if (fruit.x == segment.first && fruit.y == segment.second)
            {
                onSnakeBody = true;
                break;
            }
        }
    }
}
void foodplace2()
{
    bool onSnakeBody = true;
    bool onObstacleBody = true;

    while (onSnakeBody || onObstacleBody)
    {
        onSnakeBody = false;
        onObstacleBody = false;
        fruit.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
        fruit.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;

        // Check fruit on body;
        for (const auto &segment : snake.body)
        {
            if (fruit.x == segment.first && fruit.y == segment.second)
            {
                onSnakeBody = true;
                break;
            }
        }
        SDL_Rect obstacle1 = {0, 300, 300, 15};
        SDL_Rect obstacle2 = {SCREEN_WIDTH - 300, 150, 300, 15};
        if (isPointInsideRect(fruit.x, fruit.y, obstacle1) || isPointInsideRect(fruit.x, fruit.y, obstacle2))
        {
            onObstacleBody = true;
        }
    }
}
void foodplace3()
{
    bool onSnakeBody = true;

    while (onSnakeBody)
    {
        onSnakeBody = false;
        fruit.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
        fruit.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;

        // Check fruit on body;
        for (const auto &segment : snake.body)
        {
            if (fruit.x == segment.first && fruit.y == segment.second)
            {
                onSnakeBody = true;
                break;
            }
        }

        // Check fruit on side obstacles;
        if (fruit.x >= 0 && fruit.x <= 15 && fruit.y >= 0 && fruit.y <= SCREEN_HEIGHT ||
            fruit.x >= SCREEN_WIDTH - 25 && fruit.x <= SCREEN_WIDTH && fruit.y >= 0 && fruit.y <= SCREEN_HEIGHT ||
            fruit.x >= 15 && fruit.x <= SCREEN_WIDTH - 15 && fruit.y >= 0 && fruit.y <= 25 ||
            fruit.x >= 15 && fruit.x <= SCREEN_WIDTH - 15 && fruit.y >= SCREEN_HEIGHT - 25 && fruit.y <= SCREEN_HEIGHT)
        {
            onSnakeBody = true;
        }

        // Check fruit inside obstacles;
        if ((fruit.x >= 80 && fruit.x <= 280 && fruit.y >= 75 && fruit.y <= 90) ||
            (fruit.x >= 340 && fruit.x <= 540 && fruit.y >= 75 && fruit.y <= 90) ||
            (fruit.x >= 65 && fruit.x <= 80 && fruit.y >= 75 && fruit.y <= 210) ||
            (fruit.x >= 540 && fruit.x <= 555 && fruit.y >= 75 && fruit.y <= 210) ||
            (fruit.x >= 65 && fruit.x <= 80 && fruit.y >= 255 && fruit.y <= 390) ||
            (fruit.x >= 65 && fruit.x <= 280 && fruit.y >= 390 && fruit.y <= 405) ||
            (fruit.x >= 340 && fruit.x <= 540 && fruit.y >= 390 && fruit.y <= 405) ||
            (fruit.x >= 540 && fruit.x <= 555 && fruit.y >= 255 && fruit.y <= 405) ||
            (fruit.x >= SCREEN_WIDTH / 2 - 7 && fruit.x <= 15 && fruit.y >= SCREEN_WIDTH / 2 - 25 && fruit.y <= 50))
        {
            onSnakeBody = true;
        }
    }
}


void sfoodplace()
{
    current_time = SDL_GetTicks();

    bool onSnakeBody = true;

    while (onSnakeBody)
    {
        onSnakeBody = false;
        sfruit.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
        sfruit.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;

        // Check fruit on body;
        for (const auto &segment : snake.body)
        {
            if (sfruit.x == segment.first && sfruit.y == segment.second)
            {
                onSnakeBody = true;
                break;
            }
        }
    }
}
void sfoodplace2()
{
    current_time = SDL_GetTicks();

    bool onSnakeBody = true;
    bool onObstacleBody = true;

    while (onSnakeBody)
    {
        onSnakeBody = false;
        onObstacleBody = false;
        sfruit.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
        sfruit.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;

        // Check fruit on body;
        for (const auto &segment : snake.body)
        {
            if (sfruit.x == segment.first && sfruit.y == segment.second)
            {
                onSnakeBody = true;
                break;
            }
        }
        SDL_Rect obstacle1 = {0, 300, 300, 15};
        SDL_Rect obstacle2 = {SCREEN_WIDTH - 300, 150, 300, 15};
        if (isPointInsideRect(sfruit.x, sfruit.y, obstacle1) || isPointInsideRect(sfruit.x, sfruit.y, obstacle2))
        {
            onObstacleBody = true;
        }
    }
}
void sfoodplace3()
{
    current_time = SDL_GetTicks();

    bool onSnakeBody = true;

    while (onSnakeBody)
    {
        onSnakeBody = false;
        sfruit.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
        sfruit.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;

        // Check fruit on body;
        for (const auto &segment : snake.body)
        {
            if (sfruit.x == segment.first && sfruit.y == segment.second)
            {
                onSnakeBody = true;
                break;
            }
        }

        // Check fruit on side obstacles;
        if (sfruit.x >= 0 && sfruit.x <= 15 && sfruit.y >= 0 && sfruit.y <= SCREEN_HEIGHT ||
            sfruit.x >= SCREEN_WIDTH - 15 && sfruit.x <= SCREEN_WIDTH && sfruit.y >= 0 && sfruit.y <= SCREEN_HEIGHT ||
            sfruit.x >= 15 && sfruit.x <= SCREEN_WIDTH - 15 && sfruit.y >= 0 && sfruit.y <= 15 ||
            sfruit.x >= 15 && sfruit.x <= SCREEN_WIDTH - 15 && sfruit.y >= SCREEN_HEIGHT - 15 && sfruit.y <= SCREEN_HEIGHT)
        {
            onSnakeBody = true;
        }

        // Check fruit inside obstacles;
        if ((sfruit.x >= 80 && sfruit.x <= 280 && sfruit.y >= 75 && sfruit.y <= 90) ||
            (sfruit.x >= 340 && sfruit.x <= 540 && sfruit.y >= 75 && sfruit.y <= 90) ||
            (sfruit.x >= 65 && sfruit.x <= 80 && sfruit.y >= 75 && sfruit.y <= 210) ||
            (sfruit.x >= 540 && sfruit.x <= 555 && sfruit.y >= 75 && sfruit.y <= 210) ||
            (sfruit.x >= 65 && sfruit.x <= 80 && sfruit.y >= 255 && sfruit.y <= 390) ||
            (sfruit.x >= 65 && sfruit.x <= 280 && sfruit.y >= 390 && sfruit.y <= 405) ||
            (sfruit.x >= 340 && sfruit.x <= 540 && sfruit.y >= 390 && sfruit.y <= 405) ||
            (sfruit.x >= 540 && sfruit.x <= 555 && sfruit.y >= 255 && sfruit.y <= 405))
        {
            onSnakeBody = true;
        }
    }
}
// initialize winddow,renderer and ttf...............................
void initializepart()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

    font = TTF_OpenFont("shawon.ttf", 25);

    if (!font)
    {
        cerr << "Failed to load font: " << TTF_GetError() << endl;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        cout << "ERROR :" << Mix_GetError() << endl;
    }

    snake.body.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    snake.direction = 'R';
    foodplace();
    sfruit.x = -100;
    sfruit.y = -200;
}
bool check(int a, int b)
{

    SDL_Rect side1 = {0, 0, 15, SCREEN_HEIGHT};
    SDL_Rect side2 = {15, 0, SCREEN_WIDTH - 30, 15};
    SDL_Rect side3 = {SCREEN_WIDTH - 20, 0, 15, SCREEN_HEIGHT};
    SDL_Rect side4 = {15, SCREEN_HEIGHT - 20, SCREEN_WIDTH - 35, 15};

    SDL_Rect inside1 = {80, 75, 200, 15};
    SDL_Rect inside2 = {340, 75, 200, 15};
    SDL_Rect inside3 = {65, 75, 15, 135};
    SDL_Rect inside4 = {540, 75, 15, 135};
    SDL_Rect inside5 = {65, 255, 15, 135};
    SDL_Rect inside6 = {65, 390, 215, 15};
    SDL_Rect inside7 = {340, 390, 200, 15};
    SDL_Rect inside8 = {540, 255, 15, 150};
    SDL_Rect inside9 = {SCREEN_WIDTH / 2 - 7, SCREEN_HEIGHT / 2 - 25, 15, 50};

    if (isPointInsideRect(a, b, side1) || isPointInsideRect(a, b, side2) || isPointInsideRect(a, b, side3) || isPointInsideRect(a, b, side4) ||
        isPointInsideRect(a, b, inside1) || isPointInsideRect(a, b, inside2) || isPointInsideRect(a, b, inside3) || isPointInsideRect(a, b, inside4) ||
        isPointInsideRect(a, b, inside5) || isPointInsideRect(a, b, inside6) || isPointInsideRect(a, b, inside7) || isPointInsideRect(a, b, inside8) ||
        isPointInsideRect(a, b, inside9))
    {
        return 1;
    }
}
bool foodcheck(int a, int b)
{
    SDL_Rect foodRect = {fruit.x, fruit.y, 15, 15};
    if (isPointInsideRect(a, b, foodRect))
    {
        return 1;
    }
}
// update .....................................................................
void update()
{
    int recent_time = SDL_GetTicks();
    if (recent_time - current_time >= 6000)
    {
        sfruit.x = -100;
        sfruit.y = -100;
    }
    pair<int, int> newsnake = snake.body.front();

    switch (snake.direction)
    {
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
    for (size_t i = 1; i < snake.body.size(); ++i)
    {
        if (newsnake.first == snake.body[i].first && newsnake.second == snake.body[i].second)
        {
            gameOver = true;
            Mix_HaltMusic();
            break;
        }
    }
    if (level == 2)
    {
        SDL_Rect obstacle1 = {0, 300, 300, 15};
        SDL_Rect obstacle2 = {SCREEN_WIDTH - 300, 150, 300, 15};
        if (isPointInsideRect(newsnake.first, newsnake.second, obstacle1) || isPointInsideRect(newsnake.first, newsnake.second, obstacle2))
        {
            gameOver = true;
            Mix_HaltMusic();
        }
    }
   else if (level == 3)
    {
        bool over = check(newsnake.first, newsnake.second);
        if (over)
        {
            gameOver = true;
            Mix_HaltMusic();
        }
    }
    // collusion check with boundaries.....................................................
    if (newsnake.first < 0)
    {
        newsnake.first = SCREEN_WIDTH - GRID_SIZE;
    }
    else if (newsnake.first >= SCREEN_WIDTH)
    {
        newsnake.first = 0;
    }

    if (newsnake.second < 0)
    {
        newsnake.second = SCREEN_HEIGHT - GRID_SIZE;
    }
    else if (newsnake.second >= SCREEN_HEIGHT)
    {
        newsnake.second = 0;
    }

    snake.body.insert(snake.body.begin(), newsnake);

    if (foodcheck(newsnake.first, newsnake.second))
    {
        if (level == 1)
        {
            foodplace();
        }
        else if (level == 2)
        {
            foodplace2();
        }
        else if(level==3)
        {
            foodplace3();
        }
       Mix_PlayChannel(-1,eat,0);
        score += 5;
        cnt++;
        if (cnt % 7 == 0)
        {
            if (level == 1)
            {
                sfoodplace();
            }
            else if (level == 2)
            {
                sfoodplace2();
            }
            else if(level==3)
            {
                 sfoodplace3();
            }
        }
    }
    else if (newsnake.first == sfruit.x && newsnake.second == sfruit.y)
    {
        score += 10;
        sfruit.x = -100;
        sfruit.y = -100;
    }
    else
    {
        snake.body.pop_back();
    }
}
//
void renderGameOver()
{
    if (level == 1)
    {
        ifstream input("highscore1.txt");
        input >> highscore;

        ofstream output("highscore1.txt");
        if (score > highscore)
        {
            output << score;
        }
        else
        {
            output << highscore;
        }
    }
    else if (level == 2)
    {
        ifstream input("highscore2.txt");
        input >> highscore;

        ofstream output("highscore2.txt");
        if (score > highscore)
        {
            output << score;
        }
        else
        {
            output << highscore;
        }
    }
      else if (level == 3)
    {
        ifstream input("highscore3.txt");
        input >> highscore;

        ofstream output("highscore3.txt");
        if (score > highscore)
        {
            output << score;
        }
        else
        {
            output << highscore;
        }
    }

    TTF_Font *finalsecore = TTF_OpenFont("final.ttf", 30);
    string highscoretxt;
    if (score > highscore)
    {
        highscoretxt = "High Score: " + to_string(score);
    }
    else
    {
        highscoretxt = "High Score: " + to_string(highscore);
    }
    string finalScoreText = "Final Score: " + to_string(score);
    SDL_Color gtextColor = {0, 0, 0, 0};

    SDL_Surface *finalScoreSurface = TTF_RenderText_Solid(finalsecore, finalScoreText.c_str(), gtextColor);
    SDL_Texture *finalScoreTexture = SDL_CreateTextureFromSurface(renderer, finalScoreSurface);
    SDL_FreeSurface(finalScoreSurface);
    SDL_Rect finalScoreRect = {(SCREEN_WIDTH - finalScoreSurface->w) / 2, (SCREEN_HEIGHT / 3) + 50, finalScoreSurface->w, finalScoreSurface->h};
    SDL_RenderCopy(renderer, finalScoreTexture, nullptr, &finalScoreRect);
    SDL_DestroyTexture(finalScoreTexture);

    SDL_Surface *highscoresurface = TTF_RenderText_Solid(finalsecore, highscoretxt.c_str(), gtextColor);
    SDL_Texture *highscoretexture = SDL_CreateTextureFromSurface(renderer, highscoresurface);
    SDL_FreeSurface(highscoresurface);
    SDL_Rect highscorerect = {(SCREEN_WIDTH - highscoresurface->w) / 2, (SCREEN_HEIGHT / 2) + 20, highscoresurface->w, highscoresurface->h};
    SDL_RenderCopy(renderer, highscoretexture, nullptr, &highscorerect);
    SDL_DestroyTexture(highscoretexture);

    TTF_CloseFont(finalsecore);
}
void snakerend()
{
    SDL_Surface *headUpImg = IMG_Load("pic/head_up.png");
    SDL_Texture *headUpTexture = SDL_CreateTextureFromSurface(renderer, headUpImg);
    SDL_FreeSurface(headUpImg);

    SDL_Surface *headDownImg = IMG_Load("pic/head_down.png");
    SDL_Texture *headDownTexture = SDL_CreateTextureFromSurface(renderer, headDownImg);
    SDL_FreeSurface(headDownImg);

    SDL_Surface *headLeftImg = IMG_Load("pic/head_left.png");
    SDL_Texture *headLeftTexture = SDL_CreateTextureFromSurface(renderer, headLeftImg);
    SDL_FreeSurface(headLeftImg);

    SDL_Surface *headRightImg = IMG_Load("pic/head_right.png");
    SDL_Texture *headRightTexture = SDL_CreateTextureFromSurface(renderer, headRightImg);
    SDL_FreeSurface(headRightImg);
//.............................................................................................
    SDL_Surface *bodyUpImg = IMG_Load("pic/body_up.png");
    SDL_Texture *bodyUpTexture = SDL_CreateTextureFromSurface(renderer, bodyUpImg);
    SDL_FreeSurface(bodyUpImg);

    SDL_Surface *bodyDownImg = IMG_Load("pic/body_down.png");
    SDL_Texture *bodyDownTexture = SDL_CreateTextureFromSurface(renderer, bodyDownImg);
    SDL_FreeSurface(bodyDownImg);

    SDL_Surface *bodyLeftImg = IMG_Load("pic/body_left.png");
    SDL_Texture *bodyLeftTexture = SDL_CreateTextureFromSurface(renderer, bodyLeftImg);
    SDL_FreeSurface(bodyLeftImg);

    SDL_Surface *bodyRightImg = IMG_Load("pic/body_right.png");
    SDL_Texture *bodyRightTexture = SDL_CreateTextureFromSurface(renderer, bodyRightImg);
    SDL_FreeSurface(bodyRightImg);
    //..........................................
    SDL_Surface *tailUpImg = IMG_Load("pic/tail_up.png");
    SDL_Texture *tailUpTexture = SDL_CreateTextureFromSurface(renderer, tailUpImg);
    SDL_FreeSurface(tailUpImg);

    SDL_Surface *tailDownImg = IMG_Load("pic/tail_down.png");
    SDL_Texture *tailDownTexture = SDL_CreateTextureFromSurface(renderer, tailDownImg);
    SDL_FreeSurface(tailDownImg);

    SDL_Surface *tailLeftImg = IMG_Load("pic/tail_left.png");
    SDL_Texture *tailLeftTexture = SDL_CreateTextureFromSurface(renderer, tailLeftImg);
    SDL_FreeSurface(tailLeftImg);

    SDL_Surface *tailRightImg = IMG_Load("pic/tail_right.png");
    SDL_Texture *tailRightTexture = SDL_CreateTextureFromSurface(renderer, tailRightImg);
    SDL_FreeSurface(tailRightImg);

    for (int i = 0; i < snake.body.size(); ++i)
    {
        if (i == 0)
        {
            SDL_Rect rect = {snake.body[i].first, snake.body[i].second, 10, 10};

            switch (snake.direction)
            {
            case 'U':
                SDL_RenderCopy(renderer, headUpTexture, NULL, &rect);
                break;
            case 'D':
                SDL_RenderCopy(renderer, headDownTexture, NULL, &rect);
                break;
            case 'L':
                SDL_RenderCopy(renderer, headLeftTexture, NULL, &rect);
                break;
            case 'R':
                SDL_RenderCopy(renderer, headRightTexture, NULL, &rect);
                break;
            }
        }
        else if (i != 0 && i < snake.body.size() - 1)
        {
            SDL_Rect rect = {snake.body[i].first, snake.body[i].second, 10, 10};
            switch (snake.direction)
            {
            case 'U':
                SDL_RenderCopy(renderer, bodyUpTexture, NULL, &rect);
                break;
            case 'D':
                SDL_RenderCopy(renderer, bodyDownTexture, NULL, &rect);
                break;
            case 'L':
                SDL_RenderCopy(renderer, bodyLeftTexture, NULL, &rect);
                break;
            case 'R':
                SDL_RenderCopy(renderer, bodyRightTexture, NULL, &rect);
                break;
            }
        }
        else
        {
            SDL_Rect rect = {snake.body[i].first, snake.body[i].second, 10, 10};
            switch (snake.direction)
            {
            case 'U':
                SDL_RenderCopy(renderer, tailUpTexture, NULL, &rect);
                break;
            case 'D':
                SDL_RenderCopy(renderer, tailDownTexture, NULL, &rect);
                break;
            case 'L':
                SDL_RenderCopy(renderer, tailLeftTexture, NULL, &rect);
                break;
            case 'R':
                SDL_RenderCopy(renderer, tailRightTexture, NULL, &rect);
                break;
            }
        }
    }
    SDL_DestroyTexture(headUpTexture);
    SDL_DestroyTexture(headDownTexture);
    SDL_DestroyTexture(headLeftTexture);
    SDL_DestroyTexture(headRightTexture);
    SDL_DestroyTexture(bodyUpTexture);
    SDL_DestroyTexture(bodyDownTexture);
    SDL_DestroyTexture(bodyLeftTexture);
    SDL_DestroyTexture(bodyRightTexture);
    SDL_DestroyTexture(tailUpTexture);
    SDL_DestroyTexture(tailDownTexture);
    SDL_DestroyTexture(tailLeftTexture);
    SDL_DestroyTexture(tailRightTexture);
}
void foodrend()
{
    SDL_Rect foodRect = {fruit.x, fruit.y, 15, 15};
    // SDL_RenderFillRect(renderer, &foodRect);
    SDL_Surface *image = IMG_Load("pic/fruit2.png");
    SDL_Texture *ourpng = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    SDL_RenderCopy(renderer, ourpng, NULL, &foodRect);
    SDL_DestroyTexture(ourpng);
}
void sfoodrend()
{
    SDL_Rect sfoodRect = {sfruit.x, sfruit.y, 15, 15};
   // SDL_RenderFillRect(renderer, &sfoodRect);
     SDL_Surface *image = IMG_Load("pic/fruit.png");
    SDL_Texture *ourpng = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    SDL_RenderCopy(renderer, ourpng, NULL, &sfoodRect);
    SDL_DestroyTexture(ourpng);
}
void resetsnake()
{
    snake.body.clear();
    snake.body.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    snake.direction = 'R';
    foodplace();
    sfruit.x = -10;
    sfruit.y = -20;
    cnt = 0;
}
void createobstacle()
{
    // obstacle1..............
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect obstacle1 = {0, 300, 300, 15};
    SDL_RenderFillRect(renderer, &obstacle1);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect obstacle2 = {SCREEN_WIDTH - 300, 150, 300, 15};
    SDL_RenderFillRect(renderer, &obstacle2);
    // obstacle2.......................
}
void sideobstacle()
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect side1 = {0, 0, 15, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &side1);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect side2 = {15, 0, SCREEN_WIDTH - 30, 15};
    SDL_RenderFillRect(renderer, &side2);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect side3 = {SCREEN_WIDTH - 15, 0, 15, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &side3);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect side4 = {15, SCREEN_HEIGHT - 15, SCREEN_WIDTH - 30, 15};
    SDL_RenderFillRect(renderer, &side4);
}
void insideobtacle1()
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect inside1 = {80, 75, 200, 15};
    SDL_RenderFillRect(renderer, &inside1);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect inside2 = {340, 75, 200, 15};
    SDL_RenderFillRect(renderer, &inside2);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect inside3 = {65, 75, 15, 135};
    SDL_RenderFillRect(renderer, &inside3);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect inside4 = {540, 75, 15, 135};
    SDL_RenderFillRect(renderer, &inside4);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect inside5 = {65, 255, 15, 135};
    SDL_RenderFillRect(renderer, &inside5);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect inside6 = {65, 390, 215, 15};
    SDL_RenderFillRect(renderer, &inside6);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect inside7 = {340, 390, 200, 15};
    SDL_RenderFillRect(renderer, &inside7);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect inside8 = {540, 255, 15, 150};
    SDL_RenderFillRect(renderer, &inside8);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect inside9 = {SCREEN_WIDTH / 2 - 7, SCREEN_HEIGHT / 2 - 25, 15, 50};
    SDL_RenderFillRect(renderer, &inside9);
}
void render()
{

    SDL_SetRenderDrawColor(renderer, 0, 102, 102, 255);
    SDL_RenderClear(renderer);

    if (!gameOver)
    {
        if (level == 2)
        {
            createobstacle();
        }
        if (level == 3)
        {
            sideobstacle();
            insideobtacle1();
        }
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        snakerend();

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        foodrend();
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        sfoodrend();

        // Render the score
        string scoreText = "Score: " + to_string(score);
        SDL_Surface *surface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect scoreRect = {10, 10, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &scoreRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        SDL_RenderPresent(renderer);
    }
    else
    {
        finished = SDL_LoadBMP("pic/over.bmp");
        g_finished = SDL_CreateTextureFromSurface(renderer, finished);
        SDL_FreeSurface(finished);
        SDL_RenderCopy(renderer, g_finished, NULL, NULL);
        SDL_DestroyTexture(g_finished);
        renderGameOver();
        resetsnake();
        SDL_RenderPresent(renderer);

        // SDL_Delay(15000);
        // SDL_Quit();
    }
}

// button clicked or not........................................

void renderStartButton()
{
    SDL_Rect startButtonRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 5, 200, 50};
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // SDL_RenderFillRect(renderer, &startButtonRect);

    TTF_Font *buttonFont = TTF_OpenFont("shawon.ttf", 35);
    SDL_Color buttonTextColor = {0, 0, 0, 0};
    string buttonText = "play-game";

    SDL_Surface *surface = TTF_RenderText_Solid(buttonFont, buttonText.c_str(), buttonTextColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {startButtonRect.x + (startButtonRect.w - surface->w) / 2,
                         startButtonRect.y + (startButtonRect.h - surface->h) / 2,
                         surface->w,
                         surface->h};

    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(buttonFont);
}
bool isStartButtonClicked(int mouseX, int mouseY)
{

    SDL_Rect startButtonRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 5, 200, 50};

    return isPointInsideRect(mouseX, mouseY, startButtonRect);
}

void renderExitButton()
{
    SDL_Rect exitButtonRect = {200, 300, 200, 50};
    // SDL_RenderFillRect(renderer, &exitButtonRect);

    TTF_Font *buttonFont = TTF_OpenFont("shawon.ttf", 40);
    SDL_Color buttonTextColor = {255, 0, 0, 255};
    string buttonText = "Exit";

    SDL_Surface *surface = TTF_RenderText_Solid(buttonFont, buttonText.c_str(), buttonTextColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {exitButtonRect.x + (exitButtonRect.w - surface->w) / 2,
                         exitButtonRect.y + (exitButtonRect.h - surface->h) / 2,
                         surface->w,
                         surface->h};

    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(buttonFont);
}
bool isExitButtonClicked(int mouseX, int mouseY)
{
    SDL_Rect exitButtonRect = {200, 300, 200, 50};
    return isPointInsideRect(mouseX, mouseY, exitButtonRect);
}

void easybutton()
{
    SDL_Rect easybuttonrect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 70, 200, 50};
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // SDL_RenderFillRect(renderer, &easybuttonrect);

    TTF_Font *buttonFont = TTF_OpenFont("shawon.ttf", 40);
    SDL_Color buttonTextColor = {0, 0, 0, 0};
    string buttonText = "EASY";

    SDL_Surface *surface = TTF_RenderText_Solid(buttonFont, buttonText.c_str(), buttonTextColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {easybuttonrect.x + (easybuttonrect.w - surface->w) / 2,
                         easybuttonrect.y + (easybuttonrect.h - surface->h) / 2,
                         surface->w,
                         surface->h};

    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(buttonFont);
}
bool iseasybuttonclicked(int mouseX, int mouseY)
{

    SDL_Rect easybuttonrect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 70, 200, 50};

    return isPointInsideRect(mouseX, mouseY, easybuttonrect);
}

void mediumbutton()
{
    SDL_Rect mediumrect = {200, SCREEN_HEIGHT / 2, 200, 50};
    // SDL_RenderFillRect(renderer, &exitButtonRect);

    TTF_Font *buttonFont = TTF_OpenFont("shawon.ttf", 40);
    SDL_Color buttonTextColor = {255, 0, 0, 255};
    string buttonText = "MEDIUM";

    SDL_Surface *surface = TTF_RenderText_Solid(buttonFont, buttonText.c_str(), buttonTextColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {mediumrect.x + (mediumrect.w - surface->w) / 2,
                         mediumrect.y + (mediumrect.h - surface->h) / 2,
                         surface->w,
                         surface->h};

    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(buttonFont);
}
bool ismediumbuttonclicked(int mouseX, int mouseY)
{

    SDL_Rect mediumrect = {200, SCREEN_HEIGHT / 2, 200, 50};

    return isPointInsideRect(mouseX, mouseY, mediumrect);
}

void hardbutton()
{
    SDL_Rect hardbuttonract = {200, SCREEN_HEIGHT / 2 + 70, 200, 50};
    // SDL_RenderFillRect(renderer, &exitButtonRect);

    TTF_Font *buttonFont = TTF_OpenFont("shawon.ttf", 40);
    SDL_Color buttonTextColor = {0, 0, 0, 0};
    string buttonText = "HARD";

    SDL_Surface *surface = TTF_RenderText_Solid(buttonFont, buttonText.c_str(), buttonTextColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {hardbuttonract.x + (hardbuttonract.w - surface->w) / 2,
                         hardbuttonract.y + (hardbuttonract.h - surface->h) / 2,
                         surface->w,
                         surface->h};

    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(buttonFont);
}
bool ishardbuttonclicked(int mouseX, int mouseY)
{

    SDL_Rect hardbuttonract = {200, SCREEN_HEIGHT / 2 + 70, 200, 50};

    return isPointInsideRect(mouseX, mouseY, hardbuttonract);
}

void mutebutton()
{
    SDL_Rect mute = {5, 5, 20, 20};
    msurface = SDL_LoadBMP("pic/m.bmp");
    mtexture = SDL_CreateTextureFromSurface(renderer, msurface);
    SDL_FreeSurface(msurface);
    SDL_RenderCopy(renderer, mtexture, NULL, &mute);
    SDL_DestroyTexture(mtexture);
}
bool ismutebuttonclicked(int mouseX, int mouseY)
{
    SDL_Rect mute = {5, 5, 20, 20};
    return isPointInsideRect(mouseX, mouseY, mute);
}

void gameloop()
{
    SDL_Event e;
    bool quit = false;
    int phase = 0;
    int mcnt = 0;
    // music
    bgm = Mix_LoadMUS("music/bgm.mp3");
    Mix_PlayMusic(bgm, -1);
    test = Mix_LoadMUS("music/prac.mp3");
    ne = Mix_LoadMUS("music/ne.mp3");
    eat=Mix_LoadWAV("music/eating.wav");


    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {

                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    if (snake.direction != 'D')
                        snake.direction = 'U';
                    break;
                case SDLK_DOWN:
                    if (snake.direction != 'U')
                        snake.direction = 'D';
                    break;
                case SDLK_LEFT:
                    if (snake.direction != 'R')
                        snake.direction = 'L';
                    break;
                case SDLK_RIGHT:
                    if (snake.direction != 'L')
                        snake.direction = 'R';
                    break;
                case SDLK_SPACE:
                    if (gameOver != false)
                    {
                        gameOver = false;
                        score = 0;
                        phase = 1;
                        Mix_PlayMusic(bgm, -1);
                    }
                    break;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (isStartButtonClicked(mouseX, mouseY) && phase == 0)
                {
                    phase = 1;
                    
                }
                else if (ismutebuttonclicked(mouseX, mouseY))
                {
                    mcnt++;
                    if (mcnt % 2 == 1)
                    {
                        Mix_PauseMusic();
                    }
                    else
                    {
                        Mix_ResumeMusic();
                    }
                }
                else if (iseasybuttonclicked(mouseX, mouseY) && phase == 1)
                {
                    Mix_HaltMusic();
                    Mix_PlayMusic(ne, -1);
                    phase = 2;
                    level = 1;
                }
                else if (isExitButtonClicked(mouseX, mouseY) && phase == 0)
                {
                    quit = true;
                }
                else if (ishardbuttonclicked(mouseX, mouseY) && phase == 1)
                {
                    Mix_HaltMusic();
                    Mix_PlayMusic(ne, -1);
                    phase = 2;
                    level = 3;
                }
                else if (ismediumbuttonclicked(mouseX, mouseY) && phase == 1)
                {
                    Mix_HaltMusic();
                    Mix_PlayMusic(ne, -1);
                    phase = 2;
                    level = 2;
                }
            }
        }

        if (phase == 0)
        {

            SDL_SetRenderDrawColor(renderer, 0, 102, 102, 255);
            SDL_RenderClear(renderer);
            p_surface = SDL_LoadBMP("pic/background.bmp");
            p_texture = SDL_CreateTextureFromSurface(renderer, p_surface);
            SDL_FreeSurface(p_surface);
            SDL_RenderCopy(renderer, p_texture, NULL, NULL);

            mutebutton();
            renderStartButton();
            renderExitButton();

            SDL_RenderPresent(renderer);
            SDL_DestroyTexture(p_texture);
        }
        else if (phase == 1)
        {
            SDL_SetRenderDrawColor(renderer, 0, 102, 102, 255);
            SDL_RenderClear(renderer);
            phase_1 = SDL_LoadBMP("pic/mode.bmp");
            Phase_t1 = SDL_CreateTextureFromSurface(renderer, phase_1);
            SDL_FreeSurface(phase_1);
            SDL_RenderCopy(renderer, Phase_t1, NULL, NULL);
            // add music here
            mutebutton();
            easybutton();
            mediumbutton();
            hardbutton();
            SDL_RenderPresent(renderer);
            SDL_DestroyTexture(Phase_t1);
        }
        else if (phase == 2)
        {
          update();
          render();
        }
        if(level==1)
        {
            SDL_Delay(80);
        }
        else if(level==2)
        {
             SDL_Delay(50);
        }
        else if(level==3)
        {
              SDL_Delay(40);
        }
    }
}

int main(int argc, char *argv[])
{
    initializepart();
    gameloop();

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_FreeMusic(test);
    Mix_FreeMusic(ne);
   // Mix_FreeMusic(eat);

    Mix_FreeMusic(bgm);
    Mix_FreeChunk(eat);
    TTF_Quit();
    SDL_Quit();
    Mix_Quit();
    return 0;
}
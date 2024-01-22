#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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
SDL_Surface *finished = nullptr;
SDL_Texture *g_finished = nullptr;
SDL_Surface *phase_1 = nullptr;
SDL_Texture *Phase_t1 = nullptr;
SDL_Surface *msurface = nullptr;
SDL_Texture *mtexture = nullptr;
int highscore=0;
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
int cnt=0;
bool gameOver = false;
// creating a food in random place............................................
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
// initialize winddow,renderer and ttf...............................
void initializepart()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

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
    sfruit.x=-10;
    sfruit.y=-20;
}

// update .....................................................................
void update()
{
      int recent_time=SDL_GetTicks();
      if(recent_time-current_time>=6000)
      {
        sfruit.x=-10;
        sfruit.y=-10;
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

    if (newsnake.first == fruit.x && newsnake.second == fruit.y)
    {
        foodplace();
        score += 5;
        cnt++;
        if(cnt%7==0)
        {
            sfoodplace();
        }
    }
    else  if (newsnake.first == sfruit.x && newsnake.second == sfruit.y)
    {
        score += 10;
        sfruit.x=-100;
        sfruit.y=-10;
    }
    else
    {
        snake.body.pop_back();
    }
      

}
//
void renderGameOver()
{
    ifstream input("highscore1.txt");
    input>>highscore;

    ofstream output("highscore1.txt");
    if(score>highscore)
    {
        output<<score;
    }
    else
    {
        output<<highscore;
    }

    TTF_Font *finalsecore = TTF_OpenFont("final.ttf", 30);
    string highscoretxt;
    if(score>highscore)
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
    SDL_Rect finalScoreRect = {(SCREEN_WIDTH - finalScoreSurface->w) / 2, (SCREEN_HEIGHT / 3)+50, finalScoreSurface->w, finalScoreSurface->h};
    SDL_RenderCopy(renderer, finalScoreTexture, nullptr, &finalScoreRect);

     SDL_Surface *highscoresurface = TTF_RenderText_Solid(finalsecore, highscoretxt.c_str(), gtextColor);
    SDL_Texture *highscoretexture = SDL_CreateTextureFromSurface(renderer, highscoresurface);
    SDL_Rect highscorerect = {(SCREEN_WIDTH - highscoresurface->w) / 2, (SCREEN_HEIGHT / 2)+20, highscoresurface->w, highscoresurface->h};
    SDL_RenderCopy(renderer, highscoretexture, nullptr, &highscorerect);


   // SDL_FreeSurface(gameOverSurface);
   // SDL_DestroyTexture(gameOverTexture);
    TTF_CloseFont(finalsecore);

    SDL_FreeSurface(finalScoreSurface);
    SDL_DestroyTexture(finalScoreTexture);
}
void snakerend()
{
    for (const auto &segment : snake.body)
    {
        SDL_Rect rect = {segment.first, segment.second, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }
}
void foodrend()
{
    SDL_Rect foodRect = {fruit.x, fruit.y, 10, 10};
   SDL_RenderFillRect(renderer, &foodRect);
}
void sfoodrend()
{
     SDL_Rect sfoodRect = {sfruit.x, sfruit.y, 10, 10};
    SDL_RenderFillRect(renderer, &sfoodRect);
}
void resetsnake()
{
     snake.body.clear();
          snake.body.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    snake.direction = 'R';
    foodplace();
     sfruit.x=-10;
     sfruit.y=-20;
}
void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 102, 102, 255);
    SDL_RenderClear(renderer);

    if (!gameOver)
    {
               /* SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_Rect obstacle={50,70,100,15};
        SDL_RenderFillRect(renderer,&obstacle);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_Rect obstacle1={50,85,15,85};
        SDL_RenderFillRect(renderer,&obstacle1);*/
        
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
        renderGameOver();
        resetsnake();
        SDL_RenderPresent(renderer);
       
        //SDL_Delay(15000);
        //SDL_Quit();
    }
}

bool isPointInsideRect(int x, int y, const SDL_Rect &rect)
{
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
}
// button clicked or not........................................
bool isStartButtonClicked(int mouseX, int mouseY)
{

    SDL_Rect startButtonRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 5, 200, 50};

    return isPointInsideRect(mouseX, mouseY, startButtonRect);
}

void renderStartButton()
{
    SDL_Rect startButtonRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 5, 200, 50};
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // SDL_RenderFillRect(renderer, &startButtonRect);

    TTF_Font *buttonFont = TTF_OpenFont("shawon.ttf",35);
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
    SDL_Rect easybuttonrect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 25, 200, 50};
     //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
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

    SDL_Rect easybuttonrect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 25, 200, 50};

    return isPointInsideRect(mouseX, mouseY, easybuttonrect);
}
void hardbutton()
{
    SDL_Rect hardbuttonract = {200, 300, 200, 50};
    // SDL_RenderFillRect(renderer, &exitButtonRect);

    TTF_Font *buttonFont = TTF_OpenFont("shawon.ttf", 40);
    SDL_Color buttonTextColor = {255, 0, 0, 255};
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

    SDL_Rect hardbuttonract = {200, 300, 200, 50};

    return isPointInsideRect(mouseX, mouseY, hardbuttonract);
}
bool ismutebuttonclicked(int mouseX, int mouseY)
{
    SDL_Rect mute = {5, 5, 20, 20};
    return isPointInsideRect(mouseX, mouseY, mute);
}
void mutebutton()
{
         SDL_Rect mute={5,5,20,20};
            msurface=SDL_LoadBMP("pic/m.bmp");
            mtexture = SDL_CreateTextureFromSurface(renderer, msurface);
            SDL_RenderCopy(renderer, mtexture, NULL, &mute);
                SDL_FreeSurface(msurface);
}
void gameloop()
{
    SDL_Event e;
    bool quit = false;
    int phase = 0;
    int mcnt=0;
    bool mode=false;
    // music
    bgm = Mix_LoadMUS("music/bgm.mp3");
    Mix_PlayMusic(bgm, -1);
    test = Mix_LoadMUS("music/prac.mp3");
    ne = Mix_LoadMUS("music/ne.mp3");

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
                    if(gameOver!=false)
                    {
                        gameOver=false;
                        score=0;
                        phase=1;
                        Mix_PlayMusic(bgm, -1);

                    }
                    break;


                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (isStartButtonClicked(mouseX, mouseY)&&phase==0)
                {
                    phase = 1;
                    // stop music...
                    //   Mix_HaltMusic();
                    //   Mix_PlayMusic(test,0);
                    //     Mix_PlayMusic(ne,-1);
                }
                else if(ismutebuttonclicked(mouseX, mouseY))
                {
                    mcnt++;
                    if(mcnt%2==1)
                    {
                       Mix_PauseMusic();
                    }
                    else{
                        Mix_ResumeMusic();
                    }

                }
                else if (iseasybuttonclicked(mouseX, mouseY)&& phase==1)
                {
                    Mix_HaltMusic();
                     Mix_PlayMusic(ne,-1);
                    phase = 2;
                    mode=true;
                }
                else if (isExitButtonClicked(mouseX, mouseY)&& phase==0)
                {
                    quit = true;
                }
                else if(ishardbuttonclicked(mouseX,mouseY)&& phase==1)
                {
                     Mix_HaltMusic();
                     Mix_PlayMusic(ne,-1);
                    phase = 2;
                  mode=false;
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
            hardbutton();

            SDL_RenderPresent(renderer);

        }
        else if (phase == 2)
        {
           if(mode)
           {
             update();
            render();
           }
           else{
             update();
            render();
           }
        }

        SDL_Delay(80);
    }
}

int main(int argc, char *argv[])
{
    initializepart();
    gameloop();

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(Phase_t1);
    SDL_DestroyTexture(mtexture);

    Mix_FreeMusic(test);
    Mix_FreeMusic(ne);

    Mix_FreeMusic(bgm);

    TTF_Quit();
    SDL_Quit();
    Mix_Quit();
    return 0;
}

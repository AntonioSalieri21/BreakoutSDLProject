#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL_mixer.h>

typedef struct 
{
    double x;
    double y;
} Vector;

typedef struct
{
    float x;
    float y;
    int size;
    Vector dir;
} Ball;

typedef struct 
{
    int score;
    float xPosition;
    SDL_Texture* texture;

} Player;

typedef struct 
{
    SDL_Rect rect;
    int score;
    int HP;
    int maxHP;
    int index;
    bool isDead;

} Brick;

typedef struct 
{
    SDL_Rect rect;
    SDL_Texture* texture;

} Button;

//Initializing functions

bool Initialize();
Ball makeBall(int size);
Player MakePlayer();
void LoadLevelFromFile();
Brick CreateBrick(int x, int y, int maxHP, int index);
void LoadTileTextures();
void CreateButtons();
void SetDifficulty(int diff);
void LoadFaceTextures();
void MenuInitialize();
void GameInitialize();

//Update functions

void Update(float elapsed);
void UpdateBall(Ball *ball, float elapsed);
void UpdateBricks(Ball *ball);
void UpdatePlayer(float elapsed);
void ButtonAction(int n);
void UpdateButtons();

//Render functions

void Render();
void RenderBall(Ball* ball);
void RenderBricks();
void RenderPlayer();
void RenderBackground();
void RenderText(TTF_Font* font, char* text, int x, int y, int text_height, int text_width);
void MainRender();
void RenderMainButtons();
void ShowStats();

//Additional functions

void Shutdown();
bool IsQuit();
int num_of_lines(FILE* file);
void MainMenu(float elapsed, SDL_Event *event);
bool IsMenuQuit();
void MainMenuCycle();
void GameCycle();



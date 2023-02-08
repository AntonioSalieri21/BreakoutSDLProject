#include "header.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const int BALL_SIZE = 12;

const int PLAYER_HEIGHT = 10;
const int PLAYER_MARGIN = 10;

const int BRICK_WIDTH = 50;
const int BRICK_HEIGHT = 25;
const int MARGIN = 50;
const int BRICK_SPACE = 7;
const int BUTTON_WIDTH = 500;
const int BUTTON_HEIGHT = 50;
const int BUTTON_MARGIN = 100;
const int BUTTON_SPACE = 100;

const char* LEVEL_PATH = "level.txt";
const char* NO_SOUND_TEXTURE_PATH = "PNG/no-sound.png";
const char* SOUND_TEXTURE_PATH = "PNG/sound.png";
const char* BACKGROUND_MUSIC_PATH = "Music/bg_music.mp3";



Ball ball;
Player player1;
Brick* bricks; 
Button buttons[5];

SDL_Window* window;
SDL_Renderer* renderer;

const SDL_Color White = {255,255,255};

TTF_Font* Contra;

Mix_Chunk* paddle_sound;
Mix_Chunk* brick_hit_sound;
Mix_Chunk* brick_break_sound;
Mix_Chunk* wall_hit_sound;

Mix_Music* music;

SDL_Texture* background;
SDL_Texture* brick_textures[6];
SDL_Texture* button_textures[5];
SDL_Texture* face_textures[4];

bool quit = false;
int bricks_count = 40;
double angle = 90;
int paddle_dir= 0;

int DIFFICULTY_MODE = 0;
int PLAYER_WIDTH;
float BALL_SPEED;
int BALLS_COUNT;
int SCORE;
bool PLAY_SOUND = true;
float PLAYER_MOVE_SPEED = 300;

bool game_start = true;
bool MainMenuQuit = false;


//Initializing functions

bool Initialize(){

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Failes to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(!window)
        return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!window)
        return false;

    Mix_Init(0);
    TTF_Init();
    background = IMG_LoadTexture(renderer,"PNG/background.jpg");
    Contra = TTF_OpenFont("Contra.ttf", 30);

    if(PLAY_SOUND)
    {
        
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024); //44100

        paddle_sound = Mix_LoadWAV("SFX/sfx_paddle_hit.wav");
        brick_hit_sound = Mix_LoadWAV("SFX/sfx_brick_hit.wav");
        brick_break_sound = Mix_LoadWAV("SFX/sfx_brick_break.wav");
        wall_hit_sound = Mix_LoadWAV("SFX/sfx_wall_hit.wav");

    }
    
    return true;
}

Ball makeBall(int size)
{
    Vector dir = {0, 0};
    Ball ball = {
        .x = SCREEN_WIDTH / 2 - size / 2,
        .y = SCREEN_HEIGHT - PLAYER_HEIGHT - BALL_SIZE - PLAYER_MARGIN,
        .size = size,
        .dir= dir
    };
    return ball;
}

Player MakePlayer(){

    Player player = {
        .xPosition = SCREEN_WIDTH/2 - PLAYER_WIDTH/2,
        .texture = IMG_LoadTexture(renderer, "PNG/paddle.png")
    };

    return player;
}

void LoadLevelFromFile()
{
    bricks_count = 0;
    int i = 0;
    FILE* level = fopen(LEVEL_PATH, "r");
    int lines = num_of_lines(level);
    bricks = malloc(sizeof(Brick) * 30 * lines);
    for(int y = 0; y < lines; y++)
    {
        char string[30];
        fgets(string, 30, level);
        for(int x = 0; x < strlen(string) + 1; x++)
        {
            if(string[x] != ' ' && string[x] != '\0' && string[x] != '\n')
            {
                int maxHP = 0;
                if(string[x] == '#')
                {
                    maxHP = 9999;
                }
                else
                {
                    maxHP = (string[x] - 48);
                    
                }

                bricks[i] = CreateBrick(x, y, maxHP, i);
                i++;
                bricks_count++;
                
            }

        }
        
    }

    fclose(level);
}

Brick CreateBrick(int x, int y, int maxHP, int index)
{
    SDL_Rect tmp = {
        .x = MARGIN + x * BRICK_WIDTH + x * BRICK_SPACE,
        .y = MARGIN + y * BRICK_WIDTH + y * BRICK_SPACE,
        .w = BRICK_WIDTH,
        .h = BRICK_HEIGHT
    };
    Brick newBrick = {tmp,maxHP, maxHP, maxHP, index, false};

    return newBrick;
}

void CreateButtons()
{
    for(int i = 0; i < 4; i++)
    {
        SDL_Rect tmp_rect = {
            .x = SCREEN_WIDTH/4 - BUTTON_WIDTH/2,
            .y = BUTTON_MARGIN + (BUTTON_HEIGHT * i) + (BUTTON_SPACE * i),
            .w = BUTTON_WIDTH,
            .h = BUTTON_HEIGHT
        };
        char path[30];
        sprintf(path, "PNG/button_%d.png",i+1);
        button_textures[i] = IMG_LoadTexture(renderer, path);
        Button tmp = {tmp_rect, button_textures[i]};
        buttons[i] = tmp;
    }

    SDL_Rect vol_button = {
        .x = SCREEN_WIDTH-50,
        .y = 0, 
        .w = 50,
        .h = 50
    };
    SDL_Texture* vol_texture = IMG_LoadTexture(renderer, "PNG/sound.png");
    Button tmp = {vol_button, vol_texture };
    buttons[4] = tmp;
}

void LoadTileTextures()
{
    for(int i = 0; i < 6; i++)
    {
        char path[30];
        sprintf(path, "PNG/tile%d.png", i + 1);
        brick_textures[i] = IMG_LoadTexture(renderer, path);
    }
}

void LoadFaceTextures()
{
    for(int i = 0; i < 4; i++)
    {
        char path[50];
        sprintf(path, "PNG/face_%d.png", i+1);
        face_textures[i] = IMG_LoadTexture(renderer, path);
    }
}

void SetDifficulty(int diff)
{
    switch (diff)
    {
        case 0:
            PLAYER_WIDTH = SCREEN_WIDTH / 1.5;
            BALL_SPEED = 200;
            BALLS_COUNT = 999;
            PLAYER_MOVE_SPEED = 600;
            break;

        case 1:
            PLAYER_WIDTH = SCREEN_WIDTH / 3;
            BALL_SPEED = 300;
            BALLS_COUNT = 10;
            PLAYER_MOVE_SPEED = 500;
            break;

        case 2:
            PLAYER_WIDTH = SCREEN_WIDTH / 8;
            BALL_SPEED = 600;
            BALLS_COUNT = 3;
            PLAYER_MOVE_SPEED = 400;
            break;

        case 3:
            PLAYER_WIDTH = BALL_SIZE * 2;
            BALL_SPEED = 1000;
            BALLS_COUNT = 10;
            music = Mix_LoadMUS(BACKGROUND_MUSIC_PATH);
            PLAYER_MOVE_SPEED = 1000;
            break;

        
        default:
            break;
    }
}

void GameInitialize()
{
    SetDifficulty(DIFFICULTY_MODE);

    ball = makeBall(BALL_SIZE);
    player1 = MakePlayer();

    LoadTileTextures();
    LoadLevelFromFile();
    if(music != NULL && PLAY_SOUND)
        Mix_PlayMusic(music, -1);
}

void MenuInitialize()
{
    CreateButtons();
    LoadFaceTextures();
}



//Update functions

void Update(float elapsed){
    UpdatePlayer(elapsed);
    UpdateBall(&ball, elapsed);
    UpdateBricks(&ball);

}

void UpdateBall(Ball *ball, float elapsed){

    SDL_Rect ball_rect = {
        .x = ball->x,
        .y = ball->y,
        .w = BALL_SIZE,
        .h = BALL_SIZE
    };

    SDL_Rect paddle = {
        .x = (int)(player1.xPosition),
        .y = SCREEN_HEIGHT - PLAYER_MARGIN - PLAYER_HEIGHT,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT
    };

    if( ball->y + BALL_SIZE >= SCREEN_HEIGHT) 
    {
        if(BALLS_COUNT == 0)
            quit = true;
        else
            BALLS_COUNT--;
        
        ball->x = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
        ball->y = SCREEN_HEIGHT - PLAYER_HEIGHT - BALL_SIZE - PLAYER_MARGIN;
        ball->dir.x = 0;
        ball->dir.y = 0;
        player1.xPosition = SCREEN_WIDTH/2 - PLAYER_WIDTH/2;
        game_start = true;
        
      
    } 
    if(ball->y <= BALL_SIZE)
    {
        if(PLAY_SOUND)
            Mix_PlayChannel(-1, wall_hit_sound, 0);
        ball->dir.y *= -1;
        int dy = abs(ball->y);
        ball->y += dy;
    }
    if(ball->x <= BALL_SIZE)
    {
        if(PLAY_SOUND)
            Mix_PlayChannel(-1, wall_hit_sound, 0);
        ball->dir.x *= -1;
        int dx = abs(ball->x);
        ball->x += dx;
    }
    if(ball->x >= SCREEN_WIDTH - BALL_SIZE)
    {
        if(PLAY_SOUND)
            Mix_PlayChannel(-1, wall_hit_sound, 0);
        ball->dir.x *= -1;
        int dx = (ball->x + BALL_SIZE) - SCREEN_WIDTH;
        ball->x -= dx;
    }

    if(SDL_HasIntersection(&ball_rect, &paddle)) //BALL ON PLAYER
    {
        Mix_PlayChannel(-1, paddle_sound, 0);
        int direction = 1;

        if(ball->dir.x > 0)
            direction = 1;
        else
            direction = -1;

        double rel = (paddle.x + paddle.w/2) - ball_rect.x;
        double angle;

        if(rel > 90)
            angle = rel - 90;
        else
            angle = rel + 90;

        double norm = angle/(paddle.w/2);

        ball->dir.y = -cos(norm); 
        ball->dir.x = sin(norm * direction);

        if(ball->dir.y > 0)
            ball->dir.y *= -1;

        ball->y += ball->dir.y * BALL_SPEED * elapsed;


        if(paddle_dir == 1)
            ball->dir.x = fabs(ball->dir.x);
        else if(paddle_dir != 0)
            ball->dir.x = -fabs(ball->dir.x);
        
    }

    for(int i = 0; i < bricks_count; i++)
    {
            
        if(SDL_HasIntersection(&ball_rect, &(bricks[i].rect)) &&  !(bricks[i].isDead)) //BALL ON BRICK
        {
            Brick b = bricks[i];
            float epsilon = 1;
            float dx, dy;
            if(PLAY_SOUND)
                Mix_PlayChannel(-1, brick_hit_sound, 0);

            int dir_x, dir_y;
            if(ball->dir.x > 0)
            {
                dx = (ball->x + BALL_SIZE) - b.rect.x; //ball right - brick left
                dir_x = -dx;
            }
            else
            {
                dx = (b.rect.x + BRICK_WIDTH) - ball->x; // brick right - ball left
                dir_x = dx;
            }
            
            if(ball->dir.y > 0)
            {
                dy = (ball->y + BALL_SIZE) - b.rect.y; // ball bottom - brick top
                dir_y = -dy;
            }
            else
            {
                dy = (b.rect.y + BRICK_HEIGHT) - ball->y; // brick bottom - ball top
                dir_y = dy;
            }

            if(abs(dx - dy) < epsilon)
            {
                ball->dir.x *= -1;
                ball->dir.y *= -1;
            }

            if(dx > dy) {ball->dir.y *= -1; ball->y += dir_y;} 
            else {ball->dir.x *= -1; ball->x += dir_x;}

            bricks[i].HP -= 1;
            break; 
        }
        
    }


        ball->x += ball->dir.x * BALL_SPEED * elapsed;
        ball->y += ball->dir.y * BALL_SPEED * elapsed;

}

void UpdatePlayer(float elapsed){
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    paddle_dir = 0;
    if(keyboardState[SDL_SCANCODE_LEFT])
    {
        
        if(player1.xPosition > 0)
        {
            player1.xPosition -= PLAYER_MOVE_SPEED * elapsed;
            paddle_dir= -1;
        }
    }
    if(keyboardState[SDL_SCANCODE_RIGHT])
    {
        
        if(player1.xPosition < SCREEN_WIDTH - PLAYER_WIDTH)
        {
            player1.xPosition += PLAYER_MOVE_SPEED * elapsed;
            paddle_dir= 1;
        }
    }

    if(ball.dir.x == 0 && paddle_dir!= 0 && game_start)
    {
        ball.dir.x = paddle_dir;
        ball.dir.y = -1;
        game_start = false;
    }



}

void UpdateBricks(Ball *ball)
{
    
    for(int i = 0; i < bricks_count; i++)
    {
        
        if(bricks[i].HP <= 0 && !(bricks[i].isDead) && bricks[i].maxHP != 9999)
        {
            
            bricks[i].isDead = true;
            SCORE += bricks[i].maxHP;

        }
            

    }
    for(int i = 0; i < bricks_count; i++)
    {
        if(!(bricks[i].isDead) && !(bricks[i].maxHP == 9999))
            return;
    }

    quit = true;
}

void UpdateButtons()
{
    SDL_Point mouse_pos;
 
    SDL_GetMouseState(&(mouse_pos.x), &(mouse_pos.y));
    for(int i = 0; i < 5; i++)
    {
        if(SDL_PointInRect(&mouse_pos, &(buttons[i].rect)))
        {
            ButtonAction(i);
        }
            
    } 
        
    
}

void ButtonAction(int n)
{
    switch (n)
    {
        case 0:
            DIFFICULTY_MODE = n;
            MainMenuQuit = true;
            break;
        case 1:
            DIFFICULTY_MODE = n;
            MainMenuQuit = true;
            break;

        case 2:
            DIFFICULTY_MODE = n;
            MainMenuQuit = true;
            break;

        case 3:
            DIFFICULTY_MODE = n;
            MainMenuQuit = true;
            break;
        
        case 4:
            if(PLAY_SOUND)
                buttons[4].texture = IMG_LoadTexture(renderer, SOUND_TEXTURE_PATH);
            else
                buttons[4].texture = IMG_LoadTexture(renderer, NO_SOUND_TEXTURE_PATH);

            PLAY_SOUND = !PLAY_SOUND;

        default:
            break;
    }

}

//Render functions

void Render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    RenderBackground();
    RenderPlayer();
    RenderBall(&ball);
    RenderBricks();

    ShowStats();

    SDL_RenderPresent(renderer);

}

void ShowStats()
{
    char score[20];
    char balls[20];
    sprintf(score, "Score: %d", SCORE);
    sprintf(balls, "Balls: %d", BALLS_COUNT);
    RenderText(Contra, score, MARGIN, 10, 40, 30 * strlen(score));
    RenderText(Contra, balls, SCREEN_WIDTH - MARGIN - 30 * strlen(score), 10, 40, 30 * strlen(score));
}

void RenderBackground()
{
    SDL_RenderCopy(renderer, background, NULL, NULL);
}

void RenderBall(Ball* ball)
{
    int size = ball->size;
    SDL_Rect rect = {
        .x = ball->x,
        .y = ball->y,
        .w = size,
        .h = size
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void RenderPlayer(void){

    SDL_Rect paddle = {
        .x = (int)(player1.xPosition),
        .y = SCREEN_HEIGHT - PLAYER_MARGIN - PLAYER_HEIGHT,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT
    };

    SDL_RenderCopy(renderer,player1.texture,NULL, &paddle);
}

void RenderBricks()
{

    for(int i = 0; i < bricks_count; i++)
    {
        if(!bricks[i].isDead)
        {
            Brick b = bricks[i];
            int texture_numb;
            if(b.maxHP == 9999)
                texture_numb = 0;
            else
                texture_numb = b.maxHP - b.HP;

            SDL_Rect texture_rect = {
                .x = 0,
                .y = 231 * texture_numb,
                .w = 792,
                .h = 231
            };
            SDL_Texture* texture;
            if(b.maxHP == 9999)
                texture = brick_textures[5];
            else
                texture = brick_textures[b.maxHP-1];

            SDL_RenderCopy(renderer, texture, &texture_rect, &b.rect);

        }
        
    }
}

void RenderText(TTF_Font* font, char* text, int x, int y, int text_height, int text_width)
{
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, White);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect rect = {
        .x = x,
        .y = y,
        .w = text_width,
        .h = text_height
    };
    SDL_RenderCopy(renderer, Message, NULL, &rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void MainRender()
{
    SDL_RenderClear(renderer);

    RenderBackground();
    RenderMainButtons();

    SDL_RenderPresent(renderer);
}

void RenderMainButtons()
{
    SDL_Point mouse_pos;
    for(int i = 0; i < 4; i++)
    {
        SDL_Rect texture_rect;
        SDL_GetMouseState(&(mouse_pos.x), &(mouse_pos.y));
        if(SDL_PointInRect(&mouse_pos, &(buttons[i].rect)))
        {
            SDL_Rect tmp = {
                .x = 15,
                .y = 400,
                .h = 605,
                .w = 6000
            };
            texture_rect = tmp;

            SDL_Rect face_rect = {
                .x = SCREEN_WIDTH/2 + 100,
                .y = 200,
                .h = 400,
                .w = 400
            };
            SDL_RenderCopy(renderer, face_textures[i], NULL, &face_rect);
            
        }
        else
        {
            SDL_Rect tmp = {
                .x = 45,
                .y = 1175,
                .h = 605,
                .w = 6000
            };
            texture_rect = tmp;
        }
        SDL_RenderCopy(renderer, buttons[i].texture, &texture_rect, &(buttons[i].rect));
    
        
    }
    SDL_RenderCopy(renderer, buttons[4].texture, NULL, &(buttons[4].rect)); 
}


//Additional functions

void Shutdown(void){
    if(renderer){SDL_DestroyRenderer(renderer);}
    if(window){SDL_DestroyWindow(window);}
    TTF_CloseFont(Contra);
    
    for(int i = 0; i < 6; i++)
        SDL_DestroyTexture(brick_textures[i]);

    free(bricks);
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

bool IsMenuQuit(){return MainMenuQuit;}

void MainMenu(float elapsed, SDL_Event *event)
{
    CreateButtons();
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    if(keyboardState[SDL_SCANCODE_LEFT])
    {
        MainMenuQuit = true;
    }
    MainRender();
}

int num_of_lines(FILE* file)
{
    int c = 0;
    int count = 0;
    for(c = getc(file); c != EOF; c = getc(file))
        if(c == '\n')
            count++;

    fseek(file,0,SEEK_SET);
    return count;
}

bool IsQuit(){return quit;}
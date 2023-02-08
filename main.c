#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "header.h"
bool Lquit, MMainMenuQuit;

int main(int argc, char* argv[])
{
    srand(time(NULL));
    if(!Initialize()){ exit(1); }

    
    SDL_Event event;
    
    MainMenuCycle();

    GameCycle();

    Shutdown();
    return 0;
}

void MainMenuCycle()
{
    SDL_Event event;
    MenuInitialize();
    while(!MMainMenuQuit)
    {
        MMainMenuQuit = IsMenuQuit();
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                Lquit = true;
                MMainMenuQuit = true;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                UpdateButtons();
            }
            
        }

        MainRender();
    }
}

void GameCycle()
{
    SDL_Event event;
    GameInitialize();
    Uint32 lastTick = SDL_GetTicks();
    const int FPS = 60;
    while(!Lquit)
    {

        Lquit = IsQuit();
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                Lquit = true;
            }
        }
        Uint32 curTick = SDL_GetTicks();
        Uint32 diff = curTick - lastTick;
        float elapsed = diff / 1000.f;

        if( (double)(1000/FPS) > diff)
            SDL_Delay( (double)(1000/FPS) - diff); //OMEZENI FPS

        Update(elapsed);
        Render();
        lastTick = curTick;

    }

}



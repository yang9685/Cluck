#include <iostream>
#include <__msvc_ostream.hpp>
#include <SDL3/SDL.h>

#include "Base/Types.h"
#include "System/GameActivityBase.h"
#include "System/TimerManager.h"

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* Window = SDL_CreateWindow("Cluck", CLUCK_WINDOW_WIDTH, CLUCK_WINDOW_HEIGHT, 0);
    SDL_Renderer* Renderer = SDL_CreateRenderer(Window, nullptr);

    TimerManager& TimeManager = TimerManager::GetInstance();
    GameActivityBase& GameActivity = GameActivityBase::GetInstance();
    GameActivity.InitializeRenderManager(Renderer);
    GameActivity.WorldBeginPlay();
    /*创建主定时器*/
    const float MainLoopIntervalSeconds = GameActivity.GetMainLoopIntervalSeconds();
    const TimerHandle MainLoopTimerHandle = TimeManager.CreateTimer(
        MainLoopIntervalSeconds,
        true,
        [&GameActivity, MainLoopIntervalSeconds]()
        {
            GameActivity.GameLoop(MainLoopIntervalSeconds);
        });

    while (GameActivity.GetCurrentGameStatus() == Pending)
    {
        TimeManager.Update();
    }
    
    TimeManager.DestroyTimer(MainLoopTimerHandle);
    GameActivity.WorldEndPlay();

    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 0;
}

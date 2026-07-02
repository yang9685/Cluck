#include<iostream>
#include<SDL3/SDL.h>
#include<SDL3/SDL_rect.h>

void Draw_Circle(SDL_Renderer* renderer,int x,int y,int radius);


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Cluck", 1920, 1080, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* Renderer = SDL_CreateRenderer(window,nullptr);
    SDL_Surface* BackGroundSurface = SDL_LoadPNG("Resource/Image/background.png");
    SDL_Texture* BackGroundSurfaceTexture = SDL_CreateTextureFromSurface(Renderer, BackGroundSurface);
    SDL_DestroySurface(BackGroundSurface);

    auto start = SDL_GetTicks();
    bool bIsRunning = true;
    while (bIsRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_ESCAPE)
                {
                    bIsRunning = false;
                }
            }
        }
        
        int width, height;
        SDL_GetWindowSize(window,&width,&height);
        SDL_RenderClear(Renderer);
        SDL_FRect backgroundDst = {0, 0, (float)width, (float)height};\
        SDL_RenderTexture(Renderer, BackGroundSurfaceTexture, nullptr, &backgroundDst);
        SDL_RenderPresent(Renderer);
    }
    auto end = SDL_GetTicks();
    std::cout << end - start;
    SDL_DestroyTexture(BackGroundSurfaceTexture);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


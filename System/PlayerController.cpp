#include "PlayerController.h"

#include <cmath>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>

#include "../GamePlay/Player.h"
#include "GameActivityBase.h"

namespace
{
float RadiansToDegrees(float Radians)
{
    return Radians * 180.0f / 3.14159265358979323846f;
}
}

void PlayerController::Initialize(GameActivityBase& InGameActivity)
{
    GameActivity = &InGameActivity;
    ControlledPlayer = &GameActivity->CreateActor<Player>(PlayerPosition, -90.0f);
}

void PlayerController::Shutdown()
{
    ControlledPlayer = nullptr;
    GameActivity = nullptr;
}

void PlayerController::Update()
{
    SDL_Event Event;
    while (SDL_PollEvent(&Event))
    {
        if (Event.type == SDL_EVENT_QUIT)
        {
            GameActivity->RequestExit();
            continue;
        }

        if (Event.type == SDL_EVENT_KEY_DOWN && Event.key.key == SDLK_ESCAPE)
        {
            GameActivity->RequestExit();
            continue;
        }

    }

    if (ControlledPlayer == nullptr)
    {
        return;
    }

    float MouseX = 0.0f;
    float MouseY = 0.0f;
    const SDL_MouseButtonFlags MouseButtons = SDL_GetMouseState(&MouseX, &MouseY);

    if ((MouseButtons & SDL_BUTTON_LMASK) != 0)
    {
        ControlledPlayer->Fire();
    }

    const Vector2D PlayerCurrentPosition = ControlledPlayer->GetPosition();
    const float DeltaX = MouseX - PlayerCurrentPosition.x;
    const float DeltaY = MouseY - PlayerCurrentPosition.y;
    const float TargetRotation = RadiansToDegrees(std::atan2(DeltaY, DeltaX));

    ControlledPlayer->SetTargetRotation(TargetRotation);
}

void PlayerController::SetPlayerPosition(const Vector2D& InPlayerPosition)
{
    PlayerPosition = InPlayerPosition;
}

const Vector2D& PlayerController::GetPlayerPosition() const
{
    return PlayerPosition;
}

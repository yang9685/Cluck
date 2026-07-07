#pragma once

#include "../Base/Vector2D.h"

class GameActivityBase;
class Player;

class PlayerController
{
private:
    PlayerController() = default;
    ~PlayerController() = default;

    PlayerController(const PlayerController&) = delete;
    PlayerController& operator=(const PlayerController&) = delete;

public:
    static PlayerController& GetInstance()
    {
        static PlayerController Instance;
        return Instance;
    }

    void Initialize(GameActivityBase& InGameActivity);
    void Shutdown();
    void Update();

    void SetPlayerPosition(const Vector2D& InPlayerPosition);
    const Vector2D& GetPlayerPosition() const;

private:
    GameActivityBase* GameActivity = nullptr;
    Player* ControlledPlayer = nullptr;
    Vector2D PlayerPosition = Vector2D(960.0f, 800.0f);
};

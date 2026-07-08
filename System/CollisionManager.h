#pragma once

class CollisionManager
{
private:
    CollisionManager() = default;
    ~CollisionManager() = default;

    CollisionManager(const CollisionManager&) = delete;
    CollisionManager& operator=(const CollisionManager&) = delete;

public:
    static CollisionManager& GetInstance()
    {
        static CollisionManager Instance;
        return Instance;
    }

    void Update();
};

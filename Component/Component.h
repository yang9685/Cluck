#pragma once

class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    virtual void BeginPlay();
    bool HasBegunPlay() const;

private:
    bool bHasBegunPlay = false;
};

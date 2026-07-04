#include "Component.h"

void Component::BeginPlay()
{
    bHasBegunPlay = true;
}

bool Component::HasBegunPlay() const
{
    return bHasBegunPlay;
}

#pragma once
#include <cstdint>
#include <functional>

#define CLUCK_WINDOW_WIDTH 1920
#define CLUCK_WINDOW_HEIGHT 1080

using TimerHandle = uint32_t;
constexpr TimerHandle InvalidTimerHandle = 0;
using TimerCallBack = std::function<void()>;

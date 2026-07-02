#pragma once
#include <cstdint>
#include <functional>

using TimerHandle = uint32_t;
inline constexpr TimerHandle InvalidTimerHandle = 0;
using TimerCallBack = std::function<void()>;

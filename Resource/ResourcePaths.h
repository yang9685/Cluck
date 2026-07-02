#pragma once
#include <array>

namespace Resources
{
namespace Audio
{
inline constexpr char Bgm[] = "Resource/Audio/bgm.mp3";
inline constexpr char Explosion[] = "Resource/Audio/explosion.wav";
inline constexpr char Fire1[] = "Resource/Audio/fire_1.wav";
inline constexpr char Fire2[] = "Resource/Audio/fire_2.wav";
inline constexpr char Fire3[] = "Resource/Audio/fire_3.wav";
inline constexpr char Hurt[] = "Resource/Audio/hurt.wav";
inline constexpr char Loss[] = "Resource/Audio/loss.mp3";
}

namespace Font
{
inline constexpr char IPix[] = "Resource/Font/IPix.ttf";
}

namespace Image
{
inline constexpr char Background[] = "Resource/Image/background.png";
inline constexpr char BarrelIdle[] = "Resource/Image/barrel_idle.png";
inline constexpr char Battery[] = "Resource/Image/battery.png";
inline constexpr char Bullet[] = "Resource/Image/bullet.png";
inline constexpr char Crosshair[] = "Resource/Image/crosshair.png";
inline constexpr char Heart[] = "Resource/Image/heart.png";

inline constexpr char BarrelFire1[] = "Resource/Image/barrel_fire_1.png";
inline constexpr char BarrelFire2[] = "Resource/Image/barrel_fire_2.png";
inline constexpr char BarrelFire3[] = "Resource/Image/barrel_fire_3.png";

inline constexpr char ChickenFast1[] = "Resource/Image/chicken_fast_1.png";
inline constexpr char ChickenFast2[] = "Resource/Image/chicken_fast_2.png";
inline constexpr char ChickenFast3[] = "Resource/Image/chicken_fast_3.png";
inline constexpr char ChickenFast4[] = "Resource/Image/chicken_fast_4.png";

inline constexpr char ChickenMedium1[] = "Resource/Image/chicken_medium_1.png";
inline constexpr char ChickenMedium2[] = "Resource/Image/chicken_medium_2.png";
inline constexpr char ChickenMedium3[] = "Resource/Image/chicken_medium_3.png";
inline constexpr char ChickenMedium4[] = "Resource/Image/chicken_medium_4.png";
inline constexpr char ChickenMedium5[] = "Resource/Image/chicken_medium_5.png";
inline constexpr char ChickenMedium6[] = "Resource/Image/chicken_medium_6.png";

inline constexpr char ChickenSlow1[] = "Resource/Image/chicken_slow_1.png";
inline constexpr char ChickenSlow2[] = "Resource/Image/chicken_slow_2.png";
inline constexpr char ChickenSlow3[] = "Resource/Image/chicken_slow_3.png";
inline constexpr char ChickenSlow4[] = "Resource/Image/chicken_slow_4.png";
inline constexpr char ChickenSlow5[] = "Resource/Image/chicken_slow_5.png";
inline constexpr char ChickenSlow6[] = "Resource/Image/chicken_slow_6.png";
inline constexpr char ChickenSlow7[] = "Resource/Image/chicken_slow_7.png";
inline constexpr char ChickenSlow8[] = "Resource/Image/chicken_slow_8.png";

inline constexpr char Explosion1[] = "Resource/Image/explosion_1.png";
inline constexpr char Explosion2[] = "Resource/Image/explosion_2.png";
inline constexpr char Explosion3[] = "Resource/Image/explosion_3.png";
inline constexpr char Explosion4[] = "Resource/Image/explosion_4.png";
inline constexpr char Explosion5[] = "Resource/Image/explosion_5.png";

inline constexpr std::array<const char*, 3> BarrelFire = {
    BarrelFire1,
    BarrelFire2,
    BarrelFire3,
};

inline constexpr std::array<const char*, 4> ChickenFast = {
    ChickenFast1,
    ChickenFast2,
    ChickenFast3,
    ChickenFast4,
};

inline constexpr std::array<const char*, 6> ChickenMedium = {
    ChickenMedium1,
    ChickenMedium2,
    ChickenMedium3,
    ChickenMedium4,
    ChickenMedium5,
    ChickenMedium6,
};

inline constexpr std::array<const char*, 8> ChickenSlow = {
    ChickenSlow1,
    ChickenSlow2,
    ChickenSlow3,
    ChickenSlow4,
    ChickenSlow5,
    ChickenSlow6,
    ChickenSlow7,
    ChickenSlow8,
};

inline constexpr std::array<const char*, 5> Explosion = {
    Explosion1,
    Explosion2,
    Explosion3,
    Explosion4,
    Explosion5,
};
}
}

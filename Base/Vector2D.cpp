#include "Vector2D.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace
{
template <typename ExceptionType>
[[noreturn]] void LogAndThrow(const char* message)
{
    std::cerr << "[Vector2D] " << message << '\n';
    throw ExceptionType(message);
}
} // namespace

Vector2D::Vector2D()
    : x(0.0f)
    , y(0.0f)
{
}

Vector2D::Vector2D(float x, float y)
    : x(x)
    , y(y)
{
}


Vector2D Vector2D::operator+(const Vector2D& v) const
{
    return Vector2D(x + v.x, y + v.y);
}

Vector2D Vector2D::operator-(const Vector2D& v) const
{
    return Vector2D(x - v.x, y - v.y);
}

Vector2D Vector2D::operator*(float scalar) const
{
    return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator/(float scalar) const
{
    if (scalar == 0.0f)
    {
        LogAndThrow<std::domain_error>("Division by zero");
    }
    return Vector2D(x / scalar, y / scalar);
}

Vector2D& Vector2D::operator+=(const Vector2D& v)
{
    x += v.x;
    y += v.y;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector2D& Vector2D::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2D& Vector2D::operator/=(float scalar)
{
    if (scalar == 0.0f)
    {
        LogAndThrow<std::domain_error>("Division by zero");
    }
    x /= scalar;
    y /= scalar;
    return *this;
}

bool Vector2D::operator==(const Vector2D& v) const
{
    constexpr float Tolerance = 0.0001f;
    return (std::abs(x - v.x) < Tolerance) && (std::abs(y - v.y) < Tolerance);
}

bool Vector2D::operator!=(const Vector2D& v) const
{
    return !(*this == v);
}

const float& Vector2D::operator[](int i) const
{
    if (i == 0)
    {
        return x;
    }
    else if (i == 1)
    {
        return y;
    }

    LogAndThrow<std::out_of_range>("Index out of range");
}

float Vector2D::Length() const
{
    return std::sqrt(x * x + y * y);
}

void Vector2D::Normalize()
{
    if (IsZero())
    {
        LogAndThrow<std::domain_error>("Cannot normalize a zero vector");
    }

    const float length = Length();
    const float inverseLength = 1.0f / length;
    x *= inverseLength;
    y *= inverseLength;
}

Vector2D Vector2D::GetNormalized() const
{
    if (IsZero())
    {
        LogAndThrow<std::domain_error>("Cannot normalize a zero vector");
    }

    const float length = Length();
    const float inverseLength = 1.0f / length;
    return Vector2D(x * inverseLength, y * inverseLength);
}

float Vector2D::Distance(const Vector2D& v) const
{
    const float dx = x - v.x;
    const float dy = y - v.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool Vector2D::IsZero() const
{
    constexpr float Tolerance = 0.0001f;
    return (std::abs(x) < Tolerance) && (std::abs(y) < Tolerance);
}

float Vector2D::Dot(const Vector2D& v) const
{
    return x * v.x + y * v.y;
}

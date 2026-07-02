#pragma once

class Vector2D
{
public:
    float x;
    float y;

    Vector2D();
    Vector2D(float x, float y);

    //Base Operator
    Vector2D operator+(const Vector2D& v) const;
    Vector2D operator-(const Vector2D& v) const;
    Vector2D operator*(float scalar) const;
    Vector2D operator/(float scalar) const;
    
    Vector2D& operator+=(const Vector2D& v);
    Vector2D& operator-=(const Vector2D& v);
    Vector2D& operator*=(float scalar);
    Vector2D& operator/=(float scalar);

    bool operator==(const Vector2D& v) const; 
    bool operator!=(const Vector2D& v) const;

    const float& operator[](int i) const;
    
    float Length() const;
    
    void Normalize();
    Vector2D GetNormalized() const;
    float Distance(const Vector2D& v) const;
    bool IsZero() const;
    float Dot(const Vector2D& v) const;
    
};


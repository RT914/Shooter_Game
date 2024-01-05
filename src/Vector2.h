#ifndef _VECTOR2_H_
#define _VECTOR2_H_

// 2�����x�N�g���N���X
class Vector2
{
public:
    float x;
    float y;

    Vector2();
    explicit Vector2(float inX, float inY);

    // ���Z�q�̃I�[�o�[���[�h
    friend Vector2 operator+(const Vector2& a, const Vector2& b);
    friend Vector2 operator-(const Vector2& a, const Vector2& b);
    friend Vector2 operator*(const Vector2& a, const Vector2& b);
    friend Vector2 operator*(const Vector2& vec, float scalar);
    Vector2& operator+=(const Vector2& right);
    Vector2& operator-=(const Vector2& right);
    Vector2& operator*=(float scalar);

    float Length() const;
    float LengthSq() const;

    static const Vector2 Zero;
};

#endif
#ifndef VECTOR_H
#define VECTOR_H

template <typename T>
struct Vector
{

    Vector(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z)
    {
    }
    T x;
    T y;
    T z;
    Vector crossProduct(Vector<float> b)
    {
        Vector res(
            (y * b.z) - (z * b.y),
            (z * b.x) - (x * b.z),
            (x * b.y) - (y * b.x));
        return res;
    }
    float dotProduct(Vector<float> b)
    {
        return x * b.x + y * b.y + z * b.z;
    }
    float length()
    {
        return sqrt(x * x + y * y + z * z);
    }
    void set(Vector other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }
    Vector operator=(Vector other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }
    Vector operator+=(Vector other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    Vector operator+(Vector rhs)
    {
        Vector temp = *this;
        temp += rhs;
        return temp;
    }
    Vector operator-=(Vector other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    Vector operator-(Vector rhs)
    {
        Vector temp = *this;
        temp -= rhs;
        return temp;
    }
    Vector normalize()
    {
        float length = sqrt(x * x + y * y + z * z);
        if (length)
        {
            x /= length;
            y /= length;
            z /= length;
        }
        return *this;
    }
    Vector operator*(float rhs)
    {
        Vector temp = *this;
        temp.x *= rhs;
        temp.y *= rhs;
        temp.z *= rhs;
        return temp;
    }
    Vector operator*=(float rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
    }

    Vector operator/(float rhs)
    {
        Vector temp = *this;
        temp.x /= rhs;
        temp.y /= rhs;
        temp.z /= rhs;
        return temp;
    }
    Vector operator/=(float rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
    }
    void Print()
    {
        printf("V(%f,%f,%f)\n", x, y, z);
    }
};

#endif
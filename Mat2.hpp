//
//  Mat2.hpp
//  engine3d
//
//  Created by Brian Dolan on 4/24/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Mat2_hpp
#define Mat2_hpp

template<typename T>
class _Mat2
{
public:
    _Mat2() = default;
    _Mat2<T> operator+(const _Mat2<T>& rhs)
    {
        for (size_t i = 0; i < 2; i++)
        {
            for (size_t j = 0; j < 2; j++)
            {
                data[i][j] += rhs[i][j];
            }
        }
    }
    _Mat2<T>& operator+=(const _Vec2<T>& rhs)
    {
        data[0][0] += rhs.x;
        data[1][1] += rhs.y;
        return *this;
    }
    _Mat2<T> operator*(const _Mat2<T>& rhs)
    {
        _Mat2<T> ret;
        
        // loop through rows (r) and columns (c) for the *output* matrix
        for (size_t r = 0; r < 2; r++)
        {
            for (size_t c = 0; c < 2; c++)
            {
                ret.data[r][c] = z;
                for (size_t i = 0; i < 2; i++)
                    ret.data[r][c] += (data[i][c] * rhs.data[r][i]);
            }
        }
        
        return ret;
    }
    static _Mat2<T> Identity()
    {
        return
        {
            o, z,
            z, o
        };
    }
    static _Mat2<T> Rot(T angle)
    {
        T s = static_cast<T>(sin(angle));
        T c = static_cast<T>(cos(angle));
        return
        {
             c, s,
            -s, c
        };
    }
    
    ~_Mat2() = default;
    
private:
    static constexpr T z = static_cast<T>(0);
    static constexpr T o = static_cast<T>(1);
public:
    T data[2][2];
};

template<typename T>
_Vec2<T> operator*(const _Vec2<T>& lhs, const _Mat2<T>& rhs)
{
    _Vec2<T> res;
    res.x = lhs.x * rhs.data[0][0] + lhs.y * rhs.data[1][0];
    res.y = lhs.x * rhs.data[0][1] + lhs.y * rhs.data[1][1];
    return res;
}

template<typename T>
_Vec2<T>& operator*=(_Vec2<T>& lhs, const _Mat2<T>& rhs)
{
    lhs = lhs * rhs;
    return lhs;
}

typedef _Mat2<float> Mat2;

#endif /* Mat2_hpp */

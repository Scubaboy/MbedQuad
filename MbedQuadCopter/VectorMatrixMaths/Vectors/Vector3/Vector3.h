#ifndef Vector3_H
#define Vector3_H

#include "QuadTypes.h"
#//include "Matrix3.h"


template <typename T>
class Vector3
{
    public:
        T x, y, z;

        // trivial ctor
        Vector3<T>() 
        {
            x = y = z = 0;
        }

        // setting ctor
        Vector3<T>(const T x0, const T y0, const T z0) : x(x0), y(y0), z(z0) 
        {
        }

        // function call operator
        void operator ()(const T x0, const T y0, const T z0)
        {
            x= x0; y= y0; z= z0;
        }

        // test for equality
        bool operator ==(const Vector3<T> &v) const;

        // test for inequality
        bool operator !=(const Vector3<T> &v) const;

        // negation
        Vector3<T> operator -(void) const;

        // addition
        Vector3<T> operator +(const Vector3<T> &v) const;

        // subtraction
        Vector3<T> operator -(const Vector3<T> &v) const;

        // uniform scaling
        Vector3<T> operator *(const T num) const;

        // uniform scaling
        Vector3<T> operator  /(const T num) const;

        // addition
        Vector3<T> &operator +=(const Vector3<T> &v);

        // subtraction
        Vector3<T> &operator -=(const Vector3<T> &v);

        // uniform scaling
        Vector3<T> &operator *=(const T num);

        // uniform scaling
        Vector3<T> &operator /=(const T num);

        // allow a vector3 to be used as an array, 0 indexed
        T & operator[](int i) 
        {
            T *_v = &x;

            return _v[i];
        }

        const T & operator[](int i) const 
        {
            const T *_v = &x;

            return _v[i];
        }

        // dot product
        T operator *(const Vector3<T> &v) const;

        

        // cross product
        Vector3<T> operator %(const Vector3<T> &v) const;

        // computes the angle between this vector and another vector
        float angle(const Vector3<T> &v2) const;

        // check if any elements are NAN
        bool is_nan(void) const;

        // check if any elements are infinity
        bool is_inf(void) const;

        // check if all elements are zero
        bool is_zero(void) const { return x==0 && y == 0 && z == 0; }

        // rotate by a standard rotation
        void rotate(QuadTypes::Rotation rotation);

        // gets the length of this vector squared
        T  length_squared() const
        {
            return (T)(*this * *this);
        }

        // gets the length of this vector
        float length(void) const;

        // normalizes this vector
        void normalize()
        {
            *this /= length();
        }

        // zero the vector
        void zero()
        {
            x = y = z = 0;
        }

        // returns the normalized version of this vector
        Vector3<T> normalized() const
        {
            return *this/length();
        }

        // reflects this vector about n
        void  reflect(const Vector3<T> &n)
        {
            Vector3<T>        orig(*this);
            project(n);
            *this = *this*2 - orig;
        }

        // projects this vector onto v
        void project(const Vector3<T> &v)
        {
            *this= v * (*this * v)/(v*v);
        }

        // returns this vector projected onto v
        Vector3<T> projected(const Vector3<T> &v) const
        {
            return v * (*this * v)/(v*v);
        }
};

typedef Vector3<short>                Vector3i;
typedef Vector3<unsigned short>       Vector3ui;
typedef Vector3<int>                  Vector3l;
typedef Vector3<unsigned int>         Vector3ul;
typedef Vector3<float>                Vector3f;
#endif
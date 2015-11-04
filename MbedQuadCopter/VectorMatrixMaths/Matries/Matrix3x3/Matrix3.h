#ifndef Matrix3_H
#define Matrix3_H
#include "Vector2.h"
#include "Vector3.h"

// 3x3 matrix with elements of type T
template <typename T>
class Matrix3 {
public:

    // Vectors comprising the rows of the matrix
    Vector3<T>        a, b, c;

    // trivial ctor
    // note that the Vector3 ctor will zero the vector elements
    Matrix3<T>() {
    }

    // setting ctor
    Matrix3<T>(const Vector3<T> a0, const Vector3<T> b0, const Vector3<T> c0) : a(a0), b(b0), c(c0) {
    }

    // setting ctor
    Matrix3<T>(const T ax, const T ay, const T az, const T bx, const T by, const T bz, const T cx, const T cy, const T cz) : a(ax,ay,az), b(bx,by,bz), c(cx,cy,cz) {
    }

    // function call operator
    void operator        () (const Vector3<T> a0, const Vector3<T> b0, const Vector3<T> c0)
    {
        a = a0; b = b0; c = c0;
    }

    // multiply a row vector by a matrix, to give a row vector
        Vector3<T> operator *(const Matrix3<T> &m) const;

        // multiply a column vector by a row vector, returning a 3x3 matrix
        Matrix3<T> mul_rowcol(const Vector3<T> &v) const;
    
    // test for equality
    bool operator        == (const Matrix3<T> &m)
    {
        return (a==m.a && b==m.b && c==m.c);
    }

    // test for inequality
    bool operator        != (const Matrix3<T> &m)
    {
        return (a!=m.a || b!=m.b || c!=m.c);
    }

    // negation
    Matrix3<T> operator        - (void) const
    {
        return Matrix3<T>(-a,-b,-c);
    }

    // addition
    Matrix3<T> operator        + (const Matrix3<T> &m) const
    {
        return Matrix3<T>(a+m.a, b+m.b, c+m.c);
    }
    Matrix3<T> &operator        += (const Matrix3<T> &m)
    {
        return *this = *this + m;
    }

    // subtraction
    Matrix3<T> operator        - (const Matrix3<T> &m) const
    {
        return Matrix3<T>(a-m.a, b-m.b, c-m.c);
    }
    Matrix3<T> &operator        -= (const Matrix3<T> &m)
    {
        return *this = *this - m;
    }

    // uniform scaling
    Matrix3<T> operator        * (const T num) const
    {
        return Matrix3<T>(a*num, b*num, c*num);
    }
    Matrix3<T> &operator        *= (const T num)
    {
        return *this = *this * num;
    }
    Matrix3<T> operator        / (const T num) const
    {
        return Matrix3<T>(a/num, b/num, c/num);
    }
    Matrix3<T> &operator        /= (const T num)
    {
        return *this = *this / num;
    }

    // allow a Matrix3 to be used as an array of vectors, 0 indexed
    Vector3<T> & operator[](int i) {
        Vector3<T> *_v = &a;

        return _v[i];
    }

    const Vector3<T> & operator[](int i) const {
        const Vector3<T> *_v = &a;

        return _v[i];
    }

    // multiplication by a vector
    Vector3<T> operator         *(const Vector3<T> &v) const;

    // multiplication of transpose by a vector
    Vector3<T>                  mul_transpose(const Vector3<T> &v) const;

    // multiplication by a vector giving a Vector2 result (XY components)
    Vector2<T> mulXY(const Vector3<T> &v) const;

    // extract x column
    Vector3<T>                  colx(void) const
    {
        return Vector3<T>(a.x, b.x, c.x);
    }

    // extract y column
    Vector3<T>        coly(void) const
    {
        return Vector3<T>(a.y, b.y, c.y);
    }

    // extract z column
    Vector3<T>        colz(void) const
    {
        return Vector3<T>(a.z, b.z, c.z);
    }

    // multiplication by another Matrix3<T>
    Matrix3<T> operator *(const Matrix3<T> &m);

    Matrix3<T> &operator *=(const Matrix3<T> &m)
    {
        return *this = *this * m;
    }

    // transpose the matrix
    Matrix3<T>          transposed(void) const;

    void transpose(void)
    {
        *this = transposed();
    }

    // zero the matrix
    void        zero(void);

    // setup the identity matrix
    void        identity(void) {
        a.x = b.y = c.z = 1;
        a.y = a.z = 0;
        b.x = b.z = 0;
        c.x = c.y = 0;
    }

    // check if any elements are NAN
    bool        is_nan(void)
    {
        return a.is_nan() || b.is_nan() || c.is_nan();
    }

    // create a rotation matrix from Euler angles
    void        from_euler(float roll, float pitch, float yaw);

    // create eulers from a rotation matrix
    void        to_euler(float *roll, float *pitch, float *yaw) const;

    // apply an additional rotation from a body frame gyro vector
    // to a rotation matrix.
    void        rotate(const Vector3<T> &g);

    // apply an additional rotation from a body frame gyro vector
    // to a rotation matrix but only use X, Y elements from gyro vector
    void        rotateXY(const Vector3<T> &g);

    // apply an additional inverse rotation to a rotation matrix but 
    // only use X, Y elements from rotation vector
    void        rotateXYinv(const Vector3<T> &g);
};

typedef Matrix3<short>                Matrix3i;
typedef Matrix3<unsigned short>               Matrix3ui;
typedef Matrix3<int>                Matrix3l;
typedef Matrix3<unsigned int>               Matrix3ul;
typedef Matrix3<float>                  Matrix3f;

#endif
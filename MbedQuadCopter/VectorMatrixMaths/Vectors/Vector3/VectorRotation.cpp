#include "Vector3.h"

const float HALF_SQRT_2 = 0.70710678118654757f;

// rotate a vector by a standard rotation, attempting
// to use the minimum number of floating point operations
template <typename T>
void Vector3<T>::rotate(QuadTypes::Rotation rotation)
{
    T tempT;
    
    switch (rotation) 
    {
        case QuadTypes::ROTATION_NONE:
        case QuadTypes::ROTATION_MAX:
        {
            break;
        }
        case QuadTypes::ROTATION_YAW_45: 
        {
            tempT = HALF_SQRT_2*(x - y);
            y   = HALF_SQRT_2*(x + y);
            x = tempT;
            break;
        }
        case QuadTypes::ROTATION_YAW_90: 
        {
            tempT = x; 
            x = -y; 
            y = tempT;
            break;
        }
        case QuadTypes::ROTATION_YAW_135: 
        {
            tempT = -HALF_SQRT_2*(x + y);
            y   =  HALF_SQRT_2*(x - y);
            x = tempT;
            break;
        }
        case QuadTypes::ROTATION_YAW_180:
        {
            x = -x; 
            y = -y;
            break;
        }
        case QuadTypes::ROTATION_YAW_225: 
        {
            tempT = HALF_SQRT_2*(y - x);
            y   = -HALF_SQRT_2*(x + y);
            x = tempT;
            break;
        }
        case QuadTypes::ROTATION_YAW_270: 
        {
            tempT = x; 
            x = y; 
            y = -tempT;
            break;
        }
        case QuadTypes::ROTATION_YAW_315: 
        {
            tempT = HALF_SQRT_2*(x + y);
            y   = HALF_SQRT_2*(y - x);
            x = tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_180: 
        {
            y = -y; 
            z = -z;
            break;
        }
        case QuadTypes::ROTATION_ROLL_180_YAW_45: 
        {
            tempT = HALF_SQRT_2*(x + y);
            y   = HALF_SQRT_2*(x - y);
            x = tempT; z = -z;
            break;
        }
        case QuadTypes::ROTATION_ROLL_180_YAW_90: 
        {
            tempT = x; 
            x = y; 
            y = tempT; z = -z;
            break;
        }
        case QuadTypes::ROTATION_ROLL_180_YAW_135: 
        {
            tempT = HALF_SQRT_2*(y - x);
            y   = HALF_SQRT_2*(y + x);
            x = tempT; z = -z;
            break;
        }
        case QuadTypes::ROTATION_PITCH_180: 
        {
            x = -x; 
            z = -z;
            break;
        }
        case QuadTypes::ROTATION_ROLL_180_YAW_225: 
        {
            tempT = -HALF_SQRT_2*(x + y);
            y   =  HALF_SQRT_2*(y - x);
            x = tempT; z = -z;
            break;
        }
        case QuadTypes::ROTATION_ROLL_180_YAW_270: 
        {
            tempT = x; 
            x = -y; 
            y = -tempT; 
            z = -z;
            break;
        }
        case QuadTypes::ROTATION_ROLL_180_YAW_315: 
        {
            tempT =  HALF_SQRT_2*(x - y);
            y   = -HALF_SQRT_2*(x + y);
            x = tempT; z = -z;
            break;
        }
        case QuadTypes::ROTATION_ROLL_90: 
        {
            tempT = z; 
            z = y; 
            y = -tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_90_YAW_45: 
        {
            tempT = z; 
            z = y; 
            y = -tempT;
            tempT = HALF_SQRT_2*(x - y);
            y   = HALF_SQRT_2*(x + y);
            x = tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_90_YAW_90: 
        {
            tempT = z; 
            z = y; 
            y = -tempT;
            tempT = x; 
            x = -y; 
            y = tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_90_YAW_135: 
        {
            tempT = z; 
            z = y; 
            y = -tempT;
            tempT = -HALF_SQRT_2*(x + y);
            y   =  HALF_SQRT_2*(x - y);
            x = tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_270: 
        {
            tempT = z; 
            z = -y; 
            y = tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_270_YAW_45: 
        {
            tempT = z; 
            z = -y; 
            y = tempT;
            tempT = HALF_SQRT_2*(x - y);
            y   = HALF_SQRT_2*(x + y);
            x = tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_270_YAW_90: 
        {
            tempT = z; 
            z = -y; 
            y = tempT;
            tempT = x; 
            x = -y; 
            y = tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_270_YAW_135: 
        {
            tempT = z; 
            z = -y; 
            y = tempT;
            tempT = -HALF_SQRT_2*(x + y);
            y   =  HALF_SQRT_2*(x - y);
            x = tempT;
            break;
        }
        case QuadTypes::ROTATION_PITCH_90: 
        {
            tempT = z; 
            z = -x; 
            x = tempT;
            break;
        }
        case QuadTypes::ROTATION_PITCH_270: 
        {
            tempT = z; 
            z = x; 
            x = -tempT;
            break;
        }
        case QuadTypes::ROTATION_PITCH_180_YAW_90: 
        {
            z = -z;
            tempT = -x; 
            x = -y; 
            y = tempT;
            break;
        }
        case QuadTypes::ROTATION_PITCH_180_YAW_270: 
        {
            x = -x; 
            z = -z;
            tempT = x; 
            x = y; 
            y = -tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_90_PITCH_90: 
        {
            tempT = z; 
            z = y; 
            y = -tempT;
            tempT = z; 
            z = -x; 
            x = tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_180_PITCH_90: 
        {
            y = -y; 
            z = -z;
            tempT = z; 
            z = -x; 
            x = tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_270_PITCH_90: 
        {
            tempT = z; 
            z = -y; 
            y = tempT;
            tempT = z; 
            z = -x; 
            x = tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_90_PITCH_180: 
        {
            tempT = z; 
            z = y; 
            y = -tempT;
            x = -x; 
            z = -z;
            break;
        }
        case QuadTypes::ROTATION_ROLL_270_PITCH_180: 
        {
            tempT = z; 
            z = -y; 
            y = tempT;
            x = -x; 
            z = -z;
            break;
        }
        case QuadTypes::ROTATION_ROLL_90_PITCH_270: 
        {
            tempT = z; 
            z = y; 
            y = -tempT;
            tempT = z; 
            z = x; 
            x = -tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_180_PITCH_270: 
        {
            y = -y; 
            z = -z;
            tempT = z; 
            z = x; 
            x = -tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_270_PITCH_270: 
        {
            tempT = z; 
            z = -y; 
            y = tempT;
            tempT = z; 
            z = x; 
            x = -tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_90_PITCH_180_YAW_90: 
        {
            tempT = z; 
            z = y; 
            y = -tempT;
            x = -x; z = -z;
            tempT = x; x = -y; y = tempT;
            break;
        }
        case QuadTypes::ROTATION_ROLL_90_YAW_270: 
        {
            tempT = z; 
            z = y; 
            y = -tempT;
            tempT = x; 
            x = y; 
            y = -tempT;
            break;
        }
    }
}

template void Vector3<float>::rotate(QuadTypes::Rotation);
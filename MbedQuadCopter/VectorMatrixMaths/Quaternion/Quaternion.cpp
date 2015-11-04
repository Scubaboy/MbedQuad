#include "Quaternion.h"
#include "mbed.h"

const float EPSILON = 0.0001f;
const float PI_2 = 1.57079632679489661923f;

    
   /* void Quaternion::encode(char *buffer){
        int value = (w * (1 << 30));
        char* bytes = (char*)&value;
        for(int i = 0; i < 4; i ++){
            buffer[i] = bytes[3-i];
        }
        
        value = v.x * (1 << 30);
        for(int i = 0; i < 4; i ++){
            buffer[i+4] = bytes[3-i];
        }
        
        value = v.y * (1 << 30);
        for(int i = 0; i < 4; i ++){
            buffer[i+8] = bytes[3-i];
        }
        
        value = v.z  * (1 << 30);
        for(int i = 0; i < 4; i ++){
            buffer[i+12] = bytes[3-i];
        }            
    }
    
    void Quaternion::decode(const char *buffer){
        set((float)((((int32_t)buffer[0] << 24) + ((int32_t)buffer[1] << 16) + ((int32_t)buffer[2] << 8) + buffer[3]))* (1.0 / (1<<30)),
                (float)((((int32_t)buffer[4] << 24) + ((int32_t)buffer[5] << 16) + ((int32_t)buffer[6] << 8) + buffer[7]))* (1.0 / (1<<30)),
                (float)((((int32_t)buffer[8] << 24) + ((int32_t)buffer[9] << 16) + ((int32_t)buffer[10] << 8) + buffer[11]))* (1.0 / (1<<30)),
                (float)((((int32_t)buffer[12] << 24) + ((int32_t)buffer[13] << 16) + ((int32_t)buffer[14] << 8) + buffer[15]))* (1.0 / (1<<30)));    
    }
    
    void Quaternion::set( float _w, float _x, float _y, float _z) {
        w = _w;
        v(_x, _y, _z); 
    }

    */
    const Vector3f Quaternion::getEulerAngles(Quat quat)
    {
        double sqw = quat.w*quat.w;
        double sqx = quat.vect.x*quat.vect.x;
        double sqy = quat.vect.y*quat.vect.y;
        double sqz = quat.vect.z*quat.vect.z;
        double unit = sqx + sqy + sqz + sqw;
        double test = quat.vect.x*quat.vect.y + quat.vect.z*quat.w;
        Vector3f r;
        
       /* if (test > 0.499*unit) { // singularity at north pole
            r.z = 2 * atan2(v.x,w);
            r.x = PI_2;
            r.y = 0;
            return r;
        }
        if (test < -0.499*unit) { // singularity at south pole
            r.z = -2 * atan2(v.x,w);
            r.x = -PI_2;
            r.y = 0;
            return r;
        }*/
       // r.z = atan2((double)(2*v.y*w-2*v.x*v.z ),1 - 2*sqy - 2*sqz);// (double)(sqx - sqy - sqz + sqw));
       // r.x = asin(2*test);///unit);
        //r.y = atan2((double)(2*v.x*w-2*v.y*v.z) ,1 - 2*sqx - 2*sqz);//(double)( -sqx + sqy - sqz + sqw));
        
        r.z = atan2(2*quat.vect.x*quat.vect.y - 2*quat.w*quat.vect.z, 2*quat.w*quat.w + 2*quat.vect.x*quat.vect.x - 1);   // psi
        r.x = -asin(2*quat.vect.x*quat.vect.z + 2*quat.w*quat.vect.y);                              // theta
        r.y = atan2(2*quat.vect.y*quat.vect.z - 2*quat.w*quat.vect.x, 2*quat.w*quat.w + 2*quat.vect.z*quat.vect.z - 1);   // phi
    
        return r;
    }
    


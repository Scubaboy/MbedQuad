#ifndef CoordinateGeometry_H
#define CoordinateGeometry_H

namespace CoordinateGeometry
{
    int wrap_360_cd(int error);
    int wrap_180_cd(int error);
    float wrap_360_cd_float(float angle);
    float wrap_180_cd_float(float angle);     
};
#endif
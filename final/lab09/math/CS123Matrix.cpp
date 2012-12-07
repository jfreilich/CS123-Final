/*!
   @file   CS123Matrix.cpp
   @author Travis Fischer (fisch0920@gmail.com)
   @date   Fall 2008
   
   @brief
      Provides basic functionality for a templated, arbitrarily-sized matrix.
      You will need to fill this file in for the Camtrans assignment.

**/

#include "CS123Algebra.h"
#include <iostream>

//@name Routines which generate specific-purpose transformation matrices
//@{---------------------------------------------------------------------
// @returns the scale matrix described by the vector
Matrix4x4 getScaleMat(const Vector4 &v) {
    Matrix4x4 m= Matrix4x4(v.x,0,0,0,0,v.y,0,0,0,0,v.z,0,0,0,0,1);
    // @TODO: (CAMTRANS) Fill this in...
    return m;

}

// @returns the translation matrix described by the vector
Matrix4x4 getTransMat(const Vector4 &v) {
    Matrix4x4 m= Matrix4x4(1,0,0,v.x,0,1,0,v.y,0,0,1,v.z,0,0,0,1);
    // @TODO: (CAMTRANS) Fill this in...
    return m;

}

// @returns the rotation matrix about the x axis by the specified angle
Matrix4x4 getRotXMat (const REAL radians) {
    REAL c = cos(radians);
    REAL s = sin(radians);
    Matrix4x4 m= Matrix4x4(1,0,0,0,0,c,-s,0,0,s,c,0,0,0,0,1);
    // @TODO: (CAMTRANS) Fill this in...
    return m;

}

// @returns the rotation matrix about the y axis by the specified angle
Matrix4x4 getRotYMat (const REAL radians) {
    REAL c = cos(radians);
    REAL s = sin(radians);
    Matrix4x4 m= Matrix4x4(c,0,s,0,0,1,0,0,-s,0,c,0,0,0,0,1);
    // @TODO: (CAMTRANS) Fill this in...
    return m;

}

// @returns the rotation matrix about the z axis by the specified angle
Matrix4x4 getRotZMat (const REAL radians) {
    REAL c = cos(radians);
    REAL s = sin(radians);
    Matrix4x4 m= Matrix4x4(c,-s,0,0,s,c,0,0,0,0,1,0,0,0,0,1);
    // @TODO: (CAMTRANS) Fill this in...
    return m;

}

// @returns the rotation matrix around the vector and point by the specified angle
Matrix4x4 getRotMat  (const Vector4 &p, const Vector4 &v, const REAL a) {
    REAL theta = atan2(v.z,v.x);
    REAL psi=-atan2(v.y,pow(v.x*v.x+v.z*v.z,0.5));
    Matrix4x4 m1=getRotYMat(theta);
    Matrix4x4 m1i=getRotYMat(-theta);
    Matrix4x4 m2=getRotZMat(psi);
    Matrix4x4 m2i=getRotZMat(-psi);
    Matrix4x4 m3=getRotXMat(a);
    Matrix4x4 m0=getTransMat(-p);
    Matrix4x4 m0i=getTransMat(p);
    Matrix4x4 m=m0i*m1i*m2i*m3*m2*m1*m0;
    // @TODO: (CAMTRANS) Fill this in...
    return m;

}


// @returns the inverse scale matrix described by the vector
Matrix4x4 getInvScaleMat(const Vector4 &v) {
    const Vector4 &v2=Vector4(1.0/v.x,1.0/v.y,1.0/v.z,1);
    return getScaleMat(v2);
    // @TODO: (CAMTRANS) Fill this in...

}

// @returns the inverse translation matrix described by the vector
Matrix4x4 getInvTransMat(const Vector4 &v) {
    const Vector4 &v2=Vector4(-v.x,-v.y,-v.z,1);
    return getTransMat(v2);
    // @TODO: (CAMTRANS) Fill this in...


}

// @returns the inverse rotation matrix about the x axis by the specified angle
Matrix4x4 getInvRotXMat (const REAL radians) {
    return getRotXMat(-radians);
    // @TODO: (CAMTRANS) Fill this in...
    return Matrix4x4::identity();

}

// @returns the inverse rotation matrix about the y axis by the specified angle
Matrix4x4 getInvRotYMat (const REAL radians) {
    return getRotYMat(-radians);
    // @TODO: (CAMTRANS) Fill this in...
    return Matrix4x4::identity();

}

// @returns the inverse rotation matrix about the z axis by the specified angle
Matrix4x4 getInvRotZMat (const REAL radians) {
    return getRotZMat(-radians);
    // @TODO: (CAMTRANS) Fill this in...
    return Matrix4x4::identity();

}

// @returns the inverse rotation matrix around the vector and point by the specified angle
Matrix4x4 getInvRotMat  (const Vector4 &p, const Vector4 &v, const REAL a) {
   // const Vector4 &v2=Vector4(-v.x,-v.y,-v.z,1);
    return getRotMat(p,v,-a);
    // @TODO: (CAMTRANS) Fill this in...


}




//@}---------------------------------------------------------------------


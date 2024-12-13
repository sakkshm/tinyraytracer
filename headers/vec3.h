#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

class vec3{
    public: 
        //3 values
        double r[3];

    vec3() : r{0,0,0} {};
    vec3(double x, double y, double z) : r{x,y,z} {};

    
    double x(){ return r[0]; }
    double y(){ return r[1]; }
    double z(){ return r[2]; }

    vec3 operator -() const { return vec3(-r[0], -r[1], -r[2]); }
    double operator[](int i) const { return r[i]; }
    double& operator[](int i) { return r[i]; }

    vec3& operator +=(const vec3& v) {
        r[0] += v.r[0];
        r[1] += v.r[1];
        r[2] += v.r[2];
        return *this;
    }

    vec3& operator *=(double t) {
        r[0] *= t;
        r[1] *= t;
        r[2] *= t;
        return *this;
    }

    vec3& operator /=(double t) {
        return *this *= 1/t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
    }
};

// point3 is an alias for vec3, but useful for geometric clarity 
using point3 = vec3;

inline std::ostream& operator << (std::ostream& out, const vec3& v){
    return out << v.r[0] << ' ' << v.r[1] << ' ' << v.r[2];
}

inline vec3 operator +(const vec3& u, const vec3& v) {
    return vec3(u.r[0] + v.r[0], u.r[1] + v.r[1], u.r[2] + v.r[2]);
}

inline vec3 operator -(const vec3& u, const vec3& v) {
    return vec3(u.r[0] - v.r[0], u.r[1] - v.r[1], u.r[2] - v.r[2]);
}

inline vec3 operator *(const vec3& u, const vec3& v) {
    return vec3(u.r[0] * v.r[0], u.r[1] * v.r[1], u.r[2] * v.r[2]);
}

inline vec3 operator *(double t, const vec3& v) {
    return vec3(t*v.r[0], t*v.r[1], t*v.r[2]);
}

inline vec3 operator *(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator /(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.r[0] * v.r[0]
         + u.r[1] * v.r[1]
         + u.r[2] * v.r[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.r[1] * v.r[2] - u.r[2] * v.r[1],
                u.r[2] * v.r[0] - u.r[0] * v.r[2],
                u.r[0] * v.r[1] - u.r[1] * v.r[0]);
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

#endif
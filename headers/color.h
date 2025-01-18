#ifndef COLOR_H
#define COLOR_H


#include "vec3.h"
#include <iostream>
#include "interval.h"

using color = vec3;

double clamp(double x)  {
    double min = 0.000;
    double max = 0.999;

    if(x < min) return min;
    if(x > max) return max;

    return x;
}

void write_color(std::ostream& out, const color& pixel_color){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255]
    int rbyte = int(256 * clamp(r));
    int gbyte = int(256 * clamp(g));
    int bbyte = int(256 * clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
    
}

#endif
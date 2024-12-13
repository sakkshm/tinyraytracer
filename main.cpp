#include <iostream>
#include "./headers/vec3.h"
#include "./headers/color.h"

int main(){

    const int img_w = 256;
    const int img_h = 256;


    //PPM boilerplate: Define mode, imgW, imgH, color spectrum
    std::cout << "P3\n" << img_w << " " << img_h << "\n255\n";

    for(int i = 0; i < img_h; i++){
        std::clog << "\rScanline remaining: " << (img_h - i) << ' ' << std::flush;

        for(int j = 0; j < img_w; j++){

            //r,g,b range from 0.0 - 1.0 
            double r = double(i) / (img_w - 1);
            double g = double(j) / (img_h - 1);
            double b = 0.0;

            color pixel_color = color(r, g, b);
            write_color(std::cout, pixel_color);
        }

        std::clog << "\nDone.                 \n";
        std::cout << '\n';
    }
}
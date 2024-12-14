#include <iostream>

#include "./headers/vec3.h"
#include "./headers/color.h"
#include "./headers/ray.h"



bool hit_sphere(const point3& center, double radius, const ray& r){
    //a, b , c are coeff of ray-sphere intersection equation
    //r.origin() is camera center
    vec3 oc = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = -2 * dot(r.direction(), oc); 
    auto c = dot(oc, oc) - radius * radius; 

    auto disc = b*b - 4*a*c;

    return (disc >= 0);

}


color ray_color(const ray& r){
    
    if(hit_sphere(point3(0, 0, 1), 0.5, r)){
        return color(1,0,0);
    }

    //Blue white gradient background
    vec3 unit_dirn = unit_vector(r.direction());

    auto a = 0.5*(unit_dirn.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0, 0, 1.0);

}

int main(){

    //Aspect ration is 16:9 -> width/height
    const double aspect_ratio = 16.0/9.0;
    int img_w = 400;

    //Img height must be atleast 1
    int img_h = int(img_w / aspect_ratio);
    img_h = (img_h < 1) ? 1 : img_h;

    //Camera
    double focal_l = 1.0;
    double viewport_h = 2.0;
    double viewport_w = viewport_h * (double(img_w)/img_h);
    auto camera_center = point3(0, 0, 0);

    //distance between camera center and viewport: focal length
    //y-axis go up, the x-axis to the right, and the negative z-axis pointing in the viewing direction. (This is commonly referred to as right-handed coordinates.) 

    // Vectors across the horizontal and down the vertical viewport edges.
    auto viewpost_u = vec3(viewport_w, 0, 0);
    auto viewpost_v = vec3(0, -viewport_h, 0);

    //horizontal and vertical delta vectors
    auto pixel_delta_u = viewpost_u / img_w;
    auto pixel_delta_v = viewpost_v / img_h;

    auto viewport_upper_left = camera_center - vec3(0,0,focal_l) - viewpost_u/2 - viewpost_v/2;

    auto pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);


    //PPM boilerplate: Define mode, imgW, imgH, color spectrum
    std::cout << "P3\n" << img_w << " " << img_h << "\n255\n";

    for(int i = 0; i < img_h; i++){
        std::clog << "\rScanline remaining: " << (img_h - i) << ' ' << std::flush;

        for(int j = 0; j < img_w; j++){

            auto pixel_center = pixel00_loc + (i * pixel_delta_v) + (j * pixel_delta_u);
            auto ray_direction = pixel_center - camera_center;

            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);

            write_color(std::cout, pixel_color);
        }

        std::clog << "\nDone.                 \n";
        std::cout << '\n';
    }
}
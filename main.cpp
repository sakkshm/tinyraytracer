#include <iostream>

#include "./headers/vec3.h"
#include "./headers/color.h"
#include "./headers/ray.h"
#include "./headers/hittable.h"
#include "./headers/hittable_list.h"
#include "./headers/sphere.h"
#include "./headers/rtweekend.h"
#include "./headers/camera.h"



double hit_sphere(const point3& center, double radius, const ray& r){
    //a, b , c are coeff of ray-sphere intersection equation
    //r.origin() is camera center
    vec3 oc = center - r.origin();
     auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = h*h - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (h - std::sqrt(discriminant)) / a;
    }

}




int main(){

    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 400;

    cam.render(world);
}
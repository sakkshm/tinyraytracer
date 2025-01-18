#include "headers/color.h"
#include "headers/vec3.h"
#include "headers/ray.h"
#include "headers/rt_weekend.h"
#include "headers/hittable.h"
#include "headers/hittable_list.h"
#include "headers/sphere.h"
#include "headers/interval.h"
#include "headers/camera.h"

#include <iostream>




int main(){

    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 400;
    cam.samples_per_pixel = 100;

    cam.render(world);

    
    return 0;
}
all: rayt

rayt: main.cpp ./headers/vec3.h ./headers/color.h ./headers/ray.h ./headers/hittable.h ./headers/hittable_list.h ./headers/rtweekend.h ./headers/sphere.h ./headers/interval.h
	g++ main.cpp ./headers/vec3.h ./headers/color.h ./headers/ray.h ./headers/hittable.h ./headers/hittable_list.h ./headers/rtweekend.h ./headers/sphere.h ./headers/interval.h -o ./build/main
	./build/main > output/out.ppm
	xdg-open ./output/out.ppm
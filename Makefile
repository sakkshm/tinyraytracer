all: rayt

rayt: main.cpp ./headers/vec3.h ./headers/color.h ./headers/ray.h ./headers/hittable.h ./headers/hittable_list.h ./headers/rtweekend.h ./headers/sphere.h ./headers/interval.h ./headers/camera.h
	ccache g++ main.cpp ./headers/*.h -o ./build/main
	./build/main > output/out.ppm
	xdg-open ./output/out.ppm
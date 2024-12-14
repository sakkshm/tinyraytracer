all: rayt

rayt: main.cpp ./headers/vec3.h ./headers/color.h ./headers/ray.h
	g++ main.cpp ./headers/vec3.h ./headers/color.h ./headers/ray.h -o ./build/main
	./build/main > output/out.ppm
	xdg-open ./output/out.ppm
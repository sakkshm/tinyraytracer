all: main.cpp ./headers/*
	g++ main.cpp ./headers/* -o build/main
	./build/main > ./out/img.ppm
	xdg-open ./out/img.ppm
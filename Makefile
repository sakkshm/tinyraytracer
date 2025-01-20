all: main.cpp 
	g++ main.cpp -o build/main
	./build/main > ./out/img.ppm
	xdg-open ./out/img.ppm
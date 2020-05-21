
all: tracer

ppm: ppm_demo.cpp
	g++ ppm_demo.cpp -o ppm

test_image: ppm
	- ./ppm > image.ppm
	- eog image.ppm

tracer: tracer.cpp geom.cpp
	g++ tracer.cpp geom.cpp -o tracer

tracer_test: tracer
	- ./tracer > image.ppm
	- eog image.ppm
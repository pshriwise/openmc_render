
all: tracer

ppm: ppm_demo.cpp
	g++ ppm_demo.cpp -o ppm

test_image: ppm
	- ./ppm > image.ppm
	- eog image.ppm

tracer: *.cpp *.h
	g++ -O2 tracer.cpp geom.cpp -o tracer

tracer_dbg: *.cpp *.h
	g++ -g tracer.cpp geom.cpp -o tracer

tracer_omp: *.cpp *.h
	g++ -O2 -fopenmp tracer.cpp geom.cpp -pthread -o tracer

test: tracer
	- ./tracer
	- eog image.ppm

clean:
	- rm -rf ppm tracer
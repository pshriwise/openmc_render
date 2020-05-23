
TRACER_SOURCES=tracer.cpp geom.cpp progress_bar.cpp scene.cpp

all: tracer

ppm: ppm_demo.cpp
	g++ ppm_demo.cpp -o ppm

test_image: ppm
	- ./ppm > image.ppm
	- eog image.ppm

tracer: *.cpp *.h
	g++ -O2 $(TRACER_SOURCES) -o tracer

tracer_dbg: *.cpp *.h
	g++ -g $(TRACER_SOURCES) -o tracer

tracer_omp: *.cpp *.h
	g++ -O2 -fopenmp $(TRACER_SOURCES) -pthread -o tracer

test: tracer
	- ./tracer
	- eog image.ppm

clean:
	- rm -rf ppm tracer
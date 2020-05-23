
TRACER_SOURCES=tracer.cpp geom.cpp progress_bar.cpp scene.cpp

MC_TRACER_SOURCES=openmc_tracer.cpp geom.cpp progress_bar.cpp scene.cpp

OPENMC_DIR=/home/shriwise/opt/openmc

all: tracer

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

openmc_tracer:
	- g++ -O2 -I$(OPENMC_DIR)/include $(MC_TRACER_SOURCES) -pthread -L$(OPENMC_DIR)/lib -lopenmc -o openmc_tracer

clean:
	- rm -rf ppm tracer
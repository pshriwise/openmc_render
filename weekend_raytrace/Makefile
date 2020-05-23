
TRACER_SOURCES=tracer.cpp geom.cpp progress_bar.cpp scene.cpp

MC_TRACER_SOURCES=openmc_tracer.cpp geom.cpp progress_bar.cpp scene.cpp openmc_scenes.cpp

OPENMC_DIR=/home/shriwise/opt/openmc

HDF5_DIR=/usr/lib/x86_64-linux-gnu/hdf5/serial

all: tracer

tracer: *.cpp *.h
	g++ -O2 $(TRACER_SOURCES) -o tracer

tracer_dbg: *.cpp *.h
	g++ -g $(TRACER_SOURCES) -o tracer

tracer_omp: *.cpp *.h
	g++ -O2 -fopenmp $(TRACER_SOURCES) -pthread -o tracer

test: tracer
	- ./tracer
	- eog image.ppm

openmc_tracer: $(MC_TRACER_SOURCES)
	- g++ --std=c++11 -I$(OPENMC_DIR)/include -I$(HDF5_DIR)/include $(MC_TRACER_SOURCES) -L$(OPENMC_DIR)/lib -lopenmc -o openmc_tracer

openmc_tracer_omp: $(MC_TRACER_SOURCES)
	- g++ --std=c++14 -O2 -fopenmp -I$(OPENMC_DIR)/include -I$(HDF5_DIR)/include $(MC_TRACER_SOURCES) -pthread -L$(OPENMC_DIR)/lib -lopenmc -o openmc_tracer

clean:
	- rm -rf openmc_tracer tracer
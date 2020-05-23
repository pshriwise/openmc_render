
COMMON_SOURCES=geom.cpp progress_bar.cpp trace.cpp color.cpp scene.cpp

TRACER_SOURCES=tracer.cpp

MC_TRACER_SOURCES=openmc_tracer.cpp openmc_scenes.cpp

RAY_TEST_SOURCES=ray_test.cpp

OPENMC_DIR=/home/shriwise/opt/openmc

HDF5_DIR=/usr/lib/x86_64-linux-gnu/hdf5/serial

all: tracer

tracer: *.cpp *.h
	g++ -O2 $(TRACER_SOURCES) $(COMMON_SOURCES) -o tracer

tracer_dbg: *.cpp *.h
	g++ -g $(TRACER_SOURCES) $(COMMON_SOURCES) -o tracer

tracer_omp: *.cpp *.h
	g++ -O2 -fopenmp $(TRACER_SOURCES) $(COMMON_SOURCES) -pthread -o tracer

openmc_tracer: $(MC_TRACER_SOURCES)
	- g++ -g -I$(OPENMC_DIR)/include -I$(HDF5_DIR)/include $(MC_TRACER_SOURCES) $(COMMON_SOURCES) -L$(OPENMC_DIR)/lib -lopenmc -o openmc_tracer

openmc_tracer_omp: $(MC_TRACER_SOURCES)
	- g++ --std=c++14 -O2 -fopenmp -I$(OPENMC_DIR)/include -I$(HDF5_DIR)/include $(MC_TRACER_SOURCES) -pthread -L$(OPENMC_DIR)/lib -lopenmc -pthread -o openmc_tracer

ray_test: $(RAY_TEST_SOURCES)
	- g++ -g -fopenmp -I$(OPENMC_DIR)/include -I$(HDF5_DIR)/include $(RAY_TEST_SOURCES) $(COMMON_SOURCES) -pthread -L$(OPENMC_DIR)/lib -lopenmc -pthread -o ray_test

clean:
	- rm -rf openmc_tracer tracer
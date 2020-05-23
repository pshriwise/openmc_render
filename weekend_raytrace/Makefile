
COMMON_SOURCES=geom.cpp progress_bar.cpp trace.cpp color.cpp scene.cpp

TRACER_SOURCES=tracer.cpp $(COMMON_SOURCES)

MC_TRACER_SOURCES=openmc_tracer.cpp

RAY_TEST_SOURCES=ray_test.cpp $(COMMON_SOURCES)

OPENMC_DIR=/home/shriwise/opt/openmc

HDF5_DIR=/usr/lib/x86_64-linux-gnu/hdf5/serial

OPENMC_LIB_FLAGS=-L$(OPENMC_DIR)/lib -lopenmc

OPENMC_INCLUDE_DIRS=-I$(OPENMC_DIR)/include -I$(HDF5_DIR)/include

all: tracer

tracer: $(TRACER_SOURCES)
	g++ -O2 $(TRACER_SOURCES) -o tracer

tracer_dbg: *.cpp *.h
	g++ -g $(TRACER_SOURCES) -o tracer

tracer_omp: $(TRACER_SOURCES)
	g++ -O2 -fopenmp $(TRACER_SOURCES) -pthread -o tracer

openmc_tracer: $(TRACER_SOURCES)
	- g++ -DOPENMC -O2 $(OPENMC_INCLUDE_DIRS) $(TRACER_SOURCES) $(OPENMC_LIB_FLAGS) -o openmc_tracer

openmc_tracer_omp: $(TRACER_SOURCES)
	- g++ -DOPENMC -O2 -fopenmp $(OPENMC_INCLUDE_DIRS) $(TRACER_SOURCES) -pthread $(OPENMC_LIB_FLAGS) -pthread -o openmc_tracer

ray_test: $(RAY_TEST_SOURCES)
	- g++ -g -fopenmp $(OPENMC_INCLUDE_DIRS) $(RAY_TEST_SOURCES) -pthread $(OPENMC_LIB_FLAGS) -pthread -o ray_test

clean:
	- rm -rf openmc_tracer tracer
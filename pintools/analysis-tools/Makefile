include ../Makefile.inc

SOURCES=waste-record.cpp zero-reuse-record.cpp low-util-record.cpp $(CACHE_SOURCES) cache-waste-analysis.cpp main.cpp
CACHE_DIR = cache-sim
CACHE_SOURCES = $(CACHE_DIR)/cache-line.cpp $(CACHE_DIR)/cache-set.cpp $(CACHE_DIR)/cache.cpp
EXECUTABLE=cache-waste-analysis
OBJ = $(addsuffix .o, $(basename $(SRC)))

all: $(OBJ)
	$(CC) $(FLAGS) $(SOURCES) -o $(EXECUTABLE)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@
	
%.o: $(CACHE_DIR)/%.cpp
	$(CC) $(FLAGS) -c $< -o $@
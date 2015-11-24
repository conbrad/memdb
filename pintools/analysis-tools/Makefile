include ../Makefile.inc

RECORD_DIR = record
CACHE_DIR = cache-sim
UTIL_DIR = util
FUNCTION = variable

SOURCES = $(RECORD_SOURCES) $(UTIL_SOURCES) $(FUNCTION_SOURCES) $(CACHE_SOURCES) cache-waste-analysis.cpp waste-record-collection.cpp main.cpp
RECORD_SOURCES = $(RECORD_DIR)/waste-record.cpp $(RECORD_DIR)/zero-reuse-record.cpp $(RECORD_DIR)/low-util-record.cpp
CACHE_SOURCES = $(CACHE_DIR)/cache-line.cpp $(CACHE_DIR)/cache-set.cpp $(CACHE_DIR)/cache.cpp
UTIL_SOURCES = $(UTIL_DIR)/map-summarizer.h $(UTIL_DIR)/access-parser.cpp $(UTIL_DIR)/miss-sender.cpp $(UTIL_DIR)/access-log-receiver.cpp
FUNCTION_SOURCES = $(FUNCTION)/function.cpp $(FUNCTION)/variable-analyzer.cpp $(FUNCTION)/variable-access.cpp


OBJ = $(addsuffix .o, $(basename $(SRC)))

EXECUTABLE=cache-waste-analysis

all: $(OBJ)
	$(CC) $(FLAGS) $(SOURCES) -o $(EXECUTABLE)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@
	
%.o: $(CACHE_DIR)/%.cpp
	$(CC) $(FLAGS) -c $< -o $@
	
%.o: $(RECORD_DIR)/%.cpp
	$(CC) $(FLAGS) -c $< -o $@
OBJ=obj
BIN=bin
TARGET=$(BIN)/libnetw.so
TARGET_STATIC=$(BIN)/libnetw.a
FLAGS_C=-fPIC -O3 -std=c++20
FLAGS_C_STATIC=-O3 -std=c++20
FLAGS_L=-shared
FLAGS_L_STATIC=
TIME=/usr/bin/time
TIME_FORMATTING="Build took %E"
VALGRIND_OUT=$(TARGET)_valgrind

CPP_SOURCES=$(wildcard *.cpp)
H_SOURCES=$(wildcard *.h)
OBJECTS=$(patsubst %.cpp,$(OBJ)/%.o,$(CPP_SOURCES))
OBJECTS_STATIC=$(OBJECTS)

default: build

ifeq (leak_check,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

leak_check:
	valgrind --log-file=$(VALGRIND_OUT) --leak-check=full $(TARGET) $(RUN_ARGS)
	cat $(VALGRIND_OUT) | less

build:
	@$(TIME) --format=$(TIME_FORMATTING) make build_s2 --no-print-directory

static:
	@$(TIME) --format=$(TIME_FORMATTING) make build_s2_static --no-print-directory

build_s2: $(TARGET)

build_s2_static: FLAGS_C=$(FLAGS_C_STATIC)
build_s2_static: FLAGS_L=$(FLAGS_L_STATIC)
build_s2_static: $(TARGET_STATIC)

$(TARGET): $(OBJECTS)
	g++ $(OBJECTS) -o $(TARGET) $(FLAGS_L)

$(TARGET_STATIC): $(OBJECTS_STATIC)
	ar rvs $(TARGET_STATIC) $(OBJECTS_STATIC)

$(OBJ)/%.o: %.cpp
	g++ -c $^ -o $@ $(FLAGS_C)

setup:
	mkdir $(OBJ)
	mkdir $(BIN)

clean:
	rm -r $(OBJ)
	mkdir -p $(OBJ)
clear:
	rm -r $(OBJ) $(BIN)
	mkdir -p $(OBJ) $(BIN)

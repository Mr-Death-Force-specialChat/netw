BIN=bin
TARGET=$(BIN)/stc
FLAGS_C=-std=c++20 ../../bin/libnetw.a

build: $(TARGET)

$(TARGET): example.cpp
	g++ example.cpp -O3 -o $(TARGET) $(FLAGS_C)

setup:
	mkdir $(BIN)

clean:
	rm -r $(BIN)
	mkdir $(BIN)

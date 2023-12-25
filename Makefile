CC := g++
CFLAGS := -Wall -std=c++11 -I/usr/include/SDL2
LDFLAGS := -lSDL2 -lSDL2_image

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))
EXECUTABLE := $(BIN_DIR)/snake.tsss

.PHONY: all clean run

all: clean $(EXECUTABLE)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(BUILD_DIR)/*.o $(BIN_DIR)/*

run: $(EXECUTABLE)
	./$(EXECUTABLE)

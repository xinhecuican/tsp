CC = g++
CFLAGS = -g -std=c++17

SRC_DIR = ./
INCLUDE_DIR = ./
BUILD_DIR = build/

src = $(wildcard $(SRC_DIR)*.cpp)

$(BUILD_DIR)main: $(src)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -o $@ $(src)

run: $(BUILD_DIR)main
	./build/main

debug: $(BUILD_DIR)main
	gdb build/main

.PHONY: clean
clean:
	rm -rf build/*

clean_windows:
	del /Q build\*
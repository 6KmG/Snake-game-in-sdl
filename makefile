# Define compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -std=c11

# Define source and target
SRC := main.c
TARGET := main

# Check the OS
ifeq ($(OS),Windows_NT)
    # Windows specific settings
    SDL_CFLAGS := $(shell sdl2-config --cflags)
    SDL_LDFLAGS := $(shell sdl2-config --libs)
    RM := del /Q
else
    # Linux specific settings
    SDL_CFLAGS := $(shell sdl2-config --cflags)
    SDL_LDFLAGS := $(shell sdl2-config --libs)
    RM := rm -f
endif

# Build target
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -o $(TARGET) $(SRC) $(SDL_LDFLAGS)

# Clean target
.PHONY: clean
clean:
	$(RM) $(TARGET)

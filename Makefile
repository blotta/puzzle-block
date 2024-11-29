CC := g++
LIBS := sdl2 sdl2_image sdl2_ttf
PKG_CONFIG := pkg-config

CFLAGS := $(shell $(PKG_CONFIG) --cflags $(LIBS))
LDFLAGS := $(shell $(PKG_CONFIG) --libs $(LIBS))

CFLAGS := $(CFLAGS) -Werror -Wall -std=c++20

TARGET := .\build\game
SRCS := $(wildcard src/*.cpp)

all: debug

# Build rules
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)

.PHONY: debug
debug: $(SRCS)
	$(CC) -g -mconsole $(CFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)

.PHONY: release
release: $(SRCS)
	$(CC) -O2 $(CFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)

.PHONY: run
run: debug
	$(TARGET)

clean:
	rm $(TARGET)

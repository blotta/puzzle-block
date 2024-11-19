CC := g++
LIBS := sdl2
PKG_CONFIG := pkg-config

CFLAGS := $(shell $(PKG_CONFIG) --cflags $(LIBS))
LDFLAGS := $(shell $(PKG_CONFIG) --libs $(LIBS))

CFLAGS := $(CFLAGS) -Werror -Wall

TARGET := .\build\game
SRCS := src\main.cpp

all: debug

# Build rules
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)

.PHONY: debug
debug: $(SRCS)
	$(CC) -g $(CFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)

.PHONY: release
release: $(SRCS)
	$(CC) -O2 $(CFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)

.PHONY: run
run: $(TARGET)
	$(TARGET)

clean:
	rm $(TARGET)

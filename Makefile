CC := g++
LIBS := sdl2 sdl2_image sdl2_ttf
PKG_CONFIG := pkg-config

CFLAGS := $(shell $(PKG_CONFIG) --cflags $(LIBS))
LDFLAGS := $(shell $(PKG_CONFIG) --libs $(LIBS))

CFLAGS := $(CFLAGS) -Werror -Wall -std=c++20

EXE := .\build\game.exe
SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:src/%.cpp=obj/%.o)

all: debug

.PHONY: debug
debug: CFLAGS += -g -mconsole
debug: LDFLAGS += -g -mconsole
debug: $(EXE)

.PHONY: release
release: CFLAGS += -O2
release: clean $(EXE)

$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY: run
run: debug
	$(EXE)

.PHONY: clean
clean:
	del $(EXE) .\obj\*.o

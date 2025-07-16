CC := g++
#CC := clang++
LIBS := SDL2 SDL2_image SDL2_ttf SDL2_mixer
PKG_CONFIG := pkg-config

CFLAGS := $(shell $(PKG_CONFIG) --cflags $(LIBS)) -Werror -Wall -std=c++20
LDFLAGS := $(shell $(PKG_CONFIG) --libs-only-L --libs-only-l $(LIBS))

EXE := .\build\windows\game.exe
SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:src/%.cpp=obj/%.o)

all: debug

.PHONY: debug
debug: CFLAGS += -g -DDEBUG
debug: $(EXE)

.PHONY: release
release: CFLAGS += -O2
release: LDFLAGS += $(shell $(PKG_CONFIG) --libs-only-other $(LIBS))
release: clean $(EXE)

$(EXE): $(OBJS)
	mkdir -p build\windows
	$(CC) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY: run
run: debug
	$(EXE)

.PHONY: clean
clean:
	rm -f build/windows/*
	rm -f obj/*.o

SRCDIR := src
OUTDIR := build
SRCS := $(wildcard $(SRCDIR)/*.cpp)
TARGETS := $(basename $(SRCS))

CXX := clang++
CFLAGS := -Wall -O2 -I./include

SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LIBS := $(shell sdl2-config --libs)

.PHONY: all clean deps
all: $(TARGETS)

clean:
	rm -rf $(OUTDIR)

$(TARGETS):
	mkdir -p $(OUTDIR)
	$(CXX) $(SDL2_CFLAGS) $(SDL2_LIBS) $(CFLAGS) -o $(addprefix $(OUTDIR)/,$(notdir $@)) $@.cpp

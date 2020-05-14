SRCDIR := src
OUTDIR := build
SRCS := $(wildcard $(SRCDIR)/*.cpp)
TARGETS := $(basename $(SRCS))

CC = gcc
CXX = g++
CFLAGS = -Wall -O2 -I/usr/local/include/SDL2 -I/opt/X11/include -D_THREAD_SAFE -L/usr/local/lib -lSDL2

.PHONY: all clean deps
all: $(TARGETS)

clean:
	rm -rf $(OUTDIR)

$(TARGETS):
	mkdir -p $(OUTDIR)
	$(CXX) $(CFLAGS) -o $(addprefix $(OUTDIR)/,$(notdir $@)) $@.cpp

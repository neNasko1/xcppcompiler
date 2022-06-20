SHELL := /bin/bash
SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(shell find src/ -type f -name '*.cpp')
OBJ_FILES := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS :=
CPPFLAGS := -std=c++2a
CXXFLAGS :=

compiler: $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p "$$(dirname $@)"
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<



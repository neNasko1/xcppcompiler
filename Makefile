SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(shell find src/ -type f -name '*.cpp')
OBJ_FILES := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS :=
CPPFLAGS := -std=c++2a
CXXFLAGS :=
EXECUTABLE := compiler

$(EXECUTABLE): $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p "$$(dirname $@)"
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

rm:
	@echo "Removing all compiled files"
	@rm -r obj || :
	@rm $(EXECUTABLE) || :

test:
	@make rm
	@make
	@./test.sh


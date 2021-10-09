appname := compiler
 
CXX := g++
CXXFLAGS := -std=c++20

srcfiles := $(shell find . -wholename "./*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))
objects  := $(patsubst %./, %./build/, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(appname) $(objects) $(LDLIBS)

depend: .depend

.depend: $(srcfiles)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	rm -f $(objects)

dist-clean: clean
	rm -f *~ .depend

include .depend

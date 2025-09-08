CXX       := g++
CXXFLAGS  := -std=c++2b -Wall -Wextra -Werror -O2 -Iinclude -g
LDFLAGS   := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -pthread
TESTFLAGS := -lgtest -lgtest_main

SRCFILES := $(shell find src -name '*.cpp')
SRCOBJS  := $(patsubst src/%.cpp,obj/src/%.o,$(SRCFILES))

TESTFILES := $(shell find test -name '*.cpp')
TESTOBJS  := $(patsubst test/%.cpp,obj/test/%.o,$(TESTFILES))

.PHONY: all clean

all: bin/simulate bin/tests

clean:
	rm -rf obj bin

bin/simulate: $(SRCOBJS) obj/simulate.o
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $^ $(LDFLAGS)

bin/tests: $(SRCOBJS) $(TESTOBJS)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $^ $(LDFLAGS) $(TESTFLAGS)

obj/src/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/test/%.o: test/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

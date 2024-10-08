# Makefile
# Build rules for EECS 280 project 3

# Compiler
CXX ?= g++

# Compiler flags
CXXFLAGS ?= --std=c++17 -Wall -Werror -pedantic -g -Wno-sign-compare -Wno-comment

# Run a regression test
test: Game_tests.exe
	./Game_tests.exe

Game_tests.exe: Game_tests.cpp Game.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

pirate.exe: pirate.cpp CommandUI.cpp Game.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

pirate-keyboard.exe: pirate.cpp KeyboardUI.cpp Game.cpp
	$(CXX) $(CXXFLAGS) $^ -DUSE_KEYBOARD_UI -lcurses -o $@

.SUFFIXES:

.PHONY: clean

clean:
	rm -rvf *.out *.exe *.dSYM *.stackdump
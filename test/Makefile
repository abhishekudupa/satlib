SATLIB_ROOT=..
STATIC_LINK_LIBS=-lsatlib -lminisat
DYNAMIC_LINK_LIBS=
LINK_FLAGS+=-L $(SATLIB_ROOT)/lib/$(BUILD_SUFFIX) 

include $(SATLIB_ROOT)/Makefile.inc

CXXFLAGS+=-I $(SATLIB_ROOT)/src/include

.PHONY:				clean

default:			debug
debug:				all
opt:				all
eopt:				all
eprof:				all
eproflto:			all
eoptlto:			all

all:				SatLibTest SudokuSolver

SatLibTest:			SatLibTest.cpp
					g++ $(CXXFLAGS) -o SatLibTest.o -c $<
					g++ $(CXXFLAGS) SatLibTest.o $(LINK_FLAGS) -o $@ -Wl,-Bstatic -lsatlib -Wl,-Bdynamic -ldl

SudokuSolver:		SudokuSolver.cpp
					g++ $(CXXFLAGS) -o SudokuSolver.o -c $<
					g++ $(CXXFLAGS) SudokuSolver.o $(LINK_FLAGS) -o $@ -Wl,-Bstatic -lsatlib -Wl,-Bdynamic -ltinyxml2 -ldl

clean:
					rm -rf *.o SatLibTest SudokuSolver

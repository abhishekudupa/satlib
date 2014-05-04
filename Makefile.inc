BUILD_SUFFIX=debug
OPTFLAGS=-ggdb3 -O0 -fno-inline
FEAT_FLAGS=-fPIC -std=c++11

ifeq ($(MAKECMDGOALS), opt)
BUILD_SUFFIX=opt
OPTFLAGS=-O2
endif
ifeq ($(MAKECMDGOALS), optlto)
BUILD_SUFFIX=opt
OPTFLAGS=-O2 -flto
endif
ifeq ($(MAKECMDGOALS), prof)
BUILD_SUFFIX=prof
OPTFLAGS=-O2 -pg
endif
ifeq ($(MAKECMDGOALS), proflto)
BUILD_SUFFIX=prof
OPTFLAGS=-O2 -pg -flto
endif
ifeq ($(MAKECMDGOALS), eopt)
BUILD_SUFFIX=opt
OPTFLAGS=-O3
endif
ifeq ($(MAKECMDGOALS), eoptlto)
BUILD_SUFFIX=opt
OPTFLAGS=-O3 -flto
endif
ifeq ($(MAKECMDGOALS), eprof)
BUILD_SUFFIX=prof
OPTFLAGS=-O3 -pg
endif
ifeq ($(MAKECMDGOALS), eproflto)
BUILD_SUFFIX=prof
OPTFLAGS=-O3 -pg -flto
endif
CXX=g++
CC=g++
CXXFLAGS+=-Wall -I $(SATLIB_ROOT)/src/include -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS
CXXFLAGS+=-pipe
CXXFLAGS+=$(FEAT_FLAGS)
CXXFLAGS+=$(OPTFLAGS)
AR=ar
ARFLAGS=rcs
LD=$(CXX) $(CXXFLAGS) -shared

.PHONY:				debug clean opt prof all default eprof eopt optlto proflto eoptlto eproflto

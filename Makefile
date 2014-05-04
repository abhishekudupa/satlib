SATLIB_ROOT=.

include Makefile.inc

SRC_DIR=$(SATLIB_ROOT)/src
GLUCOSE_SRC_DIR=$(SRC_DIR)/glucose
MINISAT_SRC_DIR=$(SRC_DIR)/minisat
OBJ_DIR=$(SATLIB_ROOT)/obj/$(BUILD_SUFFIX)
LIB_DIR=$(SATLIB_ROOT)/lib/$(BUILD_SUFFIX)
SATLIB_PLUGIN_PATH=$(realpath $(SATLIB_ROOT))/lib/plugins

CXXFLAGS+=-DSATLIB_PLUGIN_PATH=\"$(SATLIB_PLUGIN_PATH)\"

PYTHON_SRCS= \
				$(SATLIB_ROOT)/src/python/satlib.py \
				$(SATLIB_ROOT)/src/python/satlibcore.py \


PYTHON_LIBS= \
				$(SATLIB_ROOT)/lib/python/satlib.py \
				$(SATLIB_ROOT)/lib/python/satlibcore.py \


SATLIB_SOURCES= \
				ExpressionBase.cpp \
				ExpressionManager.cpp \
				ExpressionPrinter.cpp \
				ExpressionVisitorBase.cpp \
				SatLibExceptions.cpp \
				UIDGenerator.cpp \
				CNFConverterBase.cpp \
				NNFConverterBase.cpp \
				SimpleNNFConverter.cpp \
				TseitinTransform.cpp \
				TseitinTransformFull.cpp \
				Literal.cpp \
				SatLibSolverBase.cpp \
				SatLibMinisatSolver.cpp \
				SatLibGlucoseSolver.cpp \
				SatLibCIFace.cpp \
				PluginLoaderBase.cpp \
				LingelingLoader.cpp \
				SatLibLingelingSolver.cpp \
				SatLibZ3Solver.cpp \


GLUCOSE_SOURCES= \
				GlucoseSolver.cpp \
				GlucoseOptions.cpp \
				GlucoseSystem.cpp \
				GlucoseSimpSolver.cpp \


MINISAT_SOURCES= \
				MinisatSolver.cpp \
				MinisatOptions.cpp \
				MinisatSystem.cpp \
				MinisatSimpSolver.cpp \

SATLIB_PLUGINS= \
				liblgl-aqw \
				libz3 \


VPATH=$(SRC_DIR)
VPATH+=:$(SRC_DIR)/external/
VPATH+=:$(GLUCOSE_SRC_DIR)/simp
VPATH+=:$(GLUCOSE_SRC_DIR)/utils
VPATH+=:$(GLUCOSE_SRC_DIR)/core
VPATH+=:$(MINISAT_SRC_DIR)/simp
VPATH+=:$(MINISAT_SRC_DIR)/utils
VPATH+=:$(MINISAT_SRC_DIR)/core


SATLIB_OBJS=$(addprefix $(OBJ_DIR)/, $(SATLIB_SOURCES:.cpp=.o))
SATLIB_DEPS=$(addprefix $(OBJ_DIR)/, $(SATLIB_SOURCES:.cpp=.d))

GLUCOSE_OBJS=$(addprefix $(OBJ_DIR)/, $(GLUCOSE_SOURCES:.cpp=.o))
GLUCOSE_DEPS=$(addprefix $(OBJ_DIR)/, $(GLUCOSE_SOURCES:.cpp=.d))

MINISAT_OBJS=$(addprefix $(OBJ_DIR)/, $(MINISAT_SOURCES:.cpp=.o))
MINISAT_DEPS=$(addprefix $(OBJ_DIR)/, $(MINISAT_SOURCES:.cpp=.d))

SATLIB_PLUGIN_TARGETS=$(addsuffix .so, $(addprefix $(SATLIB_PLUGIN_PATH)/lib, $(SATLIB_PLUGINS)))

SATLIB_STATIC=$(LIB_DIR)/libsatlib.a
SATLIB_DYNAMIC=$(LIB_DIR)/libsatlib.so

default:			debug
debug:				all
opt:				all
optlto:				all
prof:				all
proflto:			all
eprof:				all
eproflto:			all
eopt:				all
eoptlto:			all

all:				satlib $(SATLIB_PLUGINS)

satlib:			$(SATLIB_STATIC) $(SATLIB_DYNAMIC) $(PYTHON_LIBS)

# we link in the sat solver objs as well
$(SATLIB_DYNAMIC):		$(SATLIB_DEPS) $(SATLIB_OBJS) $(GLUCOSE_OBJS) $(GLUCOSE_DEPS) $(MINISAT_OBJS) $(MINISAT_DEPS)
	$(LD) -o $@ $(SATLIB_OBJS) $(GLUCOSE_OBJS) $(MINISAT_OBJS)
	cp $@ $(SATLIB_ROOT)/lib/python

$(SATLIB_STATIC):		$(SATLIB_DEPS) $(SATLIB_OBJS) $(GLUCOSE_OBJS) $(GLUCOSE_DEPS) $(MINISAT_OBJS) $(MINISAT_DEPS)
	$(AR) $(ARFLAGS) $@ $(SATLIB_OBJS) $(GLUCOSE_OBJS) $(MINISAT_OBJS)

$(OBJ_DIR)/%.d:		%.cpp
	@set -e; rm -f $@; \
	echo "Computing dependencies for `basename $<`..."; \
	$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
    sed 's,\($*\)\.o[ :]*,$(OBJ_DIR)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJ_DIR)/%.o:		%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SATLIB_ROOT)/lib/python/satlib.py: $(SATLIB_ROOT)/src/python/satlib.py
	cp $< $@

$(SATLIB_ROOT)/lib/python/satlibcore.py: $(SATLIB_ROOT)/src/python/satlibcore.py
	cp $< $@

$(SATLIB_PLUGINS):
	$(MAKE) -C $(SATLIB_ROOT)/src/$@; cp $(addprefix $(SATLIB_ROOT)/src/$@/, $(addsuffix .so, $@)) $(addsuffix .so, $(addprefix $(SATLIB_PLUGIN_PATH)/, $@))

.PHONY: clean $(SATLIB_PLUGINS) fullclean

clean:
	rm -rf obj/debug/*
	rm -rf obj/opt/*
	rm -rf obj/prof/*
	rm -rf lib/debug/*
	rm -rf lib/opt/*
	rm -rf lib/prof/*
	rm -rf lib/python/*
	rm -rf lib/plugins/*
	for plugin in $(SATLIB_PLUGINS); do cd $(SATLIB_ROOT)/src/$$plugin; make clean; cd ../../; done

fullclean: clean
	cd $(SATLIB_ROOT)/src/libz3; make fullclean; cd ../../; done

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), fullclean)
-include $(MINISAT_DEPS)
-include $(GLUCOSE_DEPS)
-include $(SATLIB_DEPS)
endif
endif


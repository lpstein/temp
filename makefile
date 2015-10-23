# Find all code to compile in src/ and put the output in obj/
SOURCES := $(wildcard src/*.c*)
OBJECTS := $(patsubst src/%,%,$(SOURCES)) # Remove obj/ prefix
OBJECTS := $(patsubst %.cpp,%.o,$(OBJECTS)) # Convert .cpp to .o
OBJECTS := $(addprefix obj/,$(OBJECTS)) # Prefix with obj/

# Vendor stuff
GLFW := obj/vendor/lib/libglfw3.a
GIFLIB := obj/vendor/dgif_lib.o obj/vendor/gif_err.o obj/vendor/gifalloc.o
VENDOR := $(GIFLIB) $(GLFW)

# Name of the PCH source file
PCH := src/precompiled.hpp
PCH_TARGET := $(addsuffix .pch,$(PCH))

# The compiler we use.  Should probalby be platform specific...
CXX := clang++

# These flags aren't used for vendor code at all
INCLUDES += -I vendor/glfw-3.1.2/include
INCLUDES += -I vendor/giflib-5.1.1/lib
CXXFLAGS := -c -std=c++1y -g -Wno-deprecated-declarations
CXXFLAGS += $(INCLUDES)

# Global final build flags
BUILD_FLAGS := -L obj/vendor/lib
BUILD_FLAGS += -l glfw3

# Platform specific portion of final build flags
UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
	BUILD_FLAGS += -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
	BUILD_FLAGS += -l pthread
endif
ifeq ($(UNAME),Linux)
	# TODO
endif

# Lifecycle stuff

all: release/gotm | prereqs
prereqs:
	@which cmake >/dev/null || brew install cmake
test: all
	@echo ""
	@release/gotm sample/collin.gif
	# @release/gotm sample/flytrap.gif
clean-all:
	@rm -rf ./obj || true
	@rm -rf ./release || true
	@rm -rf ./src/*.pch || true
	@echo "Cleaned up"
clean:
	@rm -rf ./obj/*.o || true
	@rm -rf ./release || true
	@rm -rf ./src/*.pch || true
	@echo "Cleaned up non-vendor"
dirs:
	@mkdir -p obj/
	@mkdir -p obj/vendor
	@mkdir -p obj/vendor/lib
	@mkdir -p release/
.PHONY: all test clean dirs

# Actual compilation work

obj/%.o: src/%.cpp $(PCH_TARGET) | dirs
	@echo $<
	@$(CXX) $(CXXFLAGS) -include $(PCH) $< -o $@

release/gotm: $(VENDOR) $(OBJECTS) | dirs
	@echo $@
	@$(CXX) $(BUILD_FLAGS) $(OBJECTS) $(VENDOR) -o $@

# Precompiled header

$(PCH_TARGET): $(PCH)
	@echo $<
	@$(CXX) $(CXXFLAGS) -x c++-header $< -o $@

# Vendor

$(GIFLIB): | dirs
	@cd vendor/giflib-5.1.1; ./configure; make
	@cp vendor/giflib-5.1.1/lib/*.o obj/vendor
	@cd vendor/giflib-5.1.1; make distclean

$(GLFW): | dirs
	@mkdir -p vendor/glfw-3.1.2/_build
	@cd vendor/glfw-3.1.2/_build; cmake ..; make
	@cp vendor/glfw-3.1.2/_build/src/libglfw3.a $@
	@rm -rf vendor/glfw-3.1.2/_build

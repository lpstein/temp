# Find all code to compile in src/ and put the output in obj/
SOURCES := $(wildcard src/*.c*)
OBJECTS := $(patsubst src/%,%,$(SOURCES)) # Remove obj/ prefix
OBJECTS := $(patsubst %.cpp,%.o,$(OBJECTS)) # Convert .cpp to .o
OBJECTS := $(addprefix obj/,$(OBJECTS)) # Prefix with obj/

# Vendor stuff
GLFW := obj/vendor/lib/libglfw3.a
GIFLIB := obj/vendor/dgif_lib.o obj/vendor/gif_err.o obj/vendor/gifalloc.o
VENDOR := $(GIFLIB) $(GLFW)

# These flags aren't used for vendor code at all
CXXFLAGS := -c -std=c++11 -g -L obj/vendor/lib # Base
CXXFLAGS += -I vendor/glfw-3.1.2/include -l glfw3 # Include GLFW
CXXFLAGS += -I vendor/giflib-5.1.1/lib # Include giflib

# These flags are used for the final compilation and are platform specific
UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
	BUILD_FLAGS += -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
endif
ifeq ($(UNAME),Linux)
	# TODO
endif

# Lifecycle stuff

all: release/gotm
test: all
	@echo ""
	@release/gotm
clean:
	@rm -rf ./obj || true
	@rm -rf ./release || true
	@echo "Cleaned up"
dirs:
	@mkdir -p obj/
	@mkdir -p obj/vendor
	@mkdir -p obj/vendor/lib
	@mkdir -p release/
.PHONY: all test clean dirs

# Actual compilation work

obj/%.o: src/%.cpp | dirs
	g++ $(CXXFLAGS) $< -o $@

release/gotm: $(VENDOR) $(OBJECTS) | dirs
	g++ $(BUILD_FLAGS) $(OBJECTS) $(VENDOR) -o $@

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

SOURCES := $(wildcard src/*.c*)
OBJECTS := $(patsubst src/%,%,$(SOURCES)) # Remove obj/ prefix
OBJECTS := $(patsubst %.cpp,%.o,$(OBJECTS)) # Convert .cpp to .o
OBJECTS := $(addprefix obj/,$(OBJECTS)) # Prefix with obj/

# Vendor stuff
GIFLIB := obj/vendor/dgif_lib.o obj/vendor/gif_err.o obj/vendor/gifalloc.o
VENDOR := $(GIFLIB)

CXXFLAGS := -c -std=c++11 -g

all: release/gotm
test: | all
	@release/gotm

clean:
	@rm -rf ./obj || true
	@rm -rf ./release || true

dirs:
	@mkdir -p obj/
	@mkdir -p obj/vendor
	@mkdir -p release/

obj/%.o: src/%.cpp | dirs
	g++ $(CXXFLAGS) $< -o $@

release/gotm: $(OBJECTS) $(VENDOR) | dirs
	g++ $(OBJECTS) $(VENDOR) -o $@

# Vendor crap

$(GIFLIB): | dirs
	cd vendor/giflib-5.1.1; ./configure; make
	cp vendor/giflib-5.1.1/lib/*.o obj/vendor


# Makefile for Neural Network library
#
#
# Created by: Robert May
#
# Last Updated: 6/10/2011
#
Flags = -Wall -O3 -fpermissive
Remove = del -/F

Library = ./lib/libneural.a

SourceFolders:= \
	ann \
	data \
	glr \
	kde

Source :=
Source += $(foreach Folder, $(SourceFolders), $(wildcard src/$(Folder)/*.cpp))

Objects = $(patsubst src/%.cpp, obj/%.o, $(Source))

BinariesSource= $(wildcard ./examples/src/*.cpp)
Binaries = $(patsubst ./examples/src/%.cpp, ./examples/bin/%.exe, $(BinariesSource))

all: $(Objects) $(Binaries)
	@echo Building target library: $(Library)
	@ar -r $(Library) $(Objects)
	@echo Done.
	@echo
	@echo Listing contents of library: $(Library)
	@ar -t $(Library)
	@echo
	@echo All done.
	@echo
	
obj/%.o: src/%.cpp
	@echo Compiling $@ from source files: $^
	@g++ -c $(Flags) $< -o $@ -I./src

examples/bin/%.exe: examples/src/%.cpp
	@echo Building $@ from $<
	g++ $(F;ags) $< -o$@ -I./src -L./lib -lneural
	 
clean:


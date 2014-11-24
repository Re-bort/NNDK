# Makefile to make example programmes using the Neural Network Development Kit

IncludeDirs=../include/src
LibraryDirs=../lib
Libraries=nndk

Source= $(wildcard src/*.cpp)
Binary= $(patsubst src/%.cpp, bin/%, $(Source))

Build = g++
Compile = g++ -c
Flags = -g
Remove = rm -f

all: $(Binary)
		
bin/%: src/%.cpp
	@echo Building $@ from $<
	$(Build) $< -o$@ $(Flags) \
	$(patsubst %, -I%, $(IncludeDirs)) \
	$(patsubst %, -L%, $(LibraryDirs)) \
	$(patsubst %, -l%, $(Libraries))

	
clean: $(patsubst %, %.clean, $(Binary))

%.clean: %
	${Remove} $<
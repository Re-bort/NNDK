# Makefile for Neural Network library
#
#
# Created by: Robert May
#
# Last Updated: 6/10/2011
#

Archive = ar -r
List=ar -t
Build = g++
Compile = g++ -c
Flags = -Wall -O3 -fpermissive
DebugFlags = "-Wall -g"
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

all: $(Objects)
	@echo Building target library: $(Library)
	@$(Archive) $(Library) $(Objects)
	@echo Done.
	@echo
	@echo Listing contents of library: $(Library)
	@$(List) $(Library)
	@echo
	@echo All done.
	@echo
	
obj/%.o: src/%.cpp
	@echo Compiling $@ from source files: $^
	@$(Compile) $(Flags) $< -o $@

clean:
	@echo Cleaning source object files:
	$(foreach Object, $(Objects), $(Remove) $(Object))
	#@echo Done.
	#@echo Cleaning target library file:
	#@echo $(Remove) $(Library)
	#@echo Done.
	#@echo Done cleaning.

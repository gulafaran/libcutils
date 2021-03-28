# Compiler
CC=gcc
# Compiler flags
override CFLAGS+=-g -Wall -Wextra -fPIC
override LDLIBS+=-lm -shared
# Final executable name 
EXE=libcutils.so
# source code directory
SRC=./src
# objects files and executable directory
BIN=./bin
# possible directories where header files could be
override HEADERS+=-I./include
# c files to compile 
SOURCES=$(shell find src -name "*".c)
# object files names
OBJECTS=$(SOURCES:src/%.c=bin/%.o)
TESTS_OBJECTS=$(filter-out bin/main.o, $(OBJECTS))

# main recipe
$(BIN)/$(EXE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDLIBS)

# compiles to object a source file in the source code directory 
$(BIN)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(HEADERS) -c $(CFLAGS) $< -o $@ $(LDLIBS)

# shortcut to add compilation flags
obj: $(OBJECTS)

# compiles to an executable a source file in the source directory, except for the main one
$(BIN)/%.tst: $(TESTS_OBJECTS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(TESTS_OBJECTS) -o $@ $(LDLIBS)

# phony in case a file is named clean ??
.PHONY:clean
clean:
	[ -n $(BIN) ] && rm -rf $(BIN)/* && rm -rf $(OBJECTS)


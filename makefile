# Programs
CC = gcc
CXX = g++
LD = gcc
MKDIR = mkdir -p
RM = rm -rf

# Arguments
CXXFLAGS = -g -Wall -std=c++17
OUTPUT_OPTION = -MMD -MP -o $@
LDLIBS = -lm

# Directories
ETC = etc/
SRC = src/
BLD = build/

# Files
O_FILES = $(subst .cpp,.o,$(subst $(SRC),$(BLD),$(shell find $(SRC) -name *.cpp)))
D_FILES = $(subst .o,.d,$(O_FILES))

# Rules
.PHONY: all objects dir clean

all: dir enjoy

dir:
	$(MKDIR) $(BLD)

enjoy: objects
	$(CXX) $(CXXFLAGS) -o $@ $(O_FILES)

objects: $(O_FILES)

-include $(D_FILES)

$(BLD)%.o: $(SRC)%.cpp
	$(CXX) $(CXXFLAGS) $(OUTPUT_OPTION) -c $<

clean:
	rm -rf $(BLD)
CXX := g++
WINDRES := windres

INCLUDE_DIR := headers
SRC_DIR := sources
RES_DIR := resources
OBJ_DIR := obj

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXE_NAME := $(OBJ_DIR)/bin/test

CXXFLAGS := -iquote $(INCLUDE_DIR) -lgdi32 -ldwmapi -lgdiplus -mwindows -municode

all: $(EXE_NAME)

$(OBJ_DIR)/resources.o: $(RES_DIR)/resources.rc
	$(WINDRES) -i $< --include-dir=$(INCLUDE_DIR) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(EXE_NAME): $(OBJ_FILES) $(OBJ_DIR)/resources.o
	$(CXX) -o "$@" $^ $(CXXFLAGS)

clean:
	rm -f $(OBJ_DIR)/*.o "$(EXE_NAME)"

re: clean all

.PHONY: all clean re

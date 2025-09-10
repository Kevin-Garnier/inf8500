SYSTEMC_DIR	=	'/home/kevin/programs/systemc-2.3.3'
PROJETINC	=	'$(SYSTEMC_DIR)/include'
PROJETLIB	=	'$(SYSTEMC_DIR)/lib'

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/executable
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CXXFLAGS := -I include -I $(PROJETINC) -MMD -MP -std=c++17
CFLAGS   := -g -O0 -Wall
LDFLAGS  := -L${PROJETLIB} -lsystemc -std=c++17
LDLIBS   := -lm

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

run:
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(PROJETLIB) ./$(EXE)

-include $(OBJ:.o=.d)

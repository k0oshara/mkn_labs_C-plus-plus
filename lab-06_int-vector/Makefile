.PHONY: all clean

CFLAGS = -I$(INC_DIR)

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

TARGET = lab-06_int-vector

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	g++ $^ -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
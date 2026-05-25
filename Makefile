CC = gcc
CFLAGS = -Wall -Wextra -I$(SRC_DIR) -I$(CHIP8_DIR) -I$(GRAPHIC_DIR)
LDFLAGS = -lSDL2 -lm  

SRC_DIR = src
CHIP8_DIR = $(SRC_DIR)/Chip8
GRAPHIC_DIR = $(SRC_DIR)/Graphic
BUILD_DIR = build
BIN_DIR = bin

SOURCES = $(SRC_DIR)/main.c \
          $(CHIP8_DIR)/chip8.c \
          $(GRAPHIC_DIR)/graphic.c

OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TARGET = $(BIN_DIR)/chip8_emulator

all: $(TARGET)

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

$(BUILD_DIR)/Chip8 $(BUILD_DIR)/Graphic:
	mkdir -p $@

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Compilation complete! Executable: $(TARGET)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Chip8/%.o: $(CHIP8_DIR)/%.c | $(BUILD_DIR)/Chip8
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Graphic/%.o: $(GRAPHIC_DIR)/%.c | $(BUILD_DIR)/Graphic
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Temporary files and executable removed."

distclean: clean
	@echo "Complete cleanup done."

run: $(TARGET)
	./$(TARGET)

debug: CFLAGS += -g -DDEBUG
debug: clean all

help:
	@echo "Available commands:"
	@echo "  make         - Compile the program"
	@echo "  make clean   - Remove object files and executable"
	@echo "  make run     - Compile and run the program"
	@echo "  make debug   - Compile with debug flags"
	@echo "  make help    - Show this help"

.PHONY: all clean distclean run debug help

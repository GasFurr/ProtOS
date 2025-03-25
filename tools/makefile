CC = gcc
CFLAGS = -Wall -Wextra
BUILD_DIR = build
TARGET = $(BUILD_DIR)/tool

.PHONY: all clean

all: $(TARGET)

$(TARGET): src/tool.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)

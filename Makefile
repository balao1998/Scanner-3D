# MCU and Clock
MCU = atmega328p
F_CPU = 16000000UL

# Tools
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# Programmer (change as needed)
PROGRAMMER = usbasp
AVRDUDE_FLAGS = -c $(PROGRAMMER) -p $(MCU)

# Directories
SRC_DIR = src
BUILD_DIR = build

# File names
TARGET = scanner
TARGET_ELF = $(BUILD_DIR)/$(TARGET).elf
TARGET_HEX = $(BUILD_DIR)/$(TARGET).hex

# Flags
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -I$(SRC_DIR)

# Source and Object files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

# Default rule
all: $(TARGET_HEX)

# Build rules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_ELF): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_HEX): $(TARGET_ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Upload to microcontroller
flash: $(TARGET_HEX)
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$(TARGET_HEX):i

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all flash clean

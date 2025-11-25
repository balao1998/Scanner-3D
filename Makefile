# MCU and Clock
MCU = atmega328p
F_CPU = 16000000UL

# Tools
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# Programmer (change as needed)
PROGRAMMER = arduino
BAUDRATE = 57600
IP_ADRESS = 10.124.98.252:23
AVRDUDE_FLAGS = -c $(PROGRAMMER) -p $(MCU) -b $(BAUDRATE) -P net:$(IP_ADRESS)

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

#Linker flags for float support
LDFLAGS = -Wl,-u,vfprintf -lprintf_flt

# Default rule
all: $(TARGET_HEX)

# Build rules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_ELF): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(TARGET_HEX): $(TARGET_ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Upload to microcontroller
flash: $(TARGET_HEX)
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$(TARGET_HEX):i -D

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all flash clean

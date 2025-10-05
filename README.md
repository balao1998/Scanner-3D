

# Development of LABSI project in ISEP.
# AVR Microcontroller Project


## 🛠 Dependencies

To build and upload the firmware, you need the following tools installed on your Linux system:

- `gcc-avr` – AVR GCC compiler
- `avr-libc` – AVR C library
- `make` – Build automation tool
- `avrdude` – Tool for flashing AVR microcontrollers

### 📦 Installation

Run the following commands in your terminal:

```bash
sudo apt update
sudo apt install gcc-avr avr-libc make avrdude
```

## Compiling

In the root of the project run

```bash
make
```

## Uploading

```bash
make flash
```



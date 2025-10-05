// Set
#define SET_BIT(port, bit)     ((port) |= (1 << (bit)))

// Clear
#define CLEAR_BIT(port, bit)   ((port) &= ~(1 << (bit)))

// Toggle
#define TOGGLE_BIT(port, bit)  ((port) ^= (1 << (bit)))

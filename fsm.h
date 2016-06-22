#include <stdint.h>

struct arc {
    bool (*cause)();
    void (*effect)();
    struct arc *next;
    uint8_t state;
};

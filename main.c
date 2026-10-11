#include <stdio.h>

typedef struct {
    int cycle;
    char input_pins[8];
    char expected_pins[8];
} TestVector;

int main() {
    TestVector vec1 = {1, "10100101", "01011010"};
    printf("Executing Test Vector Cycle %d: In=%s, Exp=%s\n", vec1.cycle, vec1.input_pins, vec1.expected_pins);
    return 0;
}
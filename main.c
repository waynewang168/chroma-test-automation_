#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int cycle;
    char input_pins[8];
    char expected_pins[8];
    float supply_voltage; // 新增：供電電壓監控
} TestVector;

// 模擬 UVLO (欠壓鎖定) 防呆機制
bool verify_vector(TestVector *vec) {
    if (vec->supply_voltage < 3.0) {
        printf("[FAIL] Cycle %d: Voltage Sag (%.2fV < 3.0V) -> UVLO Triggered!\n", vec->cycle, vec->supply_voltage);
        return false;
    }
    printf("[PASS] Cycle %d: In=%s, Exp=%s\n", vec->cycle, vec->input_pins, vec->expected_pins);
    return true;
}

int main() {
    TestVector normal_case = {1, "10100101", "01011010", 3.3};
    TestVector sag_case    = {2, "11110000", "00001111", 2.6}; // 模擬電壓塌陷

    verify_vector(&normal_case);
    verify_vector(&sag_case);

    return 0;
}
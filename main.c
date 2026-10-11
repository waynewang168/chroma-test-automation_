#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PIN_COUNT 8

typedef struct {
    int  cycle;
    char input_pins[PIN_COUNT + 1];     /* +1 給 '\0' */
    char expected_pins[PIN_COUNT + 1];
} TestVector;

typedef enum { VEC_OK = 0, VEC_ERR_NULL, VEC_ERR_LEN,
               VEC_ERR_CHAR, VEC_ERR_CYCLE } VecStatus;

typedef enum { CMP_PASS = 0, CMP_FAIL, CMP_TIMEOUT, CMP_BAD_ARG } CmpResult;

/* ---------- Edge Case 1：建構時驗證 ---------- */
static bool valid_input_char(char c)  { return c == '0' || c == '1'; }
static bool valid_expect_char(char c) { return c == '0' || c == '1' || c == 'X'; }

static VecStatus vec_init(TestVector *v, int cycle,
                          const char *in, const char *exp)
{
    if (!v || !in || !exp)                 return VEC_ERR_NULL;
    if (cycle < 0)                         return VEC_ERR_CYCLE;
    if (strlen(in) != PIN_COUNT ||
        strlen(exp) != PIN_COUNT)          return VEC_ERR_LEN;

    for (int i = 0; i < PIN_COUNT; i++) {
        if (!valid_input_char(in[i]) || !valid_expect_char(exp[i]))
            return VEC_ERR_CHAR;
    }

    v->cycle = cycle;
    memcpy(v->input_pins,    in,  PIN_COUNT);
    memcpy(v->expected_pins, exp, PIN_COUNT);
    v->input_pins[PIN_COUNT]    = '\0';    /* 確保一定有結尾 */
    v->expected_pins[PIN_COUNT] = '\0';
    return VEC_OK;
}

/* ---------- Edge Case 2：DUT 回讀（此處為模擬，實機請換成硬體驅動） ---------- */
/* 回傳 0 = 成功，-1 = 逾時/硬體無回應 */
static int dut_read_pins(char *out, size_t n, uint32_t timeout_ms)
{
    (void)timeout_ms;
    if (!out || n < PIN_COUNT + 1) return -1;
    memcpy(out, "01011010", PIN_COUNT);    /* 模擬讀回值 */
    out[PIN_COUNT] = '\0';
    return 0;
}

static CmpResult vec_compare(const TestVector *v, uint32_t timeout_ms)
{
    if (!v) return CMP_BAD_ARG;

    char actual[PIN_COUNT + 1] = {0};      /* 先清零，避免殘留資料造成誤判 PASS */
    if (dut_read_pins(actual, sizeof actual, timeout_ms) != 0) {
        fprintf(stderr, "[Cycle %d] DUT read timeout (%u ms)\n",
                v->cycle, (unsigned)timeout_ms);
        return CMP_TIMEOUT;
    }

    CmpResult result = CMP_PASS;
    for (int i = 0; i < PIN_COUNT; i++) {
        if (v->expected_pins[i] == 'X') continue;      /* Don't Care 略過 */
        if (actual[i] != v->expected_pins[i]) {
            fprintf(stderr, "[Cycle %d] Pin %d mismatch: exp=%c act=%c\n",
                    v->cycle, i, v->expected_pins[i], actual[i]);
            result = CMP_FAIL;                         /* 繼續掃完，列出所有失敗腳位 */
        }
    }
    return result;
}

int main(void)
{
    TestVector vec1;
    VecStatus s = vec_init(&vec1, 1, "10100101", "01011010");
    if (s != VEC_OK) {
        fprintf(stderr, "Invalid test vector, status=%d\n", s);
        return 2;
    }

    printf("Executing Test Vector Cycle %d: In=%s, Exp=%s\n",
           vec1.cycle, vec1.input_pins, vec1.expected_pins);

    CmpResult r = vec_compare(&vec1, 100 /* ms */);
    printf("Cycle %d result: %s\n", vec1.cycle,
           r == CMP_PASS ? "PASS" :
           r == CMP_FAIL ? "FAIL" : "ERROR");

    return (r == CMP_PASS) ? 0 : 1;        /* 讓上層排程能依回傳值判斷 */
}
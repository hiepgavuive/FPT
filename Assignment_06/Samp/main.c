#include <stdio.h>
#include <stdint.h>

/*******************************************************************************
* Definitions
******************************************************************************/
/* Macro to set the nth bit in a 32-bit register*/
#define REG_BIT_SET32(add, n) (*(volatile uint32_t *)(add) |= (1U << (n)))

/* Macro to clear the nth bit in a 32-bit register*/
#define REG_BIT_CLEAR32(add, n) (*(volatile uint32_t *)(add) &= ~(1U << (n)))

/* Macro to swap bytes in a 32-bit value (big endian to little endian)*/
#define SWAP_BYTE32(add) do { \
    uint32_t *p = (uint32_t *)(add); \
    *p = ((*p >> 24) & 0x000000FF) | \
         ((*p >> 8) & 0x0000FF00) | \
         ((*p << 8) & 0x00FF0000) | \
         ((*p << 24) & 0xFF000000); \
} while(0)

/* Macro to swap bytes in a 16-bit value (big endian to little endian)*/
#define SWAP_BYTE16(add) do { \
    uint16_t *p = (uint16_t *)(add); \
    *p = ((*p >> 8) & 0x00FF) | \
         ((*p << 8) & 0xFF00); \
} while(0)


/*******************************************************************************
* Prototypes
******************************************************************************/
void test_macros() 
{
    /* Test SWAP_BYTE32 */
    uint32_t a = 0x12345678;
    SWAP_BYTE32(&a);
    printf("SWAP_BYTE32: Expected 0x78563412, Got 0x%08X\n", a);

    /* Test SWAP_BYTE16 */
    uint16_t b = 0x1234;
    SWAP_BYTE16(&b);
    printf("SWAP_BYTE16: Expected 0x3412, Got 0x%04X\n", b);

    /* Test REG_BIT_SET32 */
    uint32_t c = 0;
    REG_BIT_SET32(&c, 2);
    printf("REG_BIT_SET32: Expected 0x00000004, Got 0x%08X\n", c);

    /* Test REG_BIT_CLEAR32 */
    REG_BIT_CLEAR32(&c, 2);                                                                                                                                                                                                                 
    printf("REG_BIT_CLEAR32: Expected 0x00000000, Got 0x%08X\n", c);

    /* Additional test cases */
    REG_BIT_SET32(&c, 31); /* Set the MSB */
    printf("REG_BIT_SET32: Expected 0x80000000, Got 0x%08X\n", c);

    REG_BIT_CLEAR32(&c, 31); /* Clear the MSB */
    printf("REG_BIT_CLEAR32: Expected 0x00000000, Got 0x%08X\n", c);
}

int main(int argc, char* argv[]) 
{
    test_macros();
    return 0;
}

#ifndef CONFIG_DEFINES_H
#define CONFIG_DEFINES_H




#define CONTROL_PORT 4939

#define NUM_CALLS 4
#define DEFAULT_SERVER ""
#define IAX_DELAY 300 // delay between calls in milliseconds
#define TEST_FREQ 910.00

#ifndef PI
    #define PI 3.14159265358979323844
#endif

#define INT16_MAX 0x7FFF
#define INT16_MIN (-INT16_MAX - 1)
#define INT32_MAX (2147483647)
#define INT32_MIN (-2147483647 - 1)
#define UINT16_MAX 65535
#define UINT8_MAX 255

#endif // CONFIG_DEFINES_H

#include <stdbool.h>
#include <stdint.h>

#include <arch/antares.h>

#include <avr/io.h>
#include <util/delay.h>

#include "fix_assert.h"

// Output pin that controls load.
#define OUT_LOAD_DDR DDRB
#define OUT_LOAD_PORT PORTB
#define OUT_LOAD_N PB0

static void set_state(volatile uint8_t *port, int n, bool enabled)
{
    if (enabled) {
        *port |= _BV(n);
    } else {
        *port &= ~_BV(n);
    }
}

static void set_load_state(bool enabled)
{
    set_state(&OUT_LOAD_PORT, OUT_LOAD_N, enabled);
}

ANTARES_INIT_LOW(io_init)
{
    OUT_LOAD_DDR |= _BV(OUT_LOAD_N);
}

#if CONFIG_GWD_STARTUP_SLEEP_MS != 0
static_assert((CONFIG_GWD_STARTUP_SLEEP_MS > 0),
        "Negative CONFIG_GWD_STARTUP_SLEEP_MS");
static_assert((CONFIG_GWD_STARTUP_SLEEP_MS <= 60000),
        "CONFIG_GWD_STARTUP_SLEEP_MS too large");

ANTARES_APP(startup_sleep)
{
    _delay_ms(CONFIG_GWD_STARTUP_SLEEP_MS);
}
#endif

ANTARES_APP(watchdog)
{
    set_load_state(true);
    while (true) {}
}

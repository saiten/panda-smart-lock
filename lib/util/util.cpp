#include "util.h"
#include <nrf_wdt.h>

namespace util {

void software_reset(int maxPeriodMS) {
    nrf_wdt_behaviour_set(NRF_WDT_BEHAVIOUR_RUN_SLEEP);
    nrf_wdt_reload_value_set((maxPeriodMS * 32768) / 1000);
    nrf_wdt_reload_request_enable(NRF_WDT_RR0);
    nrf_wdt_task_trigger(NRF_WDT_TASK_START);

    while(1) {
    }
}

}